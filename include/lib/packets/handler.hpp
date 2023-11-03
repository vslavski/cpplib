/* File: /lib/packets/handler.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__packets__handler__hpp
#define CPPLIB__lib__packets__handler__hpp

#include <initializer_list>
#include <tuple>
#include <vector>
#include <set>
#include <type_traits>
#include <utility>

#include <cpp/lib_algorithm>

#include "../../lib/tl/listener.hpp"
#include "../../lib/types.hpp"
#include "../../lib/ptr.hpp"
#include "../../lib/data/stream.hpp"
#include "../../lib/data/serialize.hpp"
#include "../../lib/utils/enum.hpp"

#include "./packet.hpp"

namespace lib::packets {

// DECLARATION lib::packets::Handler

class Handler {
public:
	using id_type = Header::id_type;
	using count_type = ::std::make_unsigned_t<id_type>;

	class ISendAggregator;

	using Receiver = tl::listener<bool, const tag_serializeable &>;
	using Receivers = ::std::vector< Receiver >;

	/// @todo ::std::error_condition
	LIB_UTILS_ENUM( Error
		, SUCCESS
		, SEND_QUEUE_FULL
		, SEND_HEADER_STREAM_FAILED
		, SEND_HEADER_PARTIAL
		, SEND_DATA_STREAM_FAILED
		, SEND_DATA_PARTIAL
		, RECEIVE_HEADER_STREAM_FAILED
		, RECEIVE_HEADER_PARTIAL
		, RECEIVE_HEADER_BAD_ID
		, RECEIVE_PACKET_UNKNOWN
		, RECEIVE_DATA_STREAM_FAILED
		, RECEIVE_DATA_DESERIALIZE_FAILED
		, RECEIVE_DATA_PARTIAL
		, RECEIVE_NO_RECEIVER
		, RECEIVE_RECEIVER_FAILED
	)

	/*constexpr */Handler() noexcept = default;
	virtual ~Handler() = default;

	constexpr Error error() const noexcept { return error_; }

	void reset( data::rwstream_t * stream_ = nullptr ) noexcept;

	void listen( id_type id, const Receiver & receiver ) noexcept;
	void listen( id_type first_id, ::std::initializer_list<Receiver> receivers_ ) noexcept;
	void unlisten( id_type id ) noexcept;
	void unlisten( id_type first_id, count_type count ) noexcept;

	bool attach_aggregator( const SPtr<ISendAggregator> & aggregator ) noexcept;

	template< class T >
	constexpr bool send( const T & packet ) noexcept;
	bool send( id_type id, const tag_serializeable & packet ) noexcept;

	bool receive() noexcept;

	constexpr const Header & recv_header() const noexcept { return recv_header_; }
	constexpr const ::std::error_condition & deserialize_error() const noexcept { return deserialize_error_; }
protected:
	using DeserializeResult = ::std::tuple< const tag_serializeable *, data::result_t >;
	virtual DeserializeResult deserialize( data::rstream_t & stream, const Header & header ) = 0;
private:
	static constexpr auto HEADER_SIZE = sizeof(Header);

	bool send_header( id_type id, Header::size_type size ) noexcept;
	bool set_error( Error error ) noexcept;

	bool send_aggregated( id_type id, const tag_serializeable & packet ) noexcept;
	void flush_last_aggregator( const SPtr<ISendAggregator> & aggregator ) noexcept;
	void reset_aggregators() noexcept;

	Receivers receivers;
	data::rwstream_t * stream = nullptr;
	Error error_ = Error::SUCCESS;
	Header recv_header_ = {};
	::std::error_condition deserialize_error_;

	::std::set< WPtr<ISendAggregator>, ::cpp::wptr_less<WPtr<ISendAggregator>> > aggregators;
	WPtr<ISendAggregator> last_aggregator;
};

// DECLARATION lib::packets::ReadHandler

class ReadHandler
	: public Handler
{
	using Super = Handler;
public:
	/*constexpr */ReadHandler() noexcept = default;
	virtual ~ReadHandler() = default;

	void listen( id_type id, tag_serializeable * packet, const Receiver & receiver ) noexcept;
	void listen
		( id_type first_id
		, ::std::initializer_list<tag_serializeable*> packets
		, ::std::initializer_list<Receiver> receivers_
		) noexcept;
	void unlisten( id_type id ) noexcept;
	void unlisten( id_type first_id, count_type count ) noexcept;

	using Super::send;
private:
	using Super::listen;
	using Super::unlisten;

	DeserializeResult deserialize( data::rstream_t & stream_, const Header & header ) override;

	::std::vector<tag_serializeable*> packets;
};

// INLINES lib::packets::Handler

template< class T >
inline constexpr bool Handler::send( const T & packet ) noexcept {
	return send( T::ID, packet );
}

} // namespace lib::packets

LIB_UTILS_ENUM_NAMES( lib::packets::Handler::Error
	, "success"
	, "send queue is full"
	, "send-header stream failed"
	, "send-header partial"
	, "send-data stream_failed"
	, "send-data partial"
	, "receive-header stream failed"
	, "receive-header partial"
	, "receive-header bad id"
	, "receive-packet unknown"
	, "receive-data stream failed"
	, "receive-data deserialize failed"
	, "receive-data partial"
	, "receive no receiver"
	, "receive receiver failed"
	)

#endif // CPPLIB__lib__packets__handler__hpp
