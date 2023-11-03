/* File: /test/lib/utils/value.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <string>

#include <lib/utils/value.hpp>

#include "./value.hpp"

namespace test::lib::utils {

void Value::test_execute() noexcept/* override*/ {
	using Value = ::lib::utils::Value<int, float>;

	Value v;
	CPPLIB__TEST__EQ( v, 0 );
	CPPLIB__TEST__EQ( v, 0.f );
	CPPLIB__TEST__EQ( v, "" );

	v = 123;
	CPPLIB__TEST__EQ( v, 123 );
	CPPLIB__TEST__EQf( v, 123.f );
	CPPLIB__TEST__EQ( v, "123" );

	v = 3.14159f;
	int i = v;/* OR */i = v.i();
	float f = v;/* OR */f = v.f();
	::std::string s = v;/* OR */s = v.s();
	CPPLIB__TEST__EQ( i, 3 );
	CPPLIB__TEST__EQf( f, 3.14159f );
	CPPLIB__TEST__EQ( 0, s.find( "3.14" ) );

	v = "string value";
	CPPLIB__TEST__EQ( v, 0 );
	CPPLIB__TEST__EQ( v, 0.f );
	CPPLIB__TEST__EQ( v, "string value" );

	struct Foo {};
	struct Bar {};
	v.any() = Foo{};
	CPPLIB__TEST__NE( ::std::any_cast<Foo>( &v.any() ), nullptr );
	CPPLIB__TEST__EQ( ::std::any_cast<Bar>( &v.any() ), nullptr );

	v = "true";
	CPPLIB__TEST__TRUE( v.b() );
	v = "false";
	CPPLIB__TEST__FALSE( v.b() );
}

} // namespace test::lib::utils
