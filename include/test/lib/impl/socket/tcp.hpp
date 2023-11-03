/* File: /test/lib/impl/socket/tcp.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__impl__socket__tcp__hpp
#define CPPLIB__test__lib__impl__socket__tcp__hpp

/// @todo Unify with "unix"?

#include <lib/test/unit.hpp>

namespace test::lib::socket::impl {

class Tcp final
	: public ::lib::test::IUnit
{
public:
	Tcp() noexcept : IUnit {"Tcp"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::socket::impl

#endif // CPPLIB__test__lib__impl__socket__tcp__hpp
