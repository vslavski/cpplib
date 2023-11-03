/* File: /test/lib/packets/handler.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/debug/features.hpp>

#include <string>
#include <vector>
CPPLIB_MSVC_WARNING(disable:4355)
CPPLIB_MSVC_WARNING(disable:5204)
#include <future>
CPPLIB_MSVC_WARNING(default:5204)
CPPLIB_MSVC_WARNING(default:4355)

#include <cpp/lib_debug>
#include <cpp/lib_scope>

#include <lib/packets/handler.hpp>
#include <lib/impl/packets/ping_counter.hpp>

#include <lib/tl/listener.hpp>
#include <lib/types.hpp>
#include <lib/literals.hpp>
#include <lib/ptr.hpp>
#include <lib/data/serialize.hpp>
#include <lib/impl/serialize/std_contiguous_container.hpp>
#include <lib/impl/serialize/ptr.hpp>
#include <lib/impl/serialize/std_tuple.hpp>

#include <lib/impl/socket/tcp.hpp>
#include <lib/impl/stream/buffer.hpp>

#include "./handler.hpp"

namespace test::lib::packets {

namespace {

struct PacketBase : ::lib::tag_serializeable {};

struct Message : PacketBase {
	static constexpr auto ID = 1;
	Message() = default;
	Message( const ::std::string & message, const ::std::string & title )
		: title{ title }, message{ message } {}
	::std::string title;
	::std::string message;
	::std::tuple<int, char> tuple;

	inline ::lib::data::result_t serialized_size( ::lib::data::wstream_t & stream ) const override
	{ return ::lib::data::serialized_size( stream, title, message ); }
	inline bool can_deserialize( ::lib::data::rstream_t & stream ) const override
	{ return ::lib::data::can_deserialize( stream, title, message ); }
	inline ::lib::data::result_t serialize( ::lib::data::wstream_t & stream ) const override
	{ return ::lib::data::serialize( stream, title, message ); }
	inline ::lib::data::result_t deserialize( ::lib::data::rstream_t & stream ) override
	{ return ::lib::data::deserialize( stream, title, message ); }
	using PacketBase::deserialize;
};

class PacketsHandler
	: public ::lib::packets::ReadHandler
	, public ::lib::tag_tl_listener< PacketsHandler >
{
public:
	PacketsHandler( ::lib::data::rwstream_t & stream_ )
		: stream{ 128 }
		, ping_counter{ 33, 42 }
	{
		listen( Message::ID, &message_packet, { *this, &PacketsHandler::onMessage } );
		stream.reset( &stream_ );
		reset( &stream );
		ping_counter.attach( *this );
		ping_counter.on_ping = { *this, &PacketsHandler::onPing };
	}
	~PacketsHandler() {
		ping_counter.detach();
		reset();
		stream.reset();
	}
	bool update() { return stream.read_flush() and receive() and stream.write_flush(); }

	::lib::u32 ping_count = 0;
	::std::vector< ::std::string > messages;
private:
	void onPing()
		{ ++ping_count; }
	bool onMessage( const ::lib::tag_serializeable & message_ ) {
		const auto & message = static_cast<const Message&>( message_ );
		CPP_ASSERT( &message_packet == &message );
		messages.push_back( message.message );
		return message.title == "client" or message.title == "server";
	}

	::lib::stream::impl::buffer stream;
	Message message_packet;
	::lib::packets::impl::PingCounter ping_counter;
};

} // namespace

void Handler::test_execute() noexcept/* override*/ {
	static constexpr auto sleep = []( auto ms ) { ::std::this_thread::sleep_for( ms ); };
	using namespace ::std::literals::chrono_literals;

	constexpr ::lib::u32 SOCKET_ADDR = 0x7F000001;
	constexpr ::lib::u16 SOCKET_PORT = 32002;
	constexpr auto MSG_CLIENT_TO_SERVER = "This message has been sent from client to server.";
	constexpr auto MSG_SERVER_TO_CLIENT = "This message has been sent from server to client.";

	auto client_future = ::std::async( ::std::launch::async, [&, this]() {
		// Give server some time to get ready.
		sleep( 10ms );

		::lib::socket::impl::tcp::client client;

		const auto error_message = ::cpp::scope_exit {[&]() {
			test_error( client.error() );
		}};

		CPPLIB__TEST__TRUE( client.connect( SOCKET_ADDR, SOCKET_PORT ) );

		PacketsHandler handler( client );
		CPPLIB__TEST__TRUE( client.update() );
		auto wait_ping_count = handler.ping_count + 2;

		while ( wait_ping_count >= handler.ping_count ) {
			CPPLIB__TEST__TRUE( client.update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}
		wait_ping_count = handler.ping_count + 2;

		CPPLIB__TEST__TRUE( handler.send( Message{ MSG_CLIENT_TO_SERVER, "client" } ) );

		while ( wait_ping_count >= handler.ping_count ) {
			CPPLIB__TEST__TRUE( client.update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}
		wait_ping_count = handler.ping_count;

		CPPLIB__TEST__EQ( handler.messages.size(), 1 );
		CPPLIB__TEST__EQ( handler.messages.front(), MSG_SERVER_TO_CLIENT );

		while ( wait_ping_count == handler.ping_count ) {
			CPPLIB__TEST__TRUE( client.update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}
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
	::lib::socket::impl::tcp::server server( SOCKET_ADDR, SOCKET_PORT );

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

		PacketsHandler handler( *client.obj );
		CPPLIB__TEST__TRUE( server.update() or not client.obj->update() );
		auto wait_ping_count = handler.ping_count + 2;

		while ( wait_ping_count >= handler.ping_count ) {
			CPPLIB__TEST__TRUE( server.update() );
			CPPLIB__TEST__TRUE( client.obj->update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}
		CPPLIB__TEST__LOOP_RESET();
		wait_ping_count = handler.ping_count + 2;

		CPPLIB__TEST__TRUE( handler.send( Message{ MSG_SERVER_TO_CLIENT, "server" } ) );

		while ( wait_ping_count >= handler.ping_count ) {
			CPPLIB__TEST__TRUE( server.update() );
			CPPLIB__TEST__TRUE( client.obj->update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}
		wait_ping_count = handler.ping_count;

		CPPLIB__TEST__EQ( handler.messages.size(), 1 );
		CPPLIB__TEST__EQ( handler.messages.front(), MSG_CLIENT_TO_SERVER );

		while ( wait_ping_count == handler.ping_count ) {
			CPPLIB__TEST__TRUE( server.update() );
			CPPLIB__TEST__TRUE( client.obj->update() );
			CPPLIB__TEST__TRUE( handler.update() );
			sleep( 5ms );
		}

		client.obj.reset();

		for ( auto i = 0; i < 10; ++i ) {
			CPPLIB__TEST__LOOP_NEXT();
			CPPLIB__TEST__TRUE( server.update() );
			sleep( 10ms );
		}
		CPPLIB__TEST__LOOP_RESET();

		server_failed = false;
	}();
	CPPLIB__TEST__FALSE( server_failed );

	CPPLIB__TEST__TRUE( not client.obj or client.obj->close() );
	CPPLIB__TEST__TRUE( server.close() );
}

} // namespace test::lib::packets
