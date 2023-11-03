/* File: /lib/application/keyboard.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__application__keyboard__hpp
#define CPPLIB__lib__application__keyboard__hpp

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/delegate.hpp"
#include "../../lib/types.hpp"
#include "../../lib/utils/enum.hpp"
#include "../../lib/utils/updateable.hpp"

namespace lib::application {

// DEFINITION: lib::application::IKeyboard

class IKeyboard
	: tl::noncopymovable< IKeyboard >
	, public tag_utils_updateable<bool>
{
public:
	LIB_UTILS_ENUM( Key : u16
		,CTRL,ALT,SHIFT,ENTER,SPACE,ESCAPE,DELETE,BACKSPACE
		,PLUS,MINUS,CAPS
		,UP,DOWN,LEFT,RIGHT
		,A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z
		,ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE
		)

	template< class T >
	static constexpr Key MapKeyAZ( T key_a, T value_key ) noexcept {
		const auto offset = value_key - key_a;
		return (Key) ( utils::EnumValue( Key::A ) + offset );
	}

	template< class T >
	static constexpr Key MapKey09( T key_0, T value_key ) noexcept {
		const auto offset = value_key - key_0;
		return (Key) ( utils::EnumValue( Key::ZERO ) + offset );
	}

	struct Message {
		c32 symbol;
		Key key;
		bool down;
		u32 count;
		constexpr Message( c32 symbol, Key key, bool down, u32 count )
			: symbol{ symbol }, key{ key }, down{ down }, count{ count } {}
		constexpr auto set_handled() const noexcept { return ++handled_count_; }
		constexpr auto handled_count() const noexcept { return handled_count_; }
		constexpr bool is_handled() const noexcept { return handled_count_ > 0; }
	private:
		mutable usize handled_count_ = 0;
	};

	virtual ~IKeyboard() = default;

	using tag_utils_updateable<bool>::update;

	virtual bool is_pressed( Key key ) const noexcept = 0;

	tl::delegate<const Message&> on_key;
};

} // namespace lib::application

#endif // CPPLIB__lib__application__keyboard__hpp
