/* File: /test/lib/file/ini.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__file__ini__hpp
#define CPPLIB__test__lib__file__ini__hpp

#include <lib/test/unit.hpp>

namespace test::lib::file {

class Ini final
	: public ::lib::test::IUnit
{
public:
	Ini() noexcept : IUnit {"Ini"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::file

#endif // CPPLIB__test__lib__file__ini__hpp
