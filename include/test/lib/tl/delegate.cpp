/* File: /test/lib/tl/delegate.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <memory>

#include <lib/ptr.hpp>

#include <lib/tl/delegate.hpp>

#include "./delegate.hpp"

namespace test::lib::tl {

void Delegate::test_execute() noexcept/* override*/ {
	using ::lib::tag_tl_listener;
	using ::lib::tl::delegate;

	struct Foo : tag_tl_listener<Foo>, ::std::enable_shared_from_this<Foo> {
		void increment( int i ) {
			if ( on_increment_remove_increment ) *calc -= { *this, &Foo::increment };
			value += i; }
		void multiply( int i ) {
			if ( on_multiply_remove_decrement ) *calc -= { *this, &Foo::decrement };
			value *= i; }
		void decrement( int i ) {
			if ( on_decrement_add_divide ) *calc += { *this, &Foo::divide };
			value -= i; }
		void divide( int i ) { value /= i; }
		int value = 9;
		delegate<int> * calc = nullptr;
		bool on_increment_remove_increment = false;
		bool on_multiply_remove_decrement = false;
		bool on_decrement_add_divide = false;
	};

	delegate<int> calc;

	auto foo = ::lib::MkSPtr<Foo>();
	foo->calc = &calc;

	calc( 123 );	///< nop

	calc += { foo, &Foo::increment };
	calc( 4 );		///< 9 + 4 = 13
	CPPLIB__TEST__EQ( foo->value, 13 );

	calc += { foo, &Foo::multiply };
	calc( 2 );		///< (13 + 2) * 2 = 30
	CPPLIB__TEST__EQ( foo->value, 30 );

	calc += { foo, &Foo::decrement };
	calc( 2 );		///< (30 + 2) * 2 - 2 = 62
	CPPLIB__TEST__EQ( foo->value, 62 );

	foo->on_decrement_add_divide = true;
	calc( 15 );		///< ( (62 + 15) * 15 - 15 `add div` ) / 15 = 76
	CPPLIB__TEST__EQ( foo->value, 76 );
	foo->on_decrement_add_divide = false;

	foo->on_increment_remove_increment = true;
	calc( -12 );	///< ( (76 - 12) `rem inc` * (-12) + 12 ) / (-12) = 63
	CPPLIB__TEST__EQ( foo->value, 63 );
	foo->on_increment_remove_increment = false;

	foo->on_multiply_remove_decrement = true;
	calc( 2 );		///< 63 * 2 `rem dec` / 2 = 63
	CPPLIB__TEST__EQ( foo->value, 63 );
	foo->on_multiply_remove_decrement = false;

	foo.reset();

	calc( 123 );	///< nop
}

} // namespace test::lib::tl
