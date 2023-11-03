/* File: /lib/impl_posix/socket/unix/base.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__socket__unix__base__hpp
#define CPPLIB__lib__impl_posix__socket__unix__base__hpp

/// @todo Unify with "tcp"?

#include "../../../../lib/types.hpp"
#include "../../../../lib/socket/socket.hpp"

#ifdef unix
	#undef unix
#endif // unix
namespace lib::socket::impl::unix {

// DECLARATION lib::socket::impl::unix::base

class base
	: public virtual socket
{
public:
	base() = default;
	base( int sock );
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

	bool is_inprogress( isize result, bool check_error_ = true );

	int sock = -1;
	::std::error_condition error_;
};

} // namespace lib::socket::impl::unix

#endif // CPPLIB__lib__impl_posix__socket__unix__base__hpp
