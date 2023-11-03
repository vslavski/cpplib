/* File: /lib/impl_posix/socket/unix/server_client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__socket__unix__server_client__hpp
#define CPPLIB__lib__impl_posix__socket__unix__server_client__hpp

/// @todo Unify with "tcp"?

#include "../../../../lib/socket/server_client.hpp"

#include "./base.hpp"
#include "./server.hpp"

namespace lib::socket::impl::unix {

// DECLARATION lib::socket::impl::unix::server::client

class server::client
	: public base
	, public ::lib::socket::server::client
{
	using Super = base;
public:
	client( int sock );
	virtual ~client();

	cstring getName() const override;

	bool update() override;
	bool shutdown() override;
	bool close() override;
private:
	data::result_t check_error( isize result ) override;
	bool close_lock = false;
};

} // namespace lib::socket::impl::unix

#endif // CPPLIB__lib__impl_posix__socket__unix__server_client__hpp
