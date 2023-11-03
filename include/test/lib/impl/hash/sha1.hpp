/* File: /test/lib/impl/hash/sha1.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__impl__hash__sha1__hpp
#define CPPLIB__test__lib__impl__hash__sha1__hpp

#include <lib/test/unit.hpp>

namespace test::lib::hash::impl {

class Sha1 final
	: public ::lib::test::IUnit
{
public:
	Sha1() noexcept : IUnit {"Sha1"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::hash::impl

#endif // CPPLIB__test__lib__impl__hash__sha1__hpp
