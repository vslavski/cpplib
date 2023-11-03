/* File: /lib/impl_windows/socket/tcp/server.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_windows__socket__tcp__server__hpp
#define CPPLIB__lib__impl_windows__socket__tcp__server__hpp

/// @todo Unify with "posix" implementation?

#include <winsock2.h>

#include <vector>

#include "../../../../lib/tl/listener.hpp"
#include "../../../../lib/ptr.hpp"
#include "../../../../lib/socket/server.hpp"

#include "./base.hpp"

namespace lib::socket::impl::tcp {

// DECLARATION lib::socket::impl::tcp::server

class server
	: public base
	, public ::lib::socket::server
	, public tag_tl_listener< server >
{
	using Super = base;
public:
	/// @todo Add as lib::socket::server property.
	static constexpr int MAX_CONNECTIONS = 2;

	class client;

	server() = default;
	server( u32 ip4_address, u16 port );
	virtual ~server();

	cstring getName() const override;

	bool bind( u32 ip4_address, u16 port );
	bool listen();
	SPtr<client> accept();
	bool reject();

	bool update() override;
	bool shutdown() override;
	bool close() override;
private:
	data::result_t check_error( isize result ) override;

	void onClientState/*Changed*/( const ::lib::socket::server::client & client_ );

	void updateFlushClients();
	bool updateServer();
	bool updateClients();

	bool close_lock = false;
	::std::vector< SPtr<client> > clients;
	::std::vector< ::WSAEVENT > client_events;
};

} // namespace lib::socket::impl::tcp

#endif // CPPLIB__lib__impl_windows__socket__tcp__server__hpp
