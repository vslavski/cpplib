/* File: /lib/impl/packets/sync/client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./client.hpp"

namespace lib::packets::impl::sync {

// IMPLEMENTATION lib::packets::impl::sync::Client

Client::Client( Handler::id_type packet_id, ReadHandler & handler ) noexcept
	: handler{ handler }
	, packet_id{ packet_id }
{
	handler.listen( packet_id, &packet, {*this,&Client::onSync} );
}

Client::~Client() noexcept {
	handler.unlisten( packet_id );
}

bool Client::onSync( const tag_serializeable & ) noexcept {
	quant_ = packet.quant;
	timestamp_ = packet.timestamp;
	return handler.send( packet_id, packet );
}

} // namespace lib::packets::impl::sync
