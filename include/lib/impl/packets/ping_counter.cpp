/* File: /lib/impl/packets/ping_counter.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <utility>

#include <cpp/lib_debug>

#include "./ping_counter.hpp"

namespace lib::packets::impl {

// IMPLEMENTATION lib::packets::impl::PingCounter

PingCounter::PingCounter( id_type ping_id, id_type pong_id ) noexcept
	: ping_id{ ping_id }
	, pong_id{ pong_id }
{}

bool PingCounter::attach( ReadHandler & handler ) noexcept {
	detach();
	handler.listen( ping_id, &packet, { *this, &PingCounter::onPing } );
	handler.listen( pong_id, &packet, { *this, &PingCounter::onPong } );
	if ( not handler.send( ping_id, Packet{ 1 } ) )
		return false;
	handler_ = &handler;
	counter.reset().next();
	local_count_ = 1;
	remote_count_ = 0;
	return true;
}

void PingCounter::detach() noexcept {
	if ( handler_ == nullptr )
		return;
	ReadHandler * handler = nullptr;
	::std::swap( handler, handler_ );
	handler->unlisten( pong_id );
	handler->unlisten( ping_id );
}

bool PingCounter::onPing( const tag_serializeable & ) noexcept {
	if ( remote_count_ + 1 != packet.counter )
		return false;
	if ( not handler_->send( pong_id, Packet{ packet.counter } ) )
		return false;
	++remote_count_;
	return true;
}

bool PingCounter::onPong( const tag_serializeable & ) noexcept {
	if ( local_count_ != packet.counter )
		return false;
	if ( not handler_->send( ping_id, Packet{ local_count_ + 1 } ) )
		return false;
	counter.next();
	++local_count_;
	CPP_ASSERT( (counter_type)local_count_ == counter.count() );
	on_ping();
	return true;
}

// IMPLEMENTATION lib::packets::impl::PingCounter::Packet

data::result_t PingCounter::Packet::serialized_size( data::wstream_t & stream ) const/* override*/ {
	return data::serialized_size( stream, counter );
}

bool PingCounter::Packet::can_deserialize( data::rstream_t & stream ) const/* override*/ {
	return data::can_deserialize( stream, counter );
}

data::result_t PingCounter::Packet::serialize( data::wstream_t & stream ) const/* override*/ {
	return data::serialize( stream, counter );
}

data::result_t PingCounter::Packet::deserialize( data::rstream_t & stream )/* override*/ {
	return data::deserialize( stream, counter );
}

} // namespace lib::packets::impl
