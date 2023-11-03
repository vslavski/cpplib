/* File: /test/lib/flags.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__flags__hpp
#define CPPLIB__test__lib__flags__hpp

#include <lib/test/unit.hpp>

namespace test::lib {

class Flags final
	: public ::lib::test::IUnit
{
public:
	Flags() noexcept : IUnit {"Flags"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib

#endif // CPPLIB__test__lib__flags__hpp
