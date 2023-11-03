/* File: /test/lib/utils/enum.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__utils__enum__hpp
#define CPPLIB__test__lib__utils__enum__hpp

#include <lib/test/unit.hpp>

namespace test::lib::utils {

class Enum final
	: public ::lib::test::IUnit
{
public:
	Enum() noexcept : IUnit {"Enum"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::utils

#endif // CPPLIB__test__lib__utils__enum__hpp
