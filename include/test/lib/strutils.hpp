/* File: /test/lib/strutils.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__strutils__hpp
#define CPPLIB__test__lib__strutils__hpp

#include <lib/test/unit.hpp>

namespace test::lib {

class StrUtils final
	: public ::lib::test::IUnit
{
public:
	StrUtils() noexcept : IUnit {"StrUtils"} {}
private:
	void test_execute() noexcept override;

	void is_digit() noexcept;
	void is_integer() noexcept;
	void is_float() noexcept;
	void split() noexcept;
	void hex() noexcept;
};

} // namespace test::lib

#endif // CPPLIB__test__lib__strutils__hpp
