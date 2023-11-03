/* File: /lib/impl/application/default_keyboard.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <algorithm>

#include "./default_keyboard.hpp"

namespace lib::application::impl {

// IMPLEMENTATION lib::application::impl::IDefaultKeyboard

IDefaultKeyboard::IDefaultKeyboard() noexcept {
	keys.fill( 0 );
}

bool IDefaultKeyboard::is_pressed( Key key ) const noexcept/* override*/ {
	return keys[ utils::EnumValue( key ) ] > 0;
}

/*virtual */void IDefaultKeyboard::key_down( Key key, u32 count/* = 1*/ ) noexcept {
	keys[ utils::EnumValue( key ) ] = ::std::max( 1u, count );
	on_key( { U'\0', key, true, count } );
}

/*virtual */void IDefaultKeyboard::key_up( Key key ) noexcept {
	keys[ utils::EnumValue( key ) ] = 0;
	on_key( { U'\0', key, false, 0 } );
}

/*virtual */void IDefaultKeyboard::symbol( c32 symbol, u32 count/* = 1*/ ) noexcept {
	on_key( { symbol, Key::invalid, false, ::std::max( 1u, count ) } );
}

} // namespace lib::application::impl
