/* File: /test/lib/impl_posix/socket/unix.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__impl_posix__socket__unix__hpp
#define CPPLIB__test__lib__impl_posix__socket__unix__hpp

/// @todo Unify with "tcp"?

#include <lib/test/unit.hpp>

namespace test::lib::socket::impl {

class Unix final
	: public ::lib::test::IUnit
{
public:
	Unix() noexcept : IUnit {"Unix"} {}
private:
	void test_execute() noexcept override;
};

} // namespace test::lib::socket::impl

#endif // CPPLIB__test__lib__impl_posix__socket__unix__hpp
