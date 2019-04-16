// Copyright 2019 The Beam Team
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bitcoind016.h"

#include "bitcoin/bitcoin.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

namespace beam
{    
    namespace
    {
        std::string generateAuthorization(const std::string& userName, const std::string& pass)
        {
            std::string userWithPass(userName + ":" + pass);
            libbitcoin::data_chunk t(userWithPass.begin(), userWithPass.end());
            return std::string("Basic " + libbitcoin::encode_base64(t));
        }
    }

    Bitcoind016::Bitcoind016(io::Reactor& reactor, const std::string& userName, const std::string& pass, const io::Address& address)
        : m_httpClient(reactor)
        , m_address(address)
        , m_authorization(generateAuthorization(userName, pass))
    {
    }

    void Bitcoind016::dumpPrivKey(const std::string& btcAddress, std::function<void(const std::string&, const std::string&)> callback)
    {
        sendRequest("dumpprivkey", "\"" + btcAddress + "\"", [callback] (const std::string& response){
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "": reply["error"].get<std::string>();

            callback(error, reply["result"].get<std::string>());
        });
    }

    void Bitcoind016::fundRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&, int)> callback)
    {
        sendRequest("fundrawtransaction", "\"" + rawTx + "\"", [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();
            const auto& result = reply["result"];

            callback(error, result["hex"].get<std::string>(), result["changepos"].get<int>());
        });
    }

    void Bitcoind016::signRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&, bool)> callback)
    {
        sendRequest("signrawtransaction", "\"" + rawTx + "\"", [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();
            const auto& result = reply["result"];

            callback(error, result["hex"].get<std::string>(), result["complete"].get<bool>());
        });
    }

    void Bitcoind016::sendRawTransaction(const std::string& rawTx, std::function<void(const std::string&, const std::string&)> callback)
    {
        sendRequest("sendrawtransaction", "\"" + rawTx + "\"", [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();

            callback(error, reply["result"].get<std::string>());
        });
    }

    void Bitcoind016::getRawChangeAddress(std::function<void(const std::string&, const std::string&)> callback)
    {
        sendRequest("getrawchangeaddress", "\"legacy\"", [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();

            callback(error, reply["result"].get<std::string>());
        });
    }

    void Bitcoind016::createRawTransaction(
        const std::string& withdrawAddress,
        const std::string& contractTxId,
        uint64_t amount,
        int outputIndex,
        Timestamp locktime,
        std::function<void(const std::string&, const std::string&)> callback)
    {
        std::string args("[{\"txid\": \"" + contractTxId + "\", \"vout\":" + std::to_string(outputIndex) + ", \"Sequence\": " + std::to_string(libbitcoin::max_input_sequence - 1) + " }]");

        args += ",{\"" + withdrawAddress + "\": " + std::to_string(double(amount) / libbitcoin::satoshi_per_bitcoin) + "}";
        if (locktime)
        {
            args += "," + std::to_string(locktime);
        }
        sendRequest("createrawtransaction", args, [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();

            callback(error, reply["result"].get<std::string>());
        });
    }

    void Bitcoind016::getTxOut(const std::string& txid, int outputIndex, std::function<void(const std::string&, const std::string&, double, uint16_t)> callback)
    {
        sendRequest("gettxout", "\"" + txid + "\"" + "," + std::to_string(outputIndex), [callback](const std::string& response) {
            json reply = json::parse(response);
            std::string error = reply["error"].empty() ? "" : reply["error"].get<std::string>();
            const auto& result = reply["result"];

            callback(error, result["scriptPubKey"]["hex"], result["value"].get<double>(), result["confirmations"]);
        });
    }

    void Bitcoind016::sendRequest(const std::string& method, const std::string& params, std::function<void(const std::string&)> callback)
    {
        const std::string content(R"({"method":")" + method + R"(","params":[)" + params + "]}");
        const HeaderPair headers[] = {
            {"Authorization", m_authorization.data()}
        };
        HttpClient::Request request;

        request.address(m_address)
            .connectTimeoutMsec(2000)
            .pathAndQuery("/")
            .headers(headers)
            .numHeaders(1)
            .method("POST")
            .body(content.c_str(), content.size());

        request.callback([callback](uint64_t id, const HttpMsgReader::Message& msg) -> bool {
            size_t sz = 0;
            const void* body = msg.msg->get_body(sz);
            if (sz > 0 && body)
            {
                callback(std::string(static_cast<const char*>(body), sz));
            }
            else
            {
                callback("");
            }
            return false;
        });

        m_httpClient.send_request(request);
    }
}