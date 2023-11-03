/* File: /lib/impl_posix/socket/tcp/client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <poll.h>

#include <cerrno>

#include <system_error>

#include <cpp/lib_debug>
#include <cpp/lib_scope>

#include "../../../../lib/literals.hpp"
#include "../../../../lib/flags.hpp"

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
	address.sin_addr = { ::htonl( ip4_address ) };
	const auto result = ::connect( sock, (const struct sockaddr*) &address, sizeof(address) );
	if ( result == 0 )
		setState( State::CONNECTED );
	else if ( not is_inprogress( result ) )
		return false;

	return true;
}

bool client::update()/* override*/ {
	if ( isFailed() )
		return false;
	if ( not Super::update() )
		return false;
	if ( getState() == State::CONNECTING ) {
		const auto & result = read({});
		if ( result.failed() )
			return is_inprogress( result.error().value() );
		setState( State::CONNECTED );
	}

	struct ::pollfd poll_fd = { sock, 0, 0 };
	const auto & count = check_error( ::poll( &poll_fd, 1, 0 ) );
	if ( count != 1_sz )
		return count.success();
	const auto & revents = Flag( poll_fd.revents );
	if ( revents.check<POLLHUP>() )
		setState( State::DISCONNECTING );
	else
		CPP_ASSERT( false/*Unknown event*/ );
	return true;
}

bool client::shutdown()/* override*/ {
	data::result_t result;
	if ( getState() == State::CONNECTED ) {
		result = check_error( ::shutdown( sock, SHUT_WR ) );
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

	const auto code = (::std::errc) errno;
	if ( code == ::std::errc::broken_pipe or code == ::std::errc::connection_reset ) {
		setState( State::DISCONNECTING );
		return 0_sz;
	}
	if ( is_inprogress( result, false ) )
		return 0_sz;

	const auto & result_error = Super::check_error( result );
	setState( State::FAILED );
	return result_error;
}

} // namespace lib::socket::impl::tcp
