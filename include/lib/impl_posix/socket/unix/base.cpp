/* File: /lib/impl_posix/socket/unix/base.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <linux/sockios.h>
#include <unistd.h>

#include <cerrno>

#include <limits>
#include <system_error>
#include <utility>

#include <cpp/lib_debug>

#include "./base.hpp"

namespace lib::socket::impl::unix {

// IMPLEMENTATION lib::socket::impl::unix::base

base::base( int sock )
	: sock{ sock }
{}

/*virtual */base::~base()
{ CPP_UNUSED( close() ); }

bool base::update()/* override*/ {
	return not error_
	and ( not is_open() or read({}).success() );
}

bool base::open()/* override*/ {
	CPP_ASSERT( not is_open() );
	CPP_ASSERT( not error_/**< @todo Add 'reset()' method. */ );
	const auto & result = check_error( ::socket( AF_UNIX, SOCK_SEQPACKET, 0 ) );
	if ( result.failed() )
		return false;
	sock = (int) result.value();

	return set_blocking( false );
}

bool base::close()/* override*/ {
	if ( not is_open() )
		return true;
	const int sock_ = ::std::exchange( sock, -1 );
	return check_error( ::close( sock_ ) ).success();
}

bool base::is_open() const/* override*/ {
	return sock >= 0;
}

data::result_t base::read( const data::buffer_t & buffer )/* override*/ {
	return check_error( ::read( sock, buffer.data(), buffer.size() ) );
}

data::result_t base::peek( const data::buffer_t & buffer )/* override*/ {
	return check_error( ::recv( sock, buffer.data(), buffer.size(), MSG_PEEK ) );
}

data::result_t base::write( const data::cbuffer_t & buffer )/* override*/ {
	return check_error( ::write( sock, buffer.data(), buffer.size() ) );
}

data::result_t base::read_size()/* override*/ {
	int value = 0;
	auto result = check_error( ::ioctl( sock, SIOCINQ/*FIONREAD*/, &value ) );
	if ( result.success() )
		result = (usize) value;
	return result;
}

data::result_t base::write_size()/* override*/ {
	/// @note SIOCOUTQ is not supported by 'unix' sockets.
	return ::std::numeric_limits<data::result_t::value_type>::max();
}

::std::error_condition base::error() const/* override*/
{ return error_; }

bool base::set_blocking( bool blocking ) {
	int nonblocking = blocking ? 0 : 1;
	return check_error( ::ioctl( sock, FIONBIO, &nonblocking ) ).success();
}

/*virtual */data::result_t base::check_error( isize result ) {
	if ( result >= 0 )
		return (usize) result;
	if ( error_ )
		return error_;

	const auto code = (::std::errc) errno;
	const auto & tmp_error = ::std::make_error_condition( code );

	switch ( code ) {
	case ::std::errc::operation_in_progress:
	case ::std::errc::resource_unavailable_try_again:
		return tmp_error;
	case ::std::errc::already_connected:				///< @todo "return true;" here?
	case ::std::errc::not_connected:					///< @todo Check if could be usefull.
		break;
	default:
		break;
	};

	error_ = tmp_error;
	return error_;
}

bool base::is_inprogress( isize result, bool check_error_/* = true*/ ) {
	CPP_ASSERT( result < 0 );
	const auto code = (::std::errc) errno;
	if ( code == ::std::errc::operation_in_progress or code == ::std::errc::resource_unavailable_try_again )
		return true;
	if ( check_error_ )
		CPP_UNUSED( check_error( result ) );
	return false;
}

} // namespace lib::socket::impl::unix
