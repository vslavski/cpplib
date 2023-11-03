/* File: /lib/impl_windows/socket/tcp/base.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_windows__socket__tcp__base__hpp
#define CPPLIB__lib__impl_windows__socket__tcp__base__hpp

/// @todo Unify with "posix" implementation?

#include <winsock2.h>

#include "../../../../lib/types.hpp"
#include "../../../../lib/socket/socket.hpp"

namespace lib::socket::impl::tcp {

// DECLARATION lib::socket::impl::tcp::base

class base
	: public virtual socket
{
public:
	base();
	base( ::SOCKET sock );
	virtual ~base();

	// IMPLEMENTATION lib::socket::socket

	bool update() override;
	bool open() override;
	bool close() override;
	bool is_open() const override;

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	data::result_t peek( const data::buffer_t & buffer ) override;
	data::result_t write( const data::cbuffer_t & buffer ) override;
	data::result_t read_size() override;
	data::result_t write_size() override;
	::std::error_condition read_error() const override { return error(); }
	::std::error_condition write_error() const override { return error(); }

	// IMPLEMENTATION lib::data::rwstream_t

	::std::error_condition error() const override;

	bool set_blocking( bool blocking );

protected:
	virtual data::result_t check_error( isize result );

	::SOCKET check_sock_error( ::SOCKET sock_ );
	data::result_t check_wsa_error();
	data::result_t check_wsa_error( int error );

	::SOCKET sock = INVALID_SOCKET;
	::WSAEVENT event = WSA_INVALID_EVENT;
	::std::error_condition error_;
	bool wsa_init = false;
};

} // namespace lib::socket::impl::tcp

#endif // CPPLIB__lib__impl_windows__socket__tcp__base__hpp
