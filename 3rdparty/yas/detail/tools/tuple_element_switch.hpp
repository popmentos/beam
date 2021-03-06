
// Copyright (c) 2010-2018 niXman (i dot nixman dog gmail dot com). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__detail__tools__tuple_element_switch_hpp
#define __yas__detail__tools__tuple_element_switch_hpp

#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/preprocessor/preprocessor.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

#define __YAS_GENERATE_TUPLE_SWITCH_CB(unused0, idx, unused1) \
    case idx: { ar & std::get<idx>(t); return; };

template<typename Archive>
void tuple_switch(Archive &, std::size_t, std::tuple<> &) {}

#define __YAS_GENERATE_TUPLE_SWITCH(n) \
    template<typename Archive, YAS_PP_ENUM_PARAMS(n, typename T)> \
    void tuple_switch(Archive &ar, std::size_t idx, std::tuple<YAS_PP_ENUM_PARAMS(n, T)> &t) { \
        switch ( idx ) { \
            YAS_PP_REPEAT( \
                 n \
                ,__YAS_GENERATE_TUPLE_SWITCH_CB \
                ,~ \
            ) \
            default: return; \
        } \
    }

__YAS_GENERATE_TUPLE_SWITCH(1 )
__YAS_GENERATE_TUPLE_SWITCH(2 )
__YAS_GENERATE_TUPLE_SWITCH(3 )
__YAS_GENERATE_TUPLE_SWITCH(4 )
__YAS_GENERATE_TUPLE_SWITCH(5 )
__YAS_GENERATE_TUPLE_SWITCH(6 )
__YAS_GENERATE_TUPLE_SWITCH(7 )
__YAS_GENERATE_TUPLE_SWITCH(8 )
__YAS_GENERATE_TUPLE_SWITCH(9 )
__YAS_GENERATE_TUPLE_SWITCH(10)
__YAS_GENERATE_TUPLE_SWITCH(11)
__YAS_GENERATE_TUPLE_SWITCH(12)
__YAS_GENERATE_TUPLE_SWITCH(13)
__YAS_GENERATE_TUPLE_SWITCH(14)
__YAS_GENERATE_TUPLE_SWITCH(15)
__YAS_GENERATE_TUPLE_SWITCH(16)
__YAS_GENERATE_TUPLE_SWITCH(17)
__YAS_GENERATE_TUPLE_SWITCH(18)
__YAS_GENERATE_TUPLE_SWITCH(19)
__YAS_GENERATE_TUPLE_SWITCH(20)
//__YAS_GENERATE_TUPLE_SWITCH(21)
//__YAS_GENERATE_TUPLE_SWITCH(22)
//__YAS_GENERATE_TUPLE_SWITCH(23)
//__YAS_GENERATE_TUPLE_SWITCH(24)
//__YAS_GENERATE_TUPLE_SWITCH(25)
//__YAS_GENERATE_TUPLE_SWITCH(26)
//__YAS_GENERATE_TUPLE_SWITCH(27)
//__YAS_GENERATE_TUPLE_SWITCH(28)
//__YAS_GENERATE_TUPLE_SWITCH(29)
//__YAS_GENERATE_TUPLE_SWITCH(30)
//__YAS_GENERATE_TUPLE_SWITCH(31)
//__YAS_GENERATE_TUPLE_SWITCH(32)
//__YAS_GENERATE_TUPLE_SWITCH(33)
//__YAS_GENERATE_TUPLE_SWITCH(34)
//__YAS_GENERATE_TUPLE_SWITCH(35)
//__YAS_GENERATE_TUPLE_SWITCH(36)
//__YAS_GENERATE_TUPLE_SWITCH(37)
//__YAS_GENERATE_TUPLE_SWITCH(38)
//__YAS_GENERATE_TUPLE_SWITCH(39)
//__YAS_GENERATE_TUPLE_SWITCH(40)
//__YAS_GENERATE_TUPLE_SWITCH(41)
//__YAS_GENERATE_TUPLE_SWITCH(42)
//__YAS_GENERATE_TUPLE_SWITCH(43)
//__YAS_GENERATE_TUPLE_SWITCH(44)
//__YAS_GENERATE_TUPLE_SWITCH(45)
//__YAS_GENERATE_TUPLE_SWITCH(46)
//__YAS_GENERATE_TUPLE_SWITCH(47)
//__YAS_GENERATE_TUPLE_SWITCH(48)
//__YAS_GENERATE_TUPLE_SWITCH(49)
//__YAS_GENERATE_TUPLE_SWITCH(50)

#undef __YAS_GENERATE_TUPLE_SWITCH_CB
#undef __YAS_GENERATE_TUPLE_SWITCH

/***************************************************************************/

} // ns detail
} // ns yas

#endif // __yas__detail__tools__tuple_element_switch_hpp
