/* File: /lib/impl/packets/sync/server.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./server_client.hpp"

#include "./server.hpp"

namespace lib::packets::impl::sync {

// IMPLEMENTATION lib::packets::impl::sync::Server

Server::Server() noexcept
	: base_timestamp{ ::std::chrono::duration_cast<Timestamp>(
		::std::chrono::steady_clock::now().time_since_epoch() ) }
{}

Server::~Server() noexcept {
}

void Server::sync() noexcept {
	++quant_;
	timestamp_ = ::std::chrono::duration_cast<Timestamp>
		( ::std::chrono::steady_clock::now().time_since_epoch() )
		- base_timestamp;

	auto new_lower_quant = quant_;
	lower_timestamp_ = timestamp_;
	for ( auto & client : clients ) {
		new_lower_quant = ::std::min( new_lower_quant, client.get().quant() );
		lower_timestamp_ = ::std::min( lower_timestamp_, client.get().timestamp() );

		client.get().sync();
	}

	if ( new_lower_quant != lower_quant_ ) {
		lower_quant_ = new_lower_quant;
		on_sync( lower_quant_, lower_timestamp_ );
	}
}

void Server::addClient( Client & client ) noexcept {
	clients.emplace_back( MkRef<Client>( client ) );
}

void Server::removeClient( Client & client ) noexcept {
	const auto it = ::std::remove_if( clients.begin(), clients.end()
		, [&]( auto c ){ return &c.get() == &client; });
	clients.erase( it );

	if ( clients.empty() ) {
		lower_quant_ = 0;
		lower_timestamp_ = {};
	}
}

} // namespace lib::packets::impl::sync
