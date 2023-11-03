/* File: /lib/debug/features.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__features__hpp
#define CPPLIB__lib__debug__features__hpp

#include "./compiler.hpp"

// DECLARATION CPPLIB_ENABLE_EBO

#ifdef CPPLIB_COMPILER_MSVC
	#define CPPLIB_ENABLE_EBO	__declspec(empty_bases)
#else
	#define CPPLIB_ENABLE_EBO
#endif // CPPLIB_COMPILER_MSVC

// DECLARATION CPPLIB_PRINTF_ANNOTATION, CPPLIB_PRINTF_ATTRIBUTE

#ifdef CPPLIB_COMPILER_MSVC
	#define CPPLIB_PRINTF_ANNOTATION		_Printf_format_string_
	#define CPPLIB_PRINTF_ATTRIBUTE(...)
#else
	#define CPPLIB_PRINTF_ANNOTATION
	#define CPPLIB_PRINTF_ATTRIBUTE(...)	__attribute__(( __VA_ARGS__ ))
#endif // CPPLIB_COMPILER_MSVC

// DECLARATION CPPLIB_MSVC_WARNING

#ifdef CPPLIB_COMPILER_MSVC
	#define CPPLIB_MSVC_WARNING(...)		__pragma(warning(__VA_ARGS__))
#else
	#define CPPLIB_MSVC_WARNING(...)
#endif // CPPLIB_COMPILER_MSVC

// DECLARATION CPPLIB_GCC_CLANG_DIAGNOSTIC

#if defined(CPPLIB_COMPILER_CLANG) || defined(CPPLIB_COMPILER_MINGW) || defined(CPPLIB_COMPILER_GCC)
	#define CPPLIB_GCC_CLANG_PRAGMA(...)			_Pragma(#__VA_ARGS__)
	#define CPPLIB_GCC_CLANG_DIAGNOSTIC(...)		CPPLIB_GCC_CLANG_PRAGMA(GCC diagnostic __VA_ARGS__)
#else
	#define CPPLIB_GCC_CLANG_DIAGNOSTIC(...)
#endif // CPPLIB_COMPILER_CLANG or CPPLIB_COMPILER_MINGW or CPPLIB_COMPILER_GCC

// DECLARATION CPPLIB_GCC_DIAGNOSTIC

#if defined(CPPLIB_COMPILER_GCC) || defined(CPPLIB_COMPILER_MINGW)
	#define CPPLIB_GCC_DIAGNOSTIC(...)				CPPLIB_GCC_CLANG_DIAGNOSTIC(__VA_ARGS__)
#else
	#define CPPLIB_GCC_DIAGNOSTIC(...)
#endif // CPPLIB_COMPILER_GCC or CPPLIB_COMPILER_MINGW

// DECLARATION CPPLIB_CLANG_DIAGNOSTIC

#if defined(CPPLIB_COMPILER_CLANG)
	#define CPPLIB_CLANG_DIAGNOSTIC(...)			CPPLIB_GCC_CLANG_PRAGMA(clang diagnostic __VA_ARGS__)
#else
	#define CPPLIB_CLANG_DIAGNOSTIC(...)
#endif // CPPLIB_COMPILER_CLANG

#endif // CPPLIB__lib__debug__features__hpp
