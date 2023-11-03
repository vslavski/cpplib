/* File: /test/lib/impl_posix/application/termios_keyboard.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__impl_posix__application__termios_keyboard__hpp
#define CPPLIB__test__lib__impl_posix__application__termios_keyboard__hpp

#include <lib/test/unit.hpp>

#include <lib/tl/listener.hpp>
#include <lib/impl/stream/fifo.hpp>

#include <lib/impl_posix/application/termios_keyboard.hpp>

namespace test::lib::application::impl {

class TermiosKeyboard final
	: public ::lib::test::IUnit
	, public ::lib::tag_tl_listener< TermiosKeyboard >
{
public:
	TermiosKeyboard() noexcept;
private:
	void test_execute() noexcept override;
	void test_reset() noexcept override;

	void onKey( const ::lib::application::IKeyboard::Message & on_key ) noexcept;

	::lib::stream::impl::fifo stream;
	::lib::application::impl::TermiosKeyboard keyboard;
	::lib::usize message_idx;
	bool done;
};

} // namespace test::lib::application::impl

#endif // CPPLIB__test__lib__impl_posix__application__termios_keyboard__hpp
