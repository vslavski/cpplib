/* File: /lib/impl_windows/socket/tcp/base.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <winerror.h>
#include <winsock2.h>

#include <limits>
#include <utility>

#include <cpp/lib_debug>

#include "../../../../lib/literals.hpp"

#include "../../../../lib/impl_windows/system/error.hpp"

#include "./base.hpp"

namespace lib::socket::impl::tcp {

// IMPLEMENTATION lib::socket::impl::tcp::base

base::base() {
	WSADATA wsaData;
	wsa_init = check_wsa_error( ::WSAStartup( MAKEWORD(2, 2), &wsaData ) ).success();

	event = ::WSACreateEvent();
	if ( event != WSA_INVALID_EVENT )
		CPP_UNUSED( check_wsa_error() );
}

base::base( ::SOCKET sock )
	: sock{ sock }
{
	CPP_ASSERT( sock != INVALID_SOCKET );

	event = ::WSACreateEvent();
	if ( event != WSA_INVALID_EVENT )
		CPP_UNUSED( check_wsa_error() );
}

/*virtual */base::~base() {
	/// @warning Call to close() can't be done, as it cause invalid vtable call to is_open() method inside.
	/// This is probably due to MSVC bug (or behavior with /vd0 flag) for virtual inherited classes.
	CPP_ASSERT ( not is_open() );

	if ( event != WSA_INVALID_EVENT ) {
		const auto result = ::WSACloseEvent( event );
		CPP_UNUSED( result );
		CPP_ASSERT( result != FALSE );
	}

	if ( wsa_init ) {
		const auto & result = check_wsa_error( ::WSACleanup() );
		CPP_UNUSED( result );
		CPP_ASSERT( result.success() );
	}
}

bool base::update()/* override*/ {
	return not error_;
}

bool base::open()/* override*/ {
	CPP_ASSERT( not is_open() );
	CPP_ASSERT( not error_/**< @todo Add 'reset()' method. */ );
	const auto sock_ = check_sock_error( ::socket( AF_INET, SOCK_STREAM, 0 ) );
	if ( sock_ == INVALID_SOCKET )
		return false;
	sock = sock_;

	return set_blocking( false );
}

bool base::close()/* override*/ {
	if ( not is_open() )
		return true;
	const auto sock_ = ::std::exchange( sock, INVALID_SOCKET );
	return check_error( ::closesocket( sock_ ) ).success();
}

bool base::is_open() const/* override*/ {
	return sock != INVALID_SOCKET;
}

data::result_t base::read( const data::buffer_t & buffer )/* override*/ {
	return check_error( ::recv( sock, (char*) buffer.data(), (int) buffer.size(), 0 ) );
}

data::result_t base::peek( const data::buffer_t & buffer )/* override*/ {
	return check_error( ::recv( sock, (char*) buffer.data(), (int) buffer.size(), MSG_PEEK ) );
}

data::result_t base::write( const data::cbuffer_t & buffer )/* override*/ {
	return check_error( ::send( sock, (const char*) buffer.data(), (int) buffer.size(), 0 ) );
}

data::result_t base::read_size()/* override*/ {
	u_long value = 0;
	auto result = check_error( ::ioctlsocket( sock, FIONREAD, &value ) );
	if ( result.success() )
		result = (usize) value;
	return result;
}

data::result_t base::write_size()/* override*/ {
	/// @note FIONWRITE is not supported by 'tcp' sockets.
	return ::std::numeric_limits<data::result_t::value_type>::max();
}

::std::error_condition base::error() const/* override*/
{ return error_; }

bool base::set_blocking( bool blocking ) {
	auto nonblocking = (u_long) (blocking ? 0 : 1);
	return check_error( ::ioctlsocket( sock, FIONBIO, &nonblocking ) ).success();
}

/*virtual */data::result_t base::check_error( isize result ) {
	if ( result >= 0 )
		return (usize) result;
	if ( error_ )
		return error_;
	return check_wsa_error();
}

::SOCKET base::check_sock_error( ::SOCKET sock_ ) {
	if ( sock_ == INVALID_SOCKET )
		CPP_UNUSED( check_error( SOCKET_ERROR ) );
	return sock_;
}

data::result_t base::check_wsa_error() {
	return check_wsa_error( ::WSAGetLastError() );
}

data::result_t base::check_wsa_error( int wsa_error ) {
	if ( wsa_error == 0 )
		return 0_sz;

	const auto & tmp_error = make_windows_error( wsa_error );

	switch ( wsa_error ) {
	case WSAEWOULDBLOCK:
		return tmp_error;
	case WSAEISCONN:					///< @todo "return true;" here?
	case WSAENOTCONN:					///< @todo Check if could be usefull.
		break;
	default:
		break;
	};

	error_ = tmp_error;
	return error_;
}

} // namespace lib::socket::impl::tcp
