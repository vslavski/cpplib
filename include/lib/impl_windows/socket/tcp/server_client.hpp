/* File: /lib/impl_windows/socket/tcp/server_client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_windows__socket__tcp__server_client__hpp
#define CPPLIB__lib__impl_windows__socket__tcp__server_client__hpp

/// @todo Unify with "posix" implementation?

#include <winsock2.h>

#include "../../../../lib/socket/server_client.hpp"

#include "./base.hpp"
#include "./server.hpp"

namespace lib::socket::impl::tcp {

// DECLARATION lib::socket::impl::tcp::server::client

class server::client
	: public base
	, public ::lib::socket::server::client
{
	using Super = base;
public:
	client( ::SOCKET sock );
	virtual ~client();

	cstring getName() const override;

	bool update() override;
	bool shutdown() override;
	bool close() override;
private:
	friend class server;

	data::result_t check_error( isize result ) override;

	::SOCKET get_sock() const { return sock; }
	::WSAEVENT get_event() const { return event; }

	bool close_lock = false;
};

} // namespace lib::socket::impl::tcp

#endif // CPPLIB__lib__impl_windows__socket__tcp__server_client__hpp
