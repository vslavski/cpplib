/* File: /lib/impl/stream/stdio.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "../../../lib/debug/platform.hpp"

#ifdef CPPLIB_PLATFORM_POSIX
	#include <sys/ioctl.h>
	#include <unistd.h>
#endif // CPPLIB_PLATFORM_POSIX

#include <cerrno>
#include <cstdio>
#include <cstdarg>

#include <limits>

#include "./stdio.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::stdio

data::result_t stdio::putc( char symbol ) {
	return check_error( ::std::fputc( symbol, stdout ) );
}

data::result_t stdio::puts( const char* string ) {
	return check_error( ::std::fputs( string, stdout ) );
}

data::result_t stdio::printf( CPPLIB_PRINTF_ANNOTATION const char* format, ... )
	//CPPLIB_PRINTF_ATTRIBUTE( format(printf,/*implicit this,*/2,3) )
{
	va_list args;
	va_start( args, format );
	const auto result = check_error( ::std::vfprintf( stdout, format, args ) );
	va_end( args );
	return result;
}

data::result_t stdio::check_error( isize result ) {
	if ( result >= 0 )
		return (usize) result;
	if ( errno == 0 )
		return {};
	return error_ = ::std::make_error_condition( (::std::errc) errno );
}

// IMPLEMENTATION lib::stream::impl::stdio: lib::data::rstream_t, lib::data::wstream_t

data::result_t stdio::read( const data::buffer_t & buffer )/* override*/ {
#ifndef CPPLIB_PLATFORM_POSIX
	const auto result = ::std::fread( buffer.data(), 1, buffer.size(), stdin );
	if ( result == 0 and check_error( ::std::ferror( stdin ) ).failed() )
		return error_;
	return result;
#else
	return check_error( ::read( STDIN_FILENO, buffer.data(), buffer.size() ) );
#endif // not CPPLIB_PLATFORM_POSIX
}

data::result_t stdio::write( const data::cbuffer_t & buffer )/* override*/ {
#ifndef CPPLIB_PLATFORM_POSIX
	const auto result = ::std::fwrite( buffer.data(), 1, buffer.size(), stdout );
	if ( result == 0 and check_error( ::std::ferror( stdout ) ).failed() )
		return error_;
	return result;
#else
	return check_error( ::write( STDOUT_FILENO, buffer.data(), buffer.size() ) );
#endif // not CPPLIB_PLATFORM_POSIX
}

bool stdio::read_flush()/* override*/ {
	/// @todo Implement?
	return false;
}

bool stdio::write_flush()/* override*/ {
	return check_error( ::std::fflush( stdout ) ).success();
}

data::result_t stdio::read_size()/* override*/ {
#ifndef CPPLIB_PLATFORM_POSIX
	/// @todo Is it possible to determine it standard way?
	return ::std::numeric_limits< decltype(read_size()) >::max();
#else
	int value = 0;
	auto result = check_error( ::ioctl( STDIN_FILENO, FIONREAD, &value ) );
	if ( result.success() )
		result = (usize) value;
	return result;
#endif // not CPPLIB_PLATFORM_POSIX
}

data::result_t stdio::write_size()/* override*/ {
#ifndef CPPLIB_PLATFORM_POSIX
	/// @todo Is it possible to determine it standard way?
	return ::std::numeric_limits< decltype(write_size()) >::max();
#else
	int value = 0;
	auto result = check_error( ::ioctl( STDOUT_FILENO, TIOCOUTQ, &value ) );
	if ( result.success() )
		result = (usize) value;
	return result;
#endif // not CPPLIB_PLATFORM_POSIX
}

} // namespace lib::stream::impl
