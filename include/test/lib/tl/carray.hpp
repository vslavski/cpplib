/* File: /test/lib/tl/carray.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__tl__carray__hpp
#define CPPLIB__test__lib__tl__carray__hpp

#include <lib/test/unit.hpp>

namespace test::lib::tl {

class CArray final
	: public ::lib::test::IUnit
{
public:
	CArray() noexcept : IUnit {"CArray"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::tl

#endif // CPPLIB__test__lib__tl__carray__hpp
