/* File: /lib/debug/compiler.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__compiler__hpp
#define CPPLIB__lib__debug__compiler__hpp

#include "../../lib/utils/enum.hpp"

namespace lib::debug {

LIB_UTILS_ENUM( Compiler
	, CLANG
	, MINGW
	, GCC
	, MSVC
	, UNKNOWN
	)

#if defined(__clang__)
	#define CPPLIB_COMPILER_CLANG
	static constexpr auto compiler = Compiler::CLANG;
#elif defined(__MINGW32__) // || __MINGW64__
	#define CPPLIB_COMPILER_MINGW
	static constexpr auto compiler = Compiler::MINGW;
#elif defined(__GNUC__)
	#define CPPLIB_COMPILER_GCC
	static constexpr auto compiler = Compiler::GCC;
#elif defined(_MSC_VER)
	#define CPPLIB_COMPILER_MSVC
	static constexpr auto compiler = Compiler::MSVC;
#else
	#define CPPLIB_COMPILER_UNKNOWN
	static constexpr auto compiler = Compiler::UNKNOWN;
	#warning "Unknown compiler"
#endif // ...compiler...

} // namespace lib::debug

LIB_UTILS_ENUM_NAMES( lib::debug::Compiler
	, "CLANG"
	, "MINGW"
	, "GCC"
	, "MSVC"
	, "UNKNOWN"
	)

#endif // CPPLIB__lib__debug__compiler__hpp
