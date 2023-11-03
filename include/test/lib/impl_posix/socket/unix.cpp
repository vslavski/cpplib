/* File: /test/lib/impl_posix/socket/unix.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <stdlib.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

#include <future>
#include <string>

#include <cpp/lib_scope>

#include <lib/data/serialize.hpp>
#include <lib/impl/serialize/std_contiguous_container.hpp>

#include <lib/impl_posix/socket/unix.hpp>

#include "./unix.hpp"

namespace test::lib::socket::impl {

void Unix::test_execute() noexcept/* override*/ {
	using namespace ::std::literals::chrono_literals;
	static constexpr auto sleep = []( auto ms ) { ::std::this_thread::sleep_for( ms ); };

	char socket_name[] = "/tmp/CPPLIB__test__lib__impl_posix__socket__unix__XXXXXX";
	{
		int socket_file = ::mkstemp( socket_name );
		CPPLIB__TEST__NE( socket_file, -1 );
		/// @todo Close file after opened by server?
		CPPLIB__TEST__EQ( ::close( socket_file ), 0 );
		CPPLIB__TEST__EQ( ::unlink( socket_name ), 0 );
	}

	const ::std::wstring	FOO( L"server -> client" );
	constexpr char			BAR[] = "client -> server";
	constexpr int			CLIENT_STOP = 313373;
	::std::atomic<bool>		emergency_client_stop = false;

	auto client_future = ::std::async( ::std::launch::async, [&, this]() {
		// Give server some time to get ready.
		sleep( 10ms );

		::lib::socket::impl::unix::client client;

		const auto error_message = ::cpp::scope_exit {[&]() {
			test_error( client.error() );
		}};

		CPPLIB__TEST__TRUE( client.update() );
		CPPLIB__TEST__TRUE( client.connect( socket_name ) );
		CPPLIB__TEST__TRUE( client.update() );

		auto count = client.write({ BAR, ::std::strlen( BAR ) + 1 });
		CPPLIB__TEST__EQ( count, sizeof(BAR) );
		CPPLIB__TEST__TRUE( client.update() );

		::std::wstring foo;
		while ( not emergency_client_stop
		and client.update()
		and not ::lib::data::can_deserialize( client, foo ) )
			sleep( 10ms );
		CPPLIB__TEST__TRUE( client.update() );
		auto deserialize_result = ::lib::data::deserialize( client, foo );
		CPPLIB__TEST__EQ( deserialize_result, ::lib::data::serialized_size( client, foo ) );
		CPPLIB__TEST__EQ( foo, FOO );

		CPPLIB__TEST__TRUE( client.update() );
		while ( not emergency_client_stop
		and client.update()
		and not ::lib::data::can_deserialize( client, CLIENT_STOP ) )
			sleep( 100ms );
		CPPLIB__TEST__TRUE( client.update() );
		int client_stop;
		deserialize_result = ::lib::data::deserialize( client, client_stop  );
		CPPLIB__TEST__EQ( deserialize_result, ::lib::data::serialized_size( client, client_stop ) );
		CPPLIB__TEST__EQ( client_stop, CLIENT_STOP );
		CPPLIB__TEST__TRUE( client.update() );
		CPPLIB__TEST__TRUE( client.close() );
		CPPLIB__TEST__TRUE( client.update() );
	} ); // client_future

	struct Client : ::lib::tag_tl_listener< Client > {
		using SockSrv = ::lib::socket::server;
		void onNew( const SockSrv&, SockSrv::NewClientAction & action ) {
			action = SockSrv::NewClientAction::ACCEPT;
		}
		void onStateChanged( const SockSrv&, const ::lib::SPtr<SockSrv::client> & client_ ) {
			obj = client_;
		}
		::lib::SPtr<::lib::socket::socket> obj;
	} client;
	::lib::socket::impl::unix::server server( socket_name );

	const auto error_message = ::cpp::scope_exit {[&]() {
		test_error( server.error() );
		if ( client.obj )
			test_error( client.obj->error() );
	}};

	server.on_new_client = { client, &Client::onNew };
	server.on_client_state_changed = { client, &Client::onStateChanged };

	bool server_failed = true;
	[&, this](){
		CPPLIB__TEST__EQ( server.getState(), ::lib::socket::server::State::BOUND );
		CPPLIB__TEST__TRUE( server.listen() );

		for ( auto i = 0; i < 10; ++i ) {
			CPPLIB__TEST__LOOP_NEXT();
			CPPLIB__TEST__TRUE( server.update() );
			sleep( 10ms );
		}
		CPPLIB__TEST__LOOP_RESET();

		CPPLIB__TEST__TRUE( client.obj );
		CPPLIB__TEST__TRUE( server.update() );

		auto serialize_result = ::lib::data::serialize( *client.obj, FOO );
		CPPLIB__TEST__EQ( serialize_result, ::lib::data::serialized_size( *client.obj, FOO ) );

		::std::string bar( sizeof(BAR) - 1, '\0' );
		{
			::lib::usize count = 0;
			while ( count < bar.size() ) {
				CPPLIB__TEST__LOOP_NEXT();
				const auto & peek_result = client.obj->peek( bar );
				CPPLIB__TEST__TRUE( peek_result.success() );
				count = peek_result.value();
				CPPLIB__TEST__EQ( ::std::strstr( BAR, bar.c_str() ), BAR );
				sleep( 10ms );
			}
			CPPLIB__TEST__LOOP_RESET();
		}
		CPPLIB__TEST__EQ( bar, BAR );

		const auto & read_size = client.obj->read_size();
		CPPLIB__TEST__TRUE( read_size.success() );
		CPPLIB__TEST__GE( read_size, bar.size() );
		CPPLIB__TEST__EQ( client.obj->read( bar ), bar.size() );

		CPPLIB__TEST__EQ( BAR, bar );

		serialize_result = ::lib::data::serialize( *client.obj, CLIENT_STOP );
		CPPLIB__TEST__EQ( serialize_result, ::lib::data::serialized_size( *client.obj, CLIENT_STOP ) );
		CPPLIB__TEST__TRUE( client.obj->shutdown() );
		CPPLIB__TEST__FALSE( client.obj->is_open() );
		client.obj.reset();

		CPPLIB__TEST__TRUE( server.shutdown() );
		CPPLIB__TEST__FALSE( server.is_open() );

		server_failed = false;
	}();

	emergency_client_stop = true;
	CPPLIB__TEST__FALSE( server_failed );

	CPPLIB__TEST__TRUE( not client.obj or client.obj->close() );
	CPPLIB__TEST__TRUE( server.close() );
}

} // namespace test::lib::socket::impl
