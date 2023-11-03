/* File: /lib/debug/os.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__os__hpp
#define CPPLIB__lib__debug__os__hpp

#include "../../lib/utils/enum.hpp"

namespace lib::debug {

LIB_UTILS_ENUM( Os
	, ANDROID
	, MACOS
	, IOS
	, LINUX
	, WINDOWS
	, UNKNOWN
	)

#if defined(__ANDROID__)
	#define CPPLIB_OS_ANDROID
	static constexpr auto os = Os::ANDROID;
#elif defined(__APPLE__)
	#if defined(__MACH__)
		#define CPPLIB_OS_MACOS
		static constexpr auto os = Os::MACOS;
	#else
		#define CPPLIB_OS_IOS
		static constexpr auto os = Os::IOS;
	#endif // __MACH__
#elif defined(__linux__)
	#define CPPLIB_OS_LINUX
	static constexpr auto os = Os::LINUX;
#elif defined(_WIN32)
	#define CPPLIB_OS_WINDOWS
	static constexpr auto os = Os::WINDOWS;
#else
	#define CPPLIB_OS_UNKNOWN
	static constexpr auto os = Os::UNKNOWN;
	#warning "Unknown OS"
#endif // ...os...

} // namespace lib::debug

LIB_UTILS_ENUM_NAMES( lib::debug::Os
	, "ANDROID"
	, "MACOS"
	, "IOS"
	, "LINUX"
	, "WINDOWS"
	, "UNKNOWN"
	)

#endif // CPPLIB__lib__debug__os__hpp
