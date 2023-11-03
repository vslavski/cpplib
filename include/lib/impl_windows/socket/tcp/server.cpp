/* File: /lib/impl_windows/socket/tcp/server.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <winerror.h>
#include <winsock2.h>

#include <cstring>

#include <cpp/lib_algorithm>
#include <cpp/lib_debug>
#include <cpp/lib_scope>

#include "../../../../lib/literals.hpp"
#include "../../../../lib/flags.hpp"

#include "./server_client.hpp"

#include "./server.hpp"

namespace lib::socket::impl::tcp {

// IMPLEMENTATION lib::socket::impl::tcp::server

server::server( u32 ip4_address, u16 port ) {
	if ( not bind( ip4_address, port ) )
		CPP_UNUSED( close() );
}

/*virtual */server::~server()
{ CPP_UNUSED( close() ); }

cstring server::getName() const/* override*/
{ return "server"; }

bool server::bind( u32 ip4_address, u16 port ) {
	CPP_ASSERT( not isFailed() );
	if ( isActive() )
		return false;

	setState( State::BINDING );

	if ( not is_open() and not open() )
		return false;

	struct ::sockaddr_in address = {};
	address.sin_family = AF_INET;
	address.sin_port = ::htons( port );
	const auto addr = ::htonl( ip4_address );
	static_assert( sizeof(addr) == sizeof(address.sin_addr) );
	::std::memcpy( &address.sin_addr, &addr, sizeof(address.sin_addr) );
	const auto & result = check_error(
		::bind( sock, (const struct sockaddr*) &address, sizeof(address) ) );
	if ( result.failed() )
		return false;

	setState( State::BOUND );
	return true;
}

bool server::listen() {
	CPP_ASSERT( not isFailed() );
	if ( getState() != State::BOUND )
		return false;
	CPP_ASSERT( clients.empty() );

	const auto & result = check_error( ::listen( sock, MAX_CONNECTIONS ) );
	if ( result.failed() )
		return false;

	if ( check_error( ::WSAEventSelect( sock, event, FD_ACCEPT ) ).failed() )
		return false;

	setState( State::LISTENING );
	return true;
}

SPtr<server::client> server::accept() {
	CPP_ASSERT( not isFailed() );
	if ( getState() != State::LISTENING )
		return {};

	const auto client_sock = check_sock_error( ::accept( sock, nullptr, nullptr ) );
	if ( client_sock == INVALID_SOCKET )
		return {};

	const auto & client_ = MkSPtr<client>( client_sock );
	clients.emplace_back( client_ );
	client_events.emplace_back( client_->get_event() );
	CPP_ASSERT( client_events.back() != WSA_INVALID_EVENT );
	client_->on_state_changed = { *this, &server::onClientState/*Changed*/ };
	onClientStateChanged( client_ );
	return client_;
}

bool server::reject() {
	CPP_ASSERT( not isFailed() );
	if ( getState() != State::LISTENING )
		return false;

	const auto client_sock = check_sock_error( ::accept( sock, nullptr, nullptr ) );
	if ( client_sock == INVALID_SOCKET )
		return false;

	const auto result = ::closesocket( client_sock );
	CPP_ASSERT( result == 0 );
	return result == 0;
}

bool server::update()/* override*/ {
	if ( isFailed() )
		return false;
	if ( getState() != State::LISTENING )
		return true;

	if ( not Super::update() )
		return false;

	updateFlushClients();
	if ( not updateServer() or not updateClients() )
		return false;

	return true;
}

void server::updateFlushClients() {
	for ( auto idx = 0_sz; idx < clients.size(); ) {
		auto & client_ = *clients[idx];

		if ( client_.isActive() ) {
			++idx;
			continue;
		}

		CPP_UNUSED( client_.close() );
		client_events.erase( client_events.begin() + (isize) idx );
		clients.erase( clients.begin() + (isize) idx );
	}
}

bool server::updateServer() {
	for ( ;; ) {
		const auto result = ::WSAWaitForMultipleEvents( 1, &event, FALSE, 0, FALSE );
		if ( result == WSA_WAIT_FAILED ) {
			CPP_UNUSED( check_wsa_error() );
			return false;
		}
		if ( result != WSA_WAIT_EVENT_0 )
			break;

		::WSANETWORKEVENTS net_events;
		if ( check_error( ::WSAEnumNetworkEvents( sock, event, &net_events ) ).failed() )
			return false;

		const auto & fdflags =  Flag {net_events.lNetworkEvents};
		if ( fdflags.check( FD_ACCEPT ) ) {
			if ( check_wsa_error( net_events.iErrorCode[FD_ACCEPT_BIT] ).failed() )
				return false;
		} else {
			CPP_ASSERT( false/*Unknown event*/ );
		}

		auto action = NewClientAction::AUTO;
		onNewClient( action );
		switch ( action ) {
		case NewClientAction::NONE:		break;
		case NewClientAction::ACCEPT:	CPP_UNUSED( accept() ); break;
		case NewClientAction::REJECT:
		case NewClientAction::AUTO:		CPP_UNUSED( reject() ); break;
		};
		if ( isFailed() )
			return false;
		break;
	}
	return true;
}

bool server::updateClients() {
	if ( clients.empty() )
		return true;

	for ( ;; ) {
		const auto result = ::WSAWaitForMultipleEvents( (DWORD) client_events.size(), client_events.data(), FALSE, 0, FALSE );
		if ( result == WSA_WAIT_FAILED ) {
			CPP_UNUSED( check_wsa_error() );
			return false;
		}
		if ( not ::cpp::is_in_range<usize>( result, WSA_WAIT_EVENT_0, WSA_WAIT_EVENT_0 + client_events.size() - 1 ) )
			break;

		const auto client_idx = result - WSA_WAIT_EVENT_0;
		auto & client_ = *clients[client_idx];

		::WSANETWORKEVENTS net_events;
		if ( ::WSAEnumNetworkEvents( client_.get_sock(), client_.get_event(), &net_events ) == SOCKET_ERROR )
			continue;

		const auto & fdflags = Flag {net_events.lNetworkEvents};
		if ( fdflags.check( FD_CLOSE ) ) {
			if ( net_events.iErrorCode[FD_CLOSE_BIT] != 0 )
				continue;
			CPP_UNUSED( client_.shutdown() );
		}
	}

	return true;
}

bool server::shutdown()/* override*/ {
	if ( getState() == State::LISTENING ) {
		setState( State::CLOSING );
		for ( auto & client_ : clients )
			CPP_UNUSED( client_->shutdown() );
	}
	return Super::shutdown();
}

bool server::close()/* override*/ {
	if ( not is_open() or close_lock )
		return true;

	close_lock = true;
	const auto lock = ::cpp::scope_exit{ [&]{ close_lock = false; } };

	for ( auto & client_ : clients )
		CPP_UNUSED( client_->close() );
	client_events.clear();
	clients.clear();

	if ( not Super::close() )
		return false;

	setState( State::CLOSED );
	return true;
}

data::result_t server::check_error( isize result )/* override*/ {
	if ( result >= 0 )
		return (usize) result;

	const auto wsa_error = ::WSAGetLastError();
	if ( wsa_error == WSAENOTCONN )
		return 0_sz;

	const auto & result_error = check_wsa_error( wsa_error );
	setState( State::FAILED );
	return result_error;
}

void server::onClientState/*Changed*/( const ::lib::socket::server::client & client_ ) {
	for ( const auto & c : clients )
		if ( c.get() == &client_ )
			onClientStateChanged( c );
}

} // namespace lib::socket::impl::tcp
