/* File: /test/lib/utils/enum.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/utils/enum.hpp>

#include "./enum.hpp"

namespace test::lib::utils {
namespace {

LIB_UTILS_ENUM( FooBar, BAR, BAZ, QUX )

} // namespace
} // namespace test::lib::utils

LIB_UTILS_ENUM_NAMES( ::test::lib::utils::FooBar, "bar", "Baz", "QUX" )

namespace test::lib::utils {

void Enum::test_execute() noexcept/* override*/ {
	CPPLIB__TEST__FALSE( ::lib::utils::is_valid( FooBar::invalid ) );
	CPPLIB__TEST__FALSE( ::lib::utils::is_valid< FooBar >( 4 ) );
	CPPLIB__TEST__TRUE( ::lib::utils::is_valid( FooBar::QUX ) );

	auto id = ::lib::utils::EnumValue( FooBar::invalid );
	bool result = ::lib::utils::foreach< FooBar >(
		[&id, this]( FooBar, decltype(id) i ) {
			CPPLIB__TEST__LOOP_NEXT();
			CPPLIB__TEST__EQv( ++id, i );
			return not CPPLIB__TEST__IS_FAILED();
		});
	CPPLIB__TEST__TRUE( result );
	CPPLIB__TEST__EQ( id, 3 );
	CPPLIB__TEST__LOOP_RESET();

	CPPLIB__TEST__EQ( ::lib::str( (FooBar)100 ), "<invalid>" );
	CPPLIB__TEST__EQ( ::lib::str( FooBar::BAR ), "bar" );

	CPPLIB__TEST__EQ( ::lib::unstr< FooBar >( "baz" ), FooBar::invalid );
	CPPLIB__TEST__EQ( ::lib::unstr< FooBar >( "Baz" ), FooBar::BAZ );
}

} // namespace test::lib::utils
