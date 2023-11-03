/* File: /test/lib/ecs.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__ecs__hpp
#define CPPLIB__test__lib__ecs__hpp

#include <lib/test/unit.hpp>

namespace test::lib {

class Ecs final
	: public ::lib::test::IUnit
{
public:
	Ecs() noexcept : IUnit {"Ecs"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib

#endif // CPPLIB__test__lib__ecs__hpp
