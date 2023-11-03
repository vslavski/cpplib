/* File: /lib/impl/packets/sync/server_client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cpp/lib_debug>

#include "./server_client.hpp"

namespace lib::packets::impl::sync {

// IMPLEMENTATION lib::packets::impl::sync::Server::Client

Server::Client::Client( Server & server, Handler::id_type packet_id, ReadHandler & handler ) noexcept
	: server{ server }
	, quant_{ server.quant() }
	, timestamp_{ server.timestamp() }
	, handler{ handler }
	, packet_id{ packet_id }
{
	handler.listen( packet_id, &packet, {*this,&Client::onSync} );
	server.addClient( *this );
	sync();
}

Server::Client::~Client() noexcept {
	server.removeClient( *this );
	handler.unlisten( packet_id );
}

void Server::Client::sync() noexcept {
	CPP_UNUSED( handler.send( packet_id, Packet{ server.quant(), server.timestamp() } ) );
}

bool Server::Client::onSync( const tag_serializeable & ) noexcept {
	quant_ = packet.quant;
	timestamp_ = packet.timestamp;
	return true;
}

} // namespace lib::packets::impl::sync
