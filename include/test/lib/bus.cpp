/* File: /test/lib/bus.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cpp/lib_debug>

#include "./bus.hpp"

namespace test::lib {

void Bus::on_message1( const Message1 * message ) noexcept {
	CPP_ASSERT( message->id == Message1::ID );
	++message1_count;
}

void Bus::on_message2( const Message2 * message ) noexcept {
	CPP_ASSERT( message->id == Message2::ID );
	++message2_count;
}

void Bus::on_multimessage( const ::lib::bus::Message<int> & ) noexcept {
	++multimessage_count;
}

void Bus::test_execute() noexcept/* override*/ {
	bus.listen_sync( *this, &Bus::on_message1 );
	CPPLIB__TEST__EQ( message1_count, 0 );

	bus.notify_sync<Message1>( *this );
	CPPLIB__TEST__EQ( message1_count, 1 );
	bus.notify_sync<Message1>( *this );
	CPPLIB__TEST__EQ( message1_count, 2 );

	bus.unlisten_sync( *this, &Bus::on_message1 );
	bus.notify_sync<Message1>( *this );
	CPPLIB__TEST__EQ( message1_count, 2 );

	auto filter = ::lib::MkSPtr< ::lib::bus::MessageFilter<int> >( true );
	bus.listen_sync( filter, *this, &Bus::on_message2 );
	CPPLIB__TEST__EQ( message2_count, 0 );

	bus.notify_sync<Message2>( *this );
	bus.notify_sync<Message2>( *this );
	bus.notify_sync<Message2>( *this );
	CPPLIB__TEST__EQ( message2_count, 1 );

	auto multifilter = ::lib::MkSPtr< ::lib::bus::MultiFilter<int> >( false, 10, 20 );
	bus.listen_sync( multifilter, *this, &Bus::on_multimessage );
	bus.listen_sync( *this, &Bus::on_message1, &Bus::on_message2 );
	CPPLIB__TEST__EQ( multimessage_count, 0 );

	bus.notify_sync<Message1>( *this );
	bus.notify_sync<Message2>( *this );
	bus.notify_sync<Message1>( *this );

	CPPLIB__TEST__EQ( message1_count, 4 );
	CPPLIB__TEST__EQ( message2_count, 2 );
	CPPLIB__TEST__EQ( multimessage_count, 3 );
};

void Bus::test_reset() noexcept/* override*/ {
	message1_count = 0;
	message2_count = 0;
	multimessage_count = 0;
}

} // namespace test::lib
