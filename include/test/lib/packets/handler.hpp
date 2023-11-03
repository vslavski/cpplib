/* File: /test/lib/packets/handler.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__packets__handler__hpp
#define CPPLIB__test__lib__packets__handler__hpp

#include <lib/test/unit.hpp>

namespace test::lib::packets {

class Handler final
	: public ::lib::test::IUnit
{
public:
	Handler() noexcept : IUnit {"Handler"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::packets

#endif // CPPLIB__test__lib__packets__handler__hpp
