/* File: /test/lib/tl/listener.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <memory>

#include <lib/debug/features.hpp>

#include <lib/ptr.hpp>

#include <lib/tl/listener.hpp>

#include "./listener.hpp"

namespace test::lib::tl {

void Listener::test_execute() noexcept/* override*/ {
	using ::lib::tag_tl_listener;
	using ::lib::tl::listener;

	struct Foo : tag_tl_listener<Foo> {
		Foo( int v ) : value{ v } {}
		int increment( int v ) { return value += v; }
		int decrement( int v ) { return value -= v; }
		Foo * if_self_ptr( Foo * ptr ) { return this == ptr ? this : nullptr; }
		int value;
	};
	struct Bar : Foo, tag_tl_listener<Bar>, ::std::enable_shared_from_this<Bar> {
		Bar( int v ) : Foo{ v } {}
		int multiply( int v ) { return bar_value = value *= v; }
		int divide( int v ) { return bar_value = value /= v; }
		Bar * if_foo_ptr( Foo * ptr ) { return (Foo*)this == ptr ? this : nullptr; }
		int bar_value;
	};

	listener<int, int> calc;
	CPPLIB__TEST__FALSE( calc );
	CPPLIB__TEST__EQ( calc( 12345678 ), 0 );

	/** @bug GCC 12 "-Wmaybe-uninitialized" issued with "-O3".
	 *  Due to possible instructions reorder or bug in "maybe-uninitialized" logic,
	 *  call to `calc( 223 )` considered as occured before `Foo foo{ 123 };` call.
	 */
	Foo foo{ 123 };
	///*STATIC ASSERT*/calc = { foo, &Bar::multiply };
	calc = { foo, &Foo::decrement };
	CPPLIB__TEST__TRUE( calc );
	CPPLIB_GCC_DIAGNOSTIC(ignored "-Wmaybe-uninitialized")
	CPPLIB__TEST__EQ( calc( 223 ), -100 );
	CPPLIB_GCC_DIAGNOSTIC(error "-Wmaybe-uninitialized")
	calc.reset();
	CPPLIB__TEST__EQ( calc( 7654321 ), 0 );

	auto bar = ::lib::MkSPtr<Bar>( 21 );
	calc = { /*(Bar&)*/*bar, &Bar::increment };
	CPPLIB__TEST__EQ( calc( 21 ), 42 );
	bar.reset();
	CPPLIB__TEST__EQ( calc( 123456 ), 0 );

	bar = ::lib::MkSPtr<Bar>( 63 );
	calc.reset( bar, &Bar::divide );
	CPPLIB__TEST__EQ( calc( 3 ), 21 );
	CPPLIB__TEST__EQ( bar->value, 21 );
	CPPLIB__TEST__EQ( bar->bar_value, 21 );

	listener<void*, void*> ptr_check{ bar, &Bar::if_foo_ptr };
	CPPLIB__TEST__EQ( ptr_check( &bar ), nullptr );
	CPPLIB__TEST__EQ( ptr_check( (Foo*)bar.get() ), bar.get() );

	const auto * bar_dangling_ptr = bar.get();
	bar.reset();
	CPPLIB__TEST__TRUE( ptr_check );
	CPPLIB__TEST__EQ( ptr_check( (Foo*)bar_dangling_ptr ), nullptr );

	ptr_check = { foo, &Foo::if_self_ptr };
	CPPLIB__TEST__EQ( ptr_check( &foo ), &foo );
	CPPLIB__TEST__EQ( ptr_check( bar.get() ), nullptr );
}

} // namespace test::lib::tl
