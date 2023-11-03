/* File: test.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/debug/platform.hpp>

#include <lib/test/main.hpp>

#include <test/lib/tl/carray.hpp>
#include <test/lib/tl/delegate.hpp>
#include <test/lib/tl/listener.hpp>

#include <test/lib/bus.hpp>
#include <test/lib/cli.hpp>
#include <test/lib/ecs.hpp>
#include <test/lib/flags.hpp>

#include <test/lib/file/ini.hpp>
#include <test/lib/packets/handler.hpp>

#include <test/lib/strutils.hpp>
#include <test/lib/utils/case_string.hpp>
#include <test/lib/utils/enum.hpp>
#include <test/lib/utils/ids_pool.hpp>
#include <test/lib/utils/value.hpp>

#include <test/lib/impl/codec/base64.hpp>
#include <test/lib/impl/hash/sha1.hpp>

#include <test/lib/impl/socket/tcp.hpp>

#ifdef CPPLIB_PLATFORM_POSIX
	#include <test/lib/impl_posix/socket/unix.hpp>
	#include <test/lib/impl_posix/application/termios_keyboard.hpp>
#endif // CPPLIB_PLATFORM_POSIX

int main() {
	CPPLIB__TEST_SYSTEM_INFO;

	CPPLIB__TEST_MAIN_BEGIN;

#ifdef CPPLIB__test__lib__tl__carray__hpp
	CPPLIB__TEST_RUN( ::test::lib::tl::CArray );
#endif // CPPLIB__test__lib__tl__carray__hpp

#ifdef CPPLIB__test__lib__tl__delegate__hpp
	CPPLIB__TEST_RUN( ::test::lib::tl::Delegate );
#endif // CPPLIB__test__lib__tl__delegate__hpp

#ifdef CPPLIB__test__lib__tl__listener__hpp
	CPPLIB__TEST_RUN( ::test::lib::tl::Listener );
#endif // CPPLIB__test__lib__tl__listener__hpp


#ifdef CPPLIB__test__lib__bus__hpp
	CPPLIB__TEST_RUN( ::test::lib::Bus );
#endif // CPPLIB__test__lib__bus__hpp

#ifdef CPPLIB__test__lib__cli__hpp
	/// @todo Provide test CLI script for automatic testing.
	CPPLIB__TEST_RUN( ::test::lib::Cli );
#endif // CPPLIB__test__lib__cli__hpp

#ifdef CPPLIB__test__lib__ecs__hpp
	CPPLIB__TEST_RUN( ::test::lib::Ecs );
#endif // CPPLIB__test__lib__ecs__hpp

#ifdef CPPLIB__test__lib__flags__hpp
	CPPLIB__TEST_RUN( ::test::lib::Flags );
#endif // CPPLIB__test__lib__flags__hpp


#ifdef CPPLIB__test__lib__file__ini__hpp
	CPPLIB__TEST_RUN( ::test::lib::file::Ini );
#endif // CPPLIB__test__lib__file__ini__hpp

#ifdef CPPLIB__test__lib__packets__handler__hpp
	CPPLIB__TEST_RUN( ::test::lib::packets::Handler );
#endif // CPPLIB__test__lib__packets__handler__hpp


#ifdef CPPLIB__test__lib__strutils__hpp
	CPPLIB__TEST_RUN( ::test::lib::StrUtils );
#endif // CPPLIB__test__lib__strutils__hpp

#ifdef CPPLIB__test__lib__utils__case_string__hpp
	CPPLIB__TEST_RUN( ::test::lib::utils::CaseString );
#endif // CPPLIB__test__lib__utils__case_string__hpp

#ifdef CPPLIB__test__lib__utils__enum__hpp
	CPPLIB__TEST_RUN( ::test::lib::utils::Enum );
#endif // CPPLIB__test__lib__utils__enum__hpp

#ifdef CPPLIB__test__lib__utils__ids_pool__hpp
	CPPLIB__TEST_RUN( ::test::lib::utils::IdsPool );
#endif // CPPLIB__test__lib__utils__ids_pool__hpp

#ifdef CPPLIB__test__lib__utils__value__hpp
	CPPLIB__TEST_RUN( ::test::lib::utils::Value );
#endif // CPPLIB__test__lib__utils__value__hpp


#ifdef CPPLIB__test__lib__impl__codec__base64__hpp
	CPPLIB__TEST_RUN( ::test::lib::codec::impl::Base64 );
#endif // CPPLIB__test__lib__impl__codec__base64__hpp

#ifdef CPPLIB__test__lib__impl__hash__sha1__hpp
	CPPLIB__TEST_RUN( ::test::lib::hash::impl::Sha1 );
#endif // CPPLIB__test__lib__impl__hash__sha1__hpp


#ifdef CPPLIB__test__lib__impl__socket__tcp__hpp
	CPPLIB__TEST_RUN( ::test::lib::socket::impl::Tcp );
#endif // CPPLIB__test__lib__impl__socket__tcp__hpp


#ifdef CPPLIB__test__lib__impl_posix__socket__unix__hpp
	CPPLIB__TEST_RUN( ::test::lib::socket::impl::Unix );
#endif // CPPLIB__test__lib__impl_posix__socket__unix__hpp

#ifdef CPPLIB__test__lib__impl_posix__application__termios_keyboard__hpp
	CPPLIB__TEST_RUN( ::test::lib::application::impl::TermiosKeyboard );
#endif // CPPLIB__test__lib__impl_posix__application__termios_keyboard__hpp

	CPPLIB__TEST_MAIN_END;
}
