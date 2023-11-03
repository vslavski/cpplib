/* File: /lib/impl/packets/sync/client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__packets__sync__client__hpp
#define CPPLIB__lib__impl__packets__sync__client__hpp

#include "../../../../lib/tl/listener.hpp"
#include "../../../../lib/types.hpp"
#include "../../../../lib/packets/handler.hpp"

#include "./types.hpp"

namespace lib::packets::impl::sync {

// DECLARATION lib::packets::impl::sync::Client

class Client final
	: public tag_tl_listener< Client >
{
public:
	Client( Handler::id_type packet_id, ReadHandler & handler ) noexcept;
	~Client() noexcept;

	constexpr usize quant() const noexcept { return quant_; }
	constexpr Timestamp timestamp() const noexcept { return timestamp_; }
private:
	bool onSync( const tag_serializeable & ) noexcept;

	ReadHandler & handler;

	const Handler::id_type packet_id;
	Packet packet;

	usize quant_ = 0;
	Timestamp timestamp_ = {};
};

} // namespace lib::packets::impl::sync

#endif // CPPLIB__lib__impl__packets__sync__client__hpp
