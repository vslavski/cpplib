/* File: /lib/impl/packets/sync/types.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__packets__sync__types__hpp
#define CPPLIB__lib__impl__packets__sync__types__hpp

#include <chrono>

#include "../../../../lib/types.hpp"
#include "../../../../lib/data/serialize.hpp"

namespace lib::packets::impl::sync {

// DECLARATION lib::packets::impl::sync::Timestamp

using Timestamp = ::std::chrono::milliseconds;

// DECLARATION lib::packets::impl::sync::Packet

struct Packet
	: tag_serializeable
{
	usize quant;
	Timestamp timestamp;

	constexpr Packet() = default;
	constexpr Packet( usize quant, Timestamp timestamp  ) : quant{ quant }, timestamp{ timestamp } {}

	data::result_t serialized_size( data::wstream_t & stream ) const override { return data::serialized_size( stream, quant, timestamp ); }
	bool can_deserialize( data::rstream_t & stream ) const override { return data::can_deserialize( stream, quant, timestamp ); }
	data::result_t serialize( data::wstream_t & stream ) const override { return data::serialize( stream, quant, timestamp ); }
	data::result_t deserialize( data::rstream_t & stream ) override { return data::deserialize( stream, quant, timestamp ); }
	using tag_serializeable::deserialize;
};

} // namespace lib::packets::impl::sync

#endif // CPPLIB__lib__impl__packets__sync__types__hpp
