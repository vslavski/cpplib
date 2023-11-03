/* File: /lib/impl_windows/socket/tcp/server_client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <winerror.h>
#include <winsock2.h>

#include <cerrno>

#include <cpp/lib_debug>
#include <cpp/lib_scope>

#include "../../../../lib/literals.hpp"

#include "./server_client.hpp"

namespace lib::socket::impl::tcp {

// IMPLEMENTATION lib::socket::impl::tcp::server::client

server::client::client( SOCKET sock )
    : base{ sock }
{
	CPP_UNUSED( check_error( ::WSAEventSelect( sock, event, FD_CLOSE ) ) );
	const bool result = set_blocking( false );
	CPP_UNUSED( result );
	CPP_ASSERT( result );
}

/*virtual */server::client::~client()
{ CPP_UNUSED( close() ); }

cstring server::client::getName() const/* override*/
{ return "server::client"; }

bool server::client::update()/* override*/ {
	if ( isFailed() )
		return false;
	if ( not Super::update() )
		return false;
	return true;
}

bool server::client::shutdown()/* override*/ {
	data::result_t result;
	if ( getState() == State::CONNECTED ) {
		result = check_error( ::shutdown( sock, SD_SEND ) );
		if ( result.success() )
			setState( State::CLOSING );
	}
	return Super::shutdown() and result.success();
}

bool server::client::close()/* override*/ {
	if ( not is_open() or close_lock )
		return true;

	close_lock = true;
	const auto lock = ::cpp::scope_exit{ [&]{ close_lock = false; } };

	if ( not Super::close() )
		return false;

	setState( State::CLOSED );
	return true;
}

data::result_t server::client::check_error( isize result )/* override*/ {
	if ( result >= 0 )
		return (usize) result;

	const auto wsa_error = ::WSAGetLastError();
	if ( wsa_error == WSAESHUTDOWN ) {
		setState( State::CLOSING );
		return 0_sz;
	}

	const auto & result_error = check_wsa_error( wsa_error );
	setState( State::FAILED );
	return result_error;
}

} // namespace lib::socket::impl::tcp
