/* File: /lib/impl_posix/socket/tcp/server.cpp
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
#include <unistd.h>

#include <cerrno>

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
	address.sin_addr = { ::htonl( ip4_address ) };
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

	poll_fd = { sock, POLLIN, 0 };
	setState( State::LISTENING );
	return true;
}

SPtr<server::client> server::accept() {
	CPP_ASSERT( not isFailed() );
	if ( getState() != State::LISTENING )
		return {};

	const auto & result = check_error( ::accept( sock, nullptr, nullptr ) );
	if ( result.failed() )
		return {};

	const auto client_sock = (int) result.value();
	const auto & client_ = MkSPtr<client>( client_sock );
	clients.emplace_back( client_ );
	client_pollfds.emplace_back( ::pollfd{ client_sock, POLLIN, 0 } );
	client_->on_state_changed = { *this, &server::onClientState/*Changed*/ };
	onClientStateChanged( client_ );
	return client_;
}

bool server::reject() {
	CPP_ASSERT( not isFailed() );
	if ( getState() != State::LISTENING )
		return false;

	const auto & result = check_error( ::accept( sock, nullptr, nullptr ) );
	if ( result.failed() )
		return false;

	const auto client_sock = (int) result.value();
	const auto close_result = ::close( client_sock );
	CPP_ASSERT( close_result == 0 );
	/// @todo Always return true?
	return close_result == 0;
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
		client_pollfds.erase( client_pollfds.begin() + (isize) idx );
		clients.erase( clients.begin() + (isize) idx );
	}
}

bool server::updateServer() {
	for ( ;; ) {
		const auto & result = check_error( ::poll( &poll_fd, 1, 0 ) );
		if ( result.failed() or result == 0_sz )
			return result.success();

		const auto & revents = Flag {poll_fd.revents};
		CPP_ASSERT( revents.flag != 0 );

		if ( revents.check<POLLIN>() ) {
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
		} else {
			CPP_ASSERT( false/*Unknown event*/ );
		}
	}
	return true;
}

bool server::updateClients() {
	if ( clients.empty() )
		return true;

	auto count = check_error(
		::poll( client_pollfds.data(), client_pollfds.size(), 0 ) );
	if ( count.failed() or count == 0_sz )
		return count.success();

	for ( auto idx = 0_sz; idx < client_pollfds.size(); ++idx ) {
		const auto & revents = Flag {client_pollfds[idx].revents};
		if ( revents.flag == 0 )
			{CPP_ASSERT(false/*Remove this assert*/);continue;}

		if ( revents.check<POLLHUP>() )
			CPP_UNUSED( clients[idx]->shutdown() );
		else if ( revents.check<POLLIN>() )
			{ /** @todo Notify data availability? */ }
		else if ( revents.check<POLLNVAL>() )
			CPP_ASSERT( false/*client.close() while update()*/ );
		else
			CPP_ASSERT( false/*Unknown event*/ );

		if ( --count == 0_sz )
			break;
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
	client_pollfds.clear();
	clients.clear();

	if ( not Super::close() )
		return false;

	setState( State::CLOSED );
	return true;
}

data::result_t server::check_error( isize result )/* override*/ {
	if ( result >= 0 )
		return (usize) result;

	const auto & result_error = Super::check_error( result );
	setState( State::FAILED );
	return result_error;
}

void server::onClientState/*Changed*/( const ::lib::socket::server::client & client_ ) {
	for ( const auto & c : clients )
		if ( c.get() == &client_ )
			onClientStateChanged( c );
}

} // namespace lib::socket::impl::tcp
