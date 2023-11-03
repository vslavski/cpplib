/* File: /lib/types.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__types__hpp
#define CPPLIB__lib__types__hpp

#include <cstdint>
#include <cstddef>

namespace lib {

// DECLARATION lib::i8, lib::i16, lib::i32, lib::i64

using i8	= ::std::int8_t;
using i16	= ::std::int16_t;
using i32	= ::std::int32_t;
using i64	= ::std::int64_t;

// DECLARATION lib::u8, lib::u16, lib::u32, lib::u64

using u8	= ::std::uint8_t;
using u16	= ::std::uint16_t;
using u32	= ::std::uint32_t;
using u64	= ::std::uint64_t;

// DECLARATION lib::f32, lib::f64

using f32	= float;
using f64	= double;

// DECLARATION lib::usize, lib::isize

using usize	= ::std::size_t;
using isize = ::std::ptrdiff_t;

// DECLARATION lib::c8, lib::c16, lib::c32

using c8	= char8_t;
using c16	= char16_t;
using c32	= char32_t;

} // namespace lib

#endif // CPPLIB__lib__types__hpp
