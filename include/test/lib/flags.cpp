/* File: /test/lib/flags.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/types.hpp>

#include <lib/flags.hpp>

#include "./flags.hpp"

namespace test::lib {

void Flags::test_execute() noexcept/* override*/ {
	using ::lib::Flag;

	enum Foo : ::lib::u8 {
		ZERO,
		ONE,
		TWO,
		ONE_AND_TWO,
		FOUR,
	} foo = ZERO;
	CPPLIB__TEST__FALSE( Flag{ foo }.check( ONE ) );
	CPPLIB__TEST__NE( Flag{ foo }.add( ONE ).flag, ZERO );
	CPPLIB__TEST__TRUE( Flag{ foo }.add( TWO ).check( ONE_AND_TWO ) );
	foo = FOUR;
	CPPLIB__TEST__NE( Flag{ foo }.flag, ZERO );

	enum class Bar {
		ZERO,
		ONE,
		TWO,
		ONE_AND_TWO,
		FOUR,
	} bar = Bar::ZERO;
	CPPLIB__TEST__FALSE( Flag{ bar }.check( Bar::ONE ) );
	CPPLIB__TEST__EQ( Flag{ bar }.add( 1 ).flag, Bar::ONE );
	CPPLIB__TEST__TRUE( Flag( bar ).add( Bar::TWO ).check( 3 ) );

	constexpr uint64_t INT_RANGE_VALUE = 0x80/*00*/;
	CPPLIB__TEST__TRUE( Flag{ foo }.add<INT_RANGE_VALUE>().check( INT_RANGE_VALUE ) );
}

} // namespace test::lib
