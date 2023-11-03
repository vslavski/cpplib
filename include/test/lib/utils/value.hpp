/* File: /test/lib/utils/value.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__utils__value__hpp
#define CPPLIB__test__lib__utils__value__hpp

#include <lib/test/unit.hpp>

namespace test::lib::utils {

class Value final
	: public ::lib::test::IUnit
{
public:
	Value() noexcept : IUnit {"Value"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::utils

#endif // CPPLIB__test__lib__utils__value__hpp
