/* File: /test/lib/strutils.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <string>

#include <lib/utils/string.hpp>

#include "./strutils.hpp"

namespace test::lib {

void StrUtils::test_execute() noexcept/* override*/ {
	CPPLIB__TEST__SUBTEST( is_digit );
	CPPLIB__TEST__SUBTEST( is_integer );
	CPPLIB__TEST__SUBTEST( is_float );
	CPPLIB__TEST__SUBTEST( split );
	CPPLIB__TEST__SUBTEST( hex );
}

void StrUtils::is_digit() noexcept {
	static constexpr const char * bad_digit[] = {
		"", "a", "+", "-", " ", "aa", " 1", "2 ", "+2", "-2", " 3 "
	};
	for ( const auto * str : bad_digit ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__FALSE( ::lib::is_digit( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();

	static constexpr const char * good_digit[] = {
		"1", "23", "456"
	};
	for ( const auto * str : good_digit ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__TRUE( ::lib::is_digit( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();
}

void StrUtils::is_integer() noexcept {
	static constexpr const char * bad_integer[] = {
		"", "a", "+", "-", " ", "aa", " 1", "2 "
	};
	for ( const auto * str : bad_integer ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__FALSE( ::lib::is_integer( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();

	static constexpr const char * good_integer[] = {
		"+2", "-2", "33", "+44", "-56", "+789"
	};
	for ( const auto * str : good_integer ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__TRUE( ::lib::is_integer( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();
}

void StrUtils::is_float() noexcept {
	static constexpr const char * bad_float[] = {
		"", "e", "E", "+", "-", " ", "aa", " 1", "2 ", "+2",
		"-2", "33", "+44", "-56", "+789", ".", "..", "...", ".1.", "1.1.",
		".1.1", "1..1", "e", "E", "1e", "e2", "2e.1", "2.E1", "1.+2", "2.-3"
	};
	for ( const auto * str : bad_float ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__FALSE( ::lib::is_float( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();

	static constexpr const char * good_float[] = {
		"1.", ".2", "-1.", "-.2", "1.2", "-3.45", "1e2", "-3e4", "5e+6",
		"123.", ".234", "-12.", "-.23", "12.34", "-34.56", "12e21", "-31e14", "15e+16"
	};
	for ( const auto * str : good_float ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__TRUE( ::lib::is_float( str ) );
	}
	CPPLIB__TEST__LOOP_RESET();
}

void StrUtils::split() noexcept {
	CPPLIB__TEST__TRUE( ::lib::split( "", " ", "'" ).empty() );
	CPPLIB__TEST__TRUE( ::lib::split( " ", " ", "'" ).empty() );
	CPPLIB__TEST__TRUE( ::lib::split( "  ", " ", "'" ).empty() );

	auto v = ::lib::split( "'", " ", "'" );
	CPPLIB__TEST__EQ( v.size(), 1 );
	CPPLIB__TEST__EQ( v[0], "'" );

	v = ::lib::split( " '' ", " ", "'" );
	CPPLIB__TEST__EQ( v.size(), 1 );
	CPPLIB__TEST__TRUE( v[0].empty() );

	v = ::lib::split( " 1st 2nd \"t 'h ' r 'd\" 4 'f\"iv''six", " ", "\"'" );
	CPPLIB__TEST__EQ( v.size(), 6 );
	CPPLIB__TEST__EQ( v[0], "1st" );
	CPPLIB__TEST__EQ( v[1], "2nd" );
	CPPLIB__TEST__EQ( v[2], "t 'h ' r 'd" );
	CPPLIB__TEST__EQ( v[3], "4" );
	CPPLIB__TEST__EQ( v[4], "f\"iv" );
	CPPLIB__TEST__EQ( v[5], "six" );
}

void StrUtils::hex() noexcept {
	const ::std::string hex{ "\x01\x23\x45\x67\x89\xAB\xCD\xEF" };
	CPPLIB__TEST__EQ( ::lib::hex( hex, "--" ), "01--23--45--67--89--ab--cd--ef" );
}

} // namespace test::lib
