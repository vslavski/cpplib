/* File: /lib/impl/packets/sync/server.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__packets__sync__server__hpp
#define CPPLIB__lib__impl__packets__sync__server__hpp

#include <vector>

#include "../../../../lib/tl/listener.hpp"
#include "../../../../lib/ref.hpp"

#include "./types.hpp"

namespace lib::packets::impl::sync {

// DECLARATION lib::packets::impl::sync::Server

class Server final {
public:
	class Client;

	Server() noexcept;
	~Server() noexcept;

	constexpr usize quant() const noexcept { return quant_; }
	constexpr usize lower_quant() const noexcept { return lower_quant_; }
	constexpr Timestamp timestamp() const noexcept { return timestamp_; }
	constexpr Timestamp lower_timestamp() const noexcept { return lower_timestamp_; }

	void sync() noexcept;

	tl::listener< void, usize/*quant*/, Timestamp > on_sync;
private:
	void addClient( Client & client ) noexcept;
	void removeClient( Client & client ) noexcept;

	const Timestamp base_timestamp;

	usize quant_ = 0;
	usize lower_quant_ = 0;
	Timestamp timestamp_ = {};
	Timestamp lower_timestamp_ = {};

	::std::vector< Ref<Client> > clients;
};

} // namespace lib::packets::impl::sync

#endif // CPPLIB__lib__impl__packets__sync__server__hpp
