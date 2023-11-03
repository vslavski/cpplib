/* File: /test/lib/tl/carray.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <tuple>

#include <lib/tl/carray.hpp>

#include "./carray.hpp"

namespace test::lib::tl {

namespace {

struct carray_Foo {
	static constexpr auto DATA_SIZE = 5;
	constexpr carray_Foo() = default;
	char pad1__[7] = {};
	unsigned count = 0;
	char pad2__[5] = {};
	int data_i[DATA_SIZE] = {};
	char pad3__[11] = {};
	char data_c[DATA_SIZE] = {};
};

} //namespace

void CArray::test_execute() noexcept/* override*/ {
	using ::lib::tl::carray;

	carray_Foo foo;

	/// @note Use -O3 to check for operations overhead
	/// @todo Check element ctor's/dtor's.

	auto && a = carray( foo.count, foo.data_i, foo.data_c );
	CPPLIB__TEST__EQ( a.max_size(), foo.DATA_SIZE );

	const auto offset = a.size();
	a.emplace_back( 123, 'A' );
	a.emplace_back( 456, 'B' );
	a.emplace_back( 789, 'C' );
	CPPLIB__TEST__EQ( a.size(), offset + 3 );
	CPPLIB__TEST__EQ( a[offset + 0], (::std::tuple{ 123, 'A' }) );
	CPPLIB__TEST__EQ( a[offset + 1], (::std::tuple{ 456, 'B' }) );
	CPPLIB__TEST__EQ( a[offset + 2], (::std::tuple{ 789, 'C' }) );

	a = { { 123, 'A' }, { 456, 'B' }, { 789, 'C' } };
	CPPLIB__TEST__EQ( a.size(), 3 );
	CPPLIB__TEST__EQ( ::std::get<0>( a[0] ), 123 );
	CPPLIB__TEST__EQ( ::std::get<1>( a[1] ), 'B' );
	CPPLIB__TEST__EQ( ::std::get<1>( a[0] ), 'A' );

	CPPLIB__TEST__EQ( foo.data_i[1], 456 );
	CPPLIB__TEST__EQ( foo.data_c[2], 'C' );
	CPPLIB__TEST__EQ( foo.count, 3 );

	a.clear();
	CPPLIB__TEST__TRUE( a.empty() );

	// check init by arrays
//	a = { { -5, +7, -9 }, { 'Q', 'W', 'E' } };

/*
	a.emplace_back( 123, 'A' );
	a.emplace_back( 456, 'B' );
	a.emplace_back( 789, 'C' );
	CPPLIB__TEST__EQ( a.size(), 3 );
	CPPLIB__TEST__EQ( a[1], { 456, 'B' } );
*/
}

} // namespace test::lib::tl
