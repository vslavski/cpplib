/* File: /test/lib/bus.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__test__lib__bus__hpp
#define CPPLIB__test__lib__bus__hpp

#include <lib/test/unit.hpp>

#include <lib/tl/listener.hpp>

#include <lib/bus/manager.hpp>

namespace test::lib {

class Bus final
	: public ::lib::test::IUnit
	, public ::lib::tag_tl_listener< Bus >
	, ::lib::tag_bus< ::lib::bus::Manager<int> >
{
public:
	Bus() noexcept
		: IUnit {"Bus"}
		, tag_bus {manager}
	{}
private:
	void test_execute() noexcept override;
	void test_reset() noexcept override;

	struct Message1 : ::lib::bus::Message<int> {
		static constexpr int ID = 10;
		Message1() noexcept : Message<int>{ ID } {}
	};

	struct Message2 : ::lib::bus::Message<int> {
		static constexpr int ID = 20;
		Message2() noexcept : Message<int>{ ID } {}
	};

	void on_message1( const Message1 * message ) noexcept;
	void on_message2( const Message2 * message ) noexcept;
	void on_multimessage( const ::lib::bus::Message<int> & ) noexcept;

	::lib::bus::Manager<int> manager;
	int message1_count = 0;
	int message2_count = 0;
	int multimessage_count = 0;
};

} // namespace test::lib

#endif // CPPLIB__test__lib__bus__hpp
