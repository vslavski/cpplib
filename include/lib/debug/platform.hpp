/* File: /lib/debug/platform.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__platform__hpp
#define CPPLIB__lib__debug__platform__hpp

#include "../../lib/utils/enum.hpp"

#include "./os.hpp"

namespace lib::debug {

LIB_UTILS_ENUM( Platform
	, POSIX
	, CYGWIN
	, WINDOWS
	, UNKNOWN
	)

#if defined(CPPLIB_OS_ANDROID) || defined(CPPLIB_OS_MACOS) || defined(CPPLIB_OS_IOS)	\
	|| defined(CPPLIB_OS_LINUX)
	#define CPPLIB_PLATFORM_POSIX
	static constexpr auto platform = Platform::POSIX;
#elif defined(CPPLIB_OS_WINDOWS)
	#if defined(__CYGWIN__)
		#define CPPLIB_PLATFORM_CYGWIN
		static constexpr auto platform = Platform::CYGWIN;
	#else
		#define CPPLIB_PLATFORM_WINDOWS
		static constexpr auto platform = Platform::WINDOWS;
	#endif // __CYGWIN__
#else
	#define CPPLIB_PLATFORM_UNKNOWN
	static constexpr auto platform = Platform::UNKNOWN;
	#warning "Unknown platform"
#endif // ...platform...

} // namespace lib::debug

LIB_UTILS_ENUM_NAMES( lib::debug::Platform
	, "POSIX"
	, "CYGWIN"
	, "WINDOWS"
	, "UNKNOWN"
	)

#endif // CPPLIB__lib__debug__platform__hpp
