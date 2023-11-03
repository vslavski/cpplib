/* File: /lib/packets/handler.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <initializer_list>
#include <tuple>
#include <vector>
#include <utility>

#include <cpp/lib_debug>

#include "../../lib/tl/listener.hpp"
#include "../../lib/types.hpp"
#include "../../lib/data/serialize.hpp"
#include "../../lib/utils/enum.hpp"

#include "./packet.hpp"
#include "./aggregator.hpp"

#include "./handler.hpp"

namespace lib::packets {

// IMPLEMENTATION lib::packets::Handler

void Handler::reset( data::rwstream_t * stream_/* = nullptr*/ ) noexcept {
	reset_aggregators();
	deserialize_error_.clear();
	recv_header_.id = 0;
	error_ = Error::SUCCESS;
	stream = stream_;
}

void Handler::listen( id_type id, const Receiver & receiver ) noexcept {
	const auto size = ::std::max<usize>( receivers.size(), id + 1 );
	receivers.resize( size );
	receivers[id] = receiver;
}

void Handler::listen( id_type first_id, ::std::initializer_list<Receiver> receivers_ ) noexcept {
	for ( const auto & receiver : receivers_ )
		listen( first_id++, receiver );
}

void Handler::unlisten( id_type id ) noexcept {
	CPP_ASSERT( id < receivers.size() );
	receivers[id] = {};
}

void Handler::unlisten( id_type first_id, count_type count ) noexcept {
	while ( count-- > 0 )
		unlisten( first_id + count );
}

bool Handler::attach_aggregator( const SPtr<ISendAggregator> & aggregated ) noexcept {
	return aggregators.emplace( aggregated ).second;
}

bool Handler::send( id_type id, const tag_serializeable & packet ) noexcept {
	if ( send_aggregated( id, packet ) )
		return true;
	CPP_ASSERT( stream != nullptr );
	const auto & data_size = serialized_size( *stream, packet );
	CPP_ASSERT( data_size.success() );
	if ( not send_header( id, (Header::size_type) data_size.value() ) )
		return false;
	if ( data_size == 0_sz )
		return true;
	const auto & data_write = serialize( *stream, packet );
	if ( data_write == data_size )
		return true;
	return set_error( data_write.failed()
		? Error::SEND_DATA_STREAM_FAILED
		: Error::SEND_DATA_PARTIAL );
}

bool Handler::receive() noexcept {
	CPP_ASSERT( stream != nullptr );
	for ( ;; ) {
		if ( recv_header_.id == 0 ) {
			if ( not can_deserialize( *stream, recv_header_ ) )
				return true;
			const auto & header_read = data::deserialize( *stream, recv_header_ );
			if ( header_read == 0_sz )
				return true;
			if ( header_read != HEADER_SIZE )
				return set_error( header_read.failed()
				? Error::RECEIVE_HEADER_STREAM_FAILED
				: Error::RECEIVE_HEADER_PARTIAL );
			CPP_ASSERT( recv_header_.id != 0 );
		}
		const auto & read_size = stream->read_size();
		if ( read_size.failed() )
			return set_error( Error::RECEIVE_DATA_STREAM_FAILED );
		if ( read_size < recv_header_.size )
			return true;
		if ( recv_header_.id >= receivers.size() )
			return set_error( Error::RECEIVE_HEADER_BAD_ID );
		const auto & [ packet, data_read ] = deserialize( *stream, recv_header_ );
		if ( packet == nullptr )
			return set_error( Error::RECEIVE_PACKET_UNKNOWN );
		if ( data_read.failed() ) {
			deserialize_error_ = data_read.error();
			return set_error( Error::RECEIVE_DATA_STREAM_FAILED );
		}
		if ( data_read != recv_header_.size ) {
			if ( data_read == 0_sz )
				return true;
			if ( data_read > 0_sz )
				return set_error( Error::RECEIVE_DATA_PARTIAL );
		}
		auto & receiver = receivers[ recv_header_.id ];
		if ( not receiver )
			return set_error( Error::RECEIVE_NO_RECEIVER );
		if ( not receiver( *packet ) )
			return set_error( Error::RECEIVE_RECEIVER_FAILED );
		recv_header_.id = 0;
	}
}

bool Handler::send_header( id_type id, Header::size_type size ) noexcept {
	const auto total_size = HEADER_SIZE + size;
	const auto & write_size = stream->write_size();
	if ( write_size.failed() )
		return set_error( Error::SEND_HEADER_STREAM_FAILED );
	if ( total_size > write_size )
		return set_error( Error::SEND_QUEUE_FULL );
	const Header header{ id, size };
	const auto & header_write = serialize( *stream, header );
	if ( header_write == HEADER_SIZE )
		return true;
	return set_error( header_write.failed()
		? Error::SEND_HEADER_STREAM_FAILED
		: Error::SEND_HEADER_PARTIAL );
}

bool Handler::set_error( Error error ) noexcept {
	if ( error_ == Error::SUCCESS )
		error_ = error;
	return false;
}

bool Handler::send_aggregated( id_type id, const tag_serializeable & packet ) noexcept {
	for ( auto it = aggregators.begin(); it != aggregators.end(); ) {
		const auto & aggregator = it->lock();
		if ( not aggregator ) {
			it = aggregators.erase( it );
			continue;
		}
		if ( *aggregator != id ) {
			++it;
			continue;
		}
		flush_last_aggregator( aggregator );
		aggregator->process( id, packet );
		return true;
	}
	flush_last_aggregator( {} );
	return false;
}

void Handler::flush_last_aggregator( const SPtr<ISendAggregator> & aggregator ) noexcept {
	const auto & last = last_aggregator.lock();
	if ( last and last != aggregator )
		last->flush( *this );
	last_aggregator = aggregator;
}

void Handler::reset_aggregators() noexcept {
	last_aggregator.reset();
	for ( auto it = aggregators.begin(); it != aggregators.end(); ) {
		const auto & aggregator = it->lock();
		if ( not aggregator ) {
			it = aggregators.erase( it );
			continue;
		}
		aggregator->reset();
		++it;
	}

}

// IMPLEMENTATION lib::packets::ReadHandler

void ReadHandler::listen( id_type id, tag_serializeable * packet, const Receiver & receiver ) noexcept {
	Super::listen( id, receiver );
	const auto size = ::std::max<usize>( packets.size(), id + 1 );
	packets.resize( size );
	packets[id] = packet;
}

void ReadHandler::listen
	( id_type first_id
	, ::std::initializer_list<tag_serializeable*> packets_
	, ::std::initializer_list<Receiver> receivers_
	) noexcept
{
	/*static_*/CPP_ASSERT( packets_.size() == receivers_.size() );
	auto receiver = receivers_.begin();
	for ( auto * packet : packets_ )
		listen( first_id++, packet, *(receiver++) );
}

void ReadHandler::unlisten( id_type id ) noexcept {
	Super::unlisten( id );
	CPP_ASSERT( id < packets.size() );
	packets[id] = nullptr;
}

void ReadHandler::unlisten( id_type first_id, count_type count ) noexcept {
	while ( count-- > 0 )
		unlisten( first_id + count );
}

ReadHandler::DeserializeResult ReadHandler
	::deserialize( data::rstream_t & stream_, const Header & header )
{
	if ( packets.size() <= header.id )
		return {};
	auto * packet = packets[header.id];
	if ( packet == nullptr )
		return {};
	return { packet, packet->deserialize( stream_ ) };
}

} // namespace lib::packets
