/* File: /lib/impl/application/default_keyboard.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__application__default_keyboard__hpp
#define CPPLIB__lib__impl__application__default_keyboard__hpp

#include <array>

#include "../../../lib/types.hpp"
#include "../../../lib/utils/enum.hpp"

#include "../../../lib/application/keyboard.hpp"

namespace lib::application::impl {

// DEFINITION: lib::application::impl::IDefaultKeyboard

class IDefaultKeyboard
	: public IKeyboard
{
public:
	IDefaultKeyboard() noexcept;
	using IKeyboard::update;
	bool is_pressed( Key key ) const noexcept override;
protected:
	virtual void key_down( Key key, u32 count = 1 ) noexcept;
	virtual void key_up( Key key ) noexcept;
	virtual void symbol( c32 symbol, u32 count = 1 ) noexcept;
	::std::array< u32/*count*/, utils::EnumValue( Key::count ) > keys;
};

} // namespace lib::application::impl

#endif // CPPLIB__lib__impl__application__default_keyboard__hpp
