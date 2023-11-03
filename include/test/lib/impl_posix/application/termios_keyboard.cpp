/* File: /test/lib/impl_posix/application/termios_keyboard.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cpp/lib_debug>

#include "./termios_keyboard.hpp"

namespace test::lib::application::impl {

namespace {

using Key = ::lib::application::IKeyboard::Key;
using Message = ::lib::application::IKeyboard::Message;

static constexpr char INPUT[] = "q!4\0330DAAA\033[1;3C<";
static constexpr ::lib::usize INPUT_SIZE = sizeof(INPUT)/sizeof(*INPUT) - 1/*no null terminator*/;
static constexpr Message OUTPUT[] =
	{{ U'\0'	,Key::Q			,true	,1 }
	,{ U'q'		,Key::invalid	,false	,1 }
	,{ U'\0'	,Key::Q			,false	,0 }

	,{ U'!'		,Key::invalid	,false	,1 }

	,{ U'\0'	,Key::FOUR		,true	,1 }
	,{ U'4'		,Key::invalid	,false	,1 }
	,{ U'\0'	,Key::FOUR		,false	,0 }

	,{ U'\0'	,Key::LEFT		,true	,1 }
	,{ U'\0'	,Key::LEFT		,false	,0 }

	,{ U'\0'	,Key::A			,true	,3 }
	,{ U'A'		,Key::invalid	,false	,3 }
	,{ U'\0'	,Key::A			,false	,0 }

	,{ U'\0'	,Key::RIGHT		,true	,1 }
	,{ U'\0'	,Key::RIGHT		,false	,0 }

	,{ U'<'		,Key::invalid	,false	,1 }
};
static constexpr ::lib::usize OUTPUT_SIZE = sizeof(OUTPUT)/sizeof(*OUTPUT);

} // namespace

TermiosKeyboard::TermiosKeyboard() noexcept
	: IUnit {"TermiosKeyboard"}
	, keyboard {stream}
	, message_idx {0}
	, done {false}
{
	keyboard.on_key += {*this,&TermiosKeyboard::onKey};
}

void TermiosKeyboard::test_execute() noexcept/* override*/ {
	CPPLIB__TEST__EQ( stream.write({ INPUT, INPUT_SIZE }), INPUT_SIZE );

	do {
		CPPLIB__TEST__TRUE( keyboard.update() );
		CPPLIB__TEST__STOP_FAILED();
	} while ( not done );
}

void TermiosKeyboard::test_reset() noexcept/* override*/ {
	(void) stream.flush();
	message_idx = 0;
	done = false;
}

void TermiosKeyboard::onKey( const ::lib::application::IKeyboard::Message & on_key ) noexcept {
	CPPLIB__TEST__STOP_FAILED();
	CPPLIB__TEST__LOOP_NEXT();

	CPPLIB__TEST__EQ( on_key.symbol, OUTPUT[message_idx].symbol );
	CPPLIB__TEST__EQ( on_key.key, OUTPUT[message_idx].key );
	CPPLIB__TEST__EQ( on_key.down, OUTPUT[message_idx].down );
	CPPLIB__TEST__EQ( on_key.count, OUTPUT[message_idx].count );

	CPPLIB__TEST__TRUE( on_key.key != Key::RIGHT or keyboard.is_pressed( Key::ALT ) );

	CPP_ASSERT( message_idx < OUTPUT_SIZE );
	done = ++message_idx == OUTPUT_SIZE;
}

} // namespace test::lib::application::impl
