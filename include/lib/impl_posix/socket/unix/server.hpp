/* File: /lib/impl_posix/socket/unix/server.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__socket__unix__server__hpp
#define CPPLIB__lib__impl_posix__socket__unix__server__hpp

/// @todo Unify with "tcp"?

#include <poll.h>

#include <vector>

#include "../../../../lib/tl/listener.hpp"
#include "../../../../lib/cstring.hpp"
#include "../../../../lib/ptr.hpp"
#include "../../../../lib/socket/server.hpp"

#include "./base.hpp"

namespace lib::socket::impl::unix {

// DECLARATION lib::socket::impl::unix::server

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
	server( const cstring & filename );
	virtual ~server();

	cstring getName() const override;

	bool bind( const cstring & filename );
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
	struct ::pollfd poll_fd = {};
	::std::vector< SPtr<client> > clients;
	::std::vector< struct ::pollfd > client_pollfds;
};

} // namespace lib::socket::impl::unix

#endif // CPPLIB__lib__impl_posix__socket__unix__server__hpp
