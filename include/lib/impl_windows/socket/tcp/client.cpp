/* File: /lib/impl_windows/socket/tcp/client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <winerror.h>
#include <winsock2.h>

#include <cstring>

#include <cpp/lib_debug>
#include <cpp/lib_scope>

#include "../../../../lib/literals.hpp"
#include "../../../../lib/flags.hpp"

#include "../../../../lib/impl_windows/system/error.hpp"

#include "./client.hpp"

namespace lib::socket::impl::tcp {

// IMPLEMENTATION lib::socket::impl::tcp::client

client::client( u32 ip4_address, u16 port ) {
	if ( not connect( ip4_address, port ) )
		CPP_UNUSED( close() );
}

/*virtual */client::~client()
{ CPP_UNUSED( close() ); }

cstring client::getName() const/* override*/
{ return "client"; }

bool client::connect( u32 ip4_address, u16 port ) {
	CPP_ASSERT( not isFailed() );
	if ( isActive() )
		return false;

	setState( State::CONNECTING );

	if ( not is_open() and not open() )
		return false;

	struct ::sockaddr_in address = {};
	address.sin_family = AF_INET;
	address.sin_port = ::htons( port );
	const auto addr = ::htonl( ip4_address );
	static_assert( sizeof(addr) == sizeof(address.sin_addr) );
	::std::memcpy( &address.sin_addr, &addr, sizeof(address.sin_addr) );
	const auto & result = check_error(
		::connect( sock, (const struct sockaddr*) &address, sizeof(address) ) );
	if ( result.success() )
		setState( State::CONNECTED );
	else if ( result.error().value() != WSAEWOULDBLOCK )
		return false;

	return check_error( ::WSAEventSelect( sock, event, FD_CONNECT | FD_CLOSE ) ).success();
}

bool client::update()/* override*/ {
	if ( isFailed() )
		return false;
	if ( not Super::update() )
		return false;

	const auto state_ = getState();
	if ( state_ != State::CONNECTING and state_ != State::CONNECTED )
		return true;

	const auto result = ::WSAWaitForMultipleEvents( 1, &event, FALSE, 0, FALSE );
	if ( result == WSA_WAIT_FAILED ) {
		CPP_UNUSED( check_wsa_error() );
		return false;
	}
	if ( result != WSA_WAIT_EVENT_0 )
		return true;

	::WSANETWORKEVENTS net_events;
	if ( check_error( ::WSAEnumNetworkEvents( sock, event, &net_events ) ).failed() )
		return false;

	const auto & fdflags =  Flag {net_events.lNetworkEvents};
	if ( fdflags.check( FD_CONNECT ) ) {
		if ( check_wsa_error( net_events.iErrorCode[FD_CONNECT_BIT] ).failed() )
			return false;
		setState( State::CONNECTED );
	} else if ( fdflags.check( FD_CLOSE ) ) {
		if ( check_wsa_error( net_events.iErrorCode[FD_CLOSE_BIT] ).failed() )
			return false;
		setState( State::DISCONNECTING );
	} else {
		CPP_ASSERT( false/*Unknown event*/ );
	}

	return true;
}

bool client::shutdown()/* override*/ {
	data::result_t result;
	if ( getState() == State::CONNECTED ) {
		result = check_error( ::shutdown( sock, SD_SEND ) );
		if ( result.success() )
			setState( State::DISCONNECTING );
	}
	return Super::shutdown() and result.success();
}

bool client::close()/* override*/ {
	if ( not is_open() or close_lock )
		return true;

	close_lock = true;
	const auto lock = ::cpp::scope_exit{ [&]{ close_lock = false; } };

	if ( not Super::close() )
		return false;

	setState( State::DISCONNECTED );
	return true;
}

data::result_t client::check_error( isize result )/* override*/ {
	if ( result >= 0 )
		return (usize) result;

	const auto wsa_error = ::WSAGetLastError();
	if ( wsa_error == WSAESHUTDOWN or wsa_error == WSAECONNRESET ) {
		setState( State::DISCONNECTING );
		return 0_sz;
	}
	if ( wsa_error == WSAEWOULDBLOCK )
		return make_windows_error( wsa_error );

	const auto & result_error = check_wsa_error( wsa_error );
	setState( State::FAILED );
	return result_error;
}

} // namespace lib::socket::impl::tcp
