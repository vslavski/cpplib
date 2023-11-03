/* File: /lib/impl/packets/ping_counter.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__packets__ping_counter__hpp
#define CPPLIB__lib__impl__packets__ping_counter__hpp

#include <chrono>

#include <cpp/lib_chrono>

#include "../../../lib/tl/listener.hpp"
#include "../../../lib/types.hpp"
#include "../../../lib/data/serialize.hpp"

#include "../../../lib/packets/handler.hpp"

namespace lib::packets::impl {

// DECLARATION lib::packets::impl::PingCounter

class PingCounter final
	: public tag_tl_listener< PingCounter >
{
public:
	using counter_type = usize;
	using id_type = Header::id_type;
	using clock_type = ::std::chrono::steady_clock;
	using latency_type = ::std::chrono::milliseconds;

	PingCounter( id_type ping_id, id_type pong_id ) noexcept;

	constexpr counter_type count() const noexcept { return local_count_; }
	constexpr latency_type latency() const noexcept { return counter.average(); }

	bool attach( ReadHandler & handler ) noexcept;
	void detach() noexcept;

	tl::listener<void> on_ping;
private:
	bool onPing( const tag_serializeable & ) noexcept;
	bool onPong( const tag_serializeable & ) noexcept;

	struct Packet : tag_serializeable {
		constexpr Packet() noexcept = default;
		constexpr Packet( counter_type counter ) noexcept : counter{ counter } {}
		counter_type counter;
		data::result_t serialized_size( data::wstream_t & stream ) const override;
		bool can_deserialize( data::rstream_t & stream ) const override;
		data::result_t serialize( data::wstream_t & stream ) const override;
		data::result_t deserialize( data::rstream_t & stream ) override;
		using tag_serializeable::deserialize;
	};

	const id_type ping_id;
	const id_type pong_id;
	Packet packet;
	ReadHandler * handler_ = nullptr;
	::cpp::chrono::counter< counter_type, clock_type, latency_type > counter;
	counter_type local_count_ = 0;
	counter_type remote_count_ = 0;
};

} // namespace lib::packets::impl

#endif // CPPLIB__lib__impl__packets__ping_counter__hpp
