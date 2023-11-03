/* File: /lib/packets/aggregator.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__packets__aggregator__hpp
#define CPPLIB__lib__packets__aggregator__hpp

#include <cpp/lib_algorithm>

#include "../../lib/types.hpp"
#include "../../lib/data/serialize.hpp"

#include "./packet.hpp"
#include "./handler.hpp"

namespace lib::packets {

// DECLARATION lib::packets::Handler::ISendAggregator

class Handler::ISendAggregator {
public:
	constexpr ISendAggregator( id_type first_id, count_type count = 1 ) noexcept;
	virtual ~ISendAggregator() = default;
	constexpr bool operator < ( const ISendAggregator & other ) const noexcept;
	constexpr bool operator == ( const ISendAggregator & other ) const noexcept;
	constexpr bool operator == ( Header::id_type id ) const noexcept;

	virtual void process( Header::id_type id, const tag_serializeable & packet ) noexcept = 0;
	virtual void flush( Handler & handler ) noexcept = 0;
	virtual void reset() noexcept = 0;
private:
	id_type first_id;
	count_type count;
};

// INLINES lib::packets::Handler::ISendAggregator

inline constexpr Handler::ISendAggregator::ISendAggregator( id_type first_id, count_type count/* = 1*/ ) noexcept
	: first_id{ first_id }
	, count{ count }
{}

inline constexpr bool Handler::ISendAggregator::operator < ( const ISendAggregator & other ) const noexcept {
	return *this != other and first_id < other.first_id;
}

inline constexpr bool Handler::ISendAggregator::operator == ( const ISendAggregator & other ) const noexcept {
	return first_id + count > other.first_id and other.first_id + other.count > first_id;
}

inline constexpr bool Handler::ISendAggregator::operator == ( Header::id_type id ) const noexcept {
	return ::cpp::is_in_range<Header::id_type>( id, first_id, first_id + count - 1 );
}

} // namespace lib::packets

#endif // CPPLIB__lib__packets__aggregator__hpp
