/* File: /lib/test/unit.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./unit.hpp"

namespace lib::test {

const Result & IUnit::test_run() noexcept {
	test_reset();
	test_execute();
	return result_;
}

void IUnit::test_set_failed( int line ) noexcept {
	if ( not result_ )
		return;
	result_.line = line;
}

} // namespace lib::test
