/* File: /test/lib/impl/codec/base64.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__impl__codec__base64__hpp
#define CPPLIB__test__lib__impl__codec__base64__hpp

#include <lib/test/unit.hpp>

namespace test::lib::codec::impl {

class Base64 final
	: public ::lib::test::IUnit
{
public:
	Base64() noexcept : IUnit {"Base64"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::codec::impl

#endif // CPPLIB__test__lib__impl__codec__base64__hpp
