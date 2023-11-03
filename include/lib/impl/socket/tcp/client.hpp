/* File: /lib/impl/socket/tcp/client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__socket__tcp__client__hpp
#define CPPLIB__lib__impl__socket__tcp__client__hpp

#include "../../../../lib/debug/os.hpp"
#include "../../../../lib/debug/platform.hpp"

#if defined(CPPLIB_PLATFORM_POSIX)
	#include "../../../../lib/impl_posix/socket/tcp/client.hpp"
#elif defined(CPPLIB_OS_WINDOWS)
	#include "../../../../lib/impl_windows/socket/tcp/client.hpp"
#else
	#error "No lib.socket.impl.tcp implementation found for current platform/OS."
#endif // CPPLIB_PLATFORM_POSIX or CPPLIB_OS_WINDOWS

#endif // CPPLIB__lib__impl__socket__tcp__client__hpp
