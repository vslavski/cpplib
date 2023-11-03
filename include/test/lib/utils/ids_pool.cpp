/* File: /test/lib/utils/ids_pool.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/types.hpp>
#include <lib/utils/ids_pool.hpp>

#include "./ids_pool.hpp"

namespace test::lib::utils {

void IdsPool::test_execute() noexcept/* override*/ {
	using ::lib::utils::ids_pool;

	ids_pool<::lib::u8> pool;

	CPPLIB__TEST__EQ( pool.retain(), 1 );
	CPPLIB__TEST__EQ( pool.retain(), 2 );
	CPPLIB__TEST__EQ( pool.retain(), 3 );
	CPPLIB__TEST__TRUE( pool.contains( 3 ) );
	CPPLIB__TEST__FALSE( pool.contains( 4 ) );
	CPPLIB__TEST__TRUE( pool.release( 2 ) );
	CPPLIB__TEST__FALSE( pool.release( 2 ) );
	CPPLIB__TEST__EQ( pool.retain(), 2 );
}

} // namespace test::lib::utils
