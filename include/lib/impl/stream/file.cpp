/* File: /lib/impl/stream/file.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cerrno>

#include <cpp/lib_debug>

#include "./file.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::file

file::file( const char * name, const char * mode/* = "r"*/ ) {
	CPP_UNUSED( open( name, mode ) );
}

file::file( FILE * file_handle )
	: file_ {file_handle}
{}

file::~file() {
	CPP_UNUSED( close() );
}

file & file::operator = ( FILE * file_handle ) {
	reset();
	file_ = file_handle;
	return *this;
}

bool file::open( const char * name, const char * mode/* = "r"*/ ) {
	CPP_ASSERT( file_ == nullptr );
	CPP_ASSERT( not error_ );
	file_ = ::std::fopen( name, mode );
	if ( file_ == nullptr )
		set_error_from_errno();
	return file_ != nullptr;
}

bool file::close() {
	if ( file_ == nullptr )
		return true;
	const auto result = ::std::fclose( file_ );
	file_ = nullptr;
	if ( result != 0 )
		set_error_from_errno();
	return result == 0;
}

void file::reset() {
	CPP_UNUSED( close() );
	CPP_ASSERT( file_ == nullptr );
	error_.clear();
}

const ::std::error_condition & file::set_error_from_errno() {
	return error_ = ::std::make_error_condition( (::std::errc) errno );
}

// IMPLEMENTATION lib::stream::impl::file: lib::data::rstream_t, lib::data::wstream_t

data::result_t file::read( const data::buffer_t & buffer )/* override*/ {
	const auto result = ::std::fread( buffer.data(), 1, buffer.size(), file_ );
	if ( ::std::ferror( file_ ) != 0 )
		return set_error_from_errno();
	return result;
}

data::result_t file::peek( const data::buffer_t & buffer )/* override*/ {
	::std::fpos_t position;
	if ( ::std::fgetpos( file_, &position ) != 0 )
		return set_error_from_errno();
	const auto & result = read( buffer );
	if ( result.failed() )
		return result;
	if ( ::std::fsetpos( file_, &position ) != 0 )
		return set_error_from_errno();
	return result;
}

data::result_t file::write( const data::cbuffer_t & buffer )/* override*/ {
	const auto result = ::std::fwrite( buffer.data(), 1, buffer.size(), file_ );
	if ( ::std::ferror( file_ ) != 0 )
		return set_error_from_errno();
	return result;
}

// IMPLEMENTATION lib::stream::impl::file: lib::data::unistream_t

bool file::flush()/* override*/ {
	if ( ::std::fflush( file_ ) == 0 )
		return true;
	set_error_from_errno();
	return false;
}

data::result_t file::size()/* override*/ {
	const auto & offset = pos();
	if ( offset.failed() )
		return offset;
	if ( ::std::fseek( file_, 0, SEEK_END ) != 0 )
		return set_error_from_errno();
	const auto & size_ = pos();
	if ( size_.failed() )
		return size_;
	if ( ::std::fseek( file_, (long) offset.value(), SEEK_SET ) != 0 )
		return set_error_from_errno();
	return size_ - offset;
}

// IMPLEMENTATION lib::stream::impl::file: lib::data::rwstream_t

data::result_t file::pos( usize position )/* override*/ {
	if ( ::std::fseek( file_, (long) position, SEEK_SET ) != 0 )
		return set_error_from_errno();
	return position;
}

data::result_t file::pos()/* override*/ {
	const auto offset = ::std::ftell( file_ );
	if ( offset < 0 )
		return set_error_from_errno();
	return (usize) offset;
}

// IMPLEMENTATION lib::stream::impl::file::dbg_test()

void file::dbg_test() {
	file f;
	::std::printf( "open:rw+= %s\n", f.open( "foo__.txt", "rw+" )?"true":"false" );
	::std::printf( "is_open = %s\n", f.is_open()?"true":"false" );
	::std::printf( "flush   = %s\n", f.flush()?"true":"false" );
	::std::printf( "write(0)= %zi\n", (usize) f.write({ "_", 0 }) );
	::std::printf( "read(0) = %zi\n", (usize) f.read({ nullptr, 0 }) );
	::std::printf( "size    = %zi\n", (usize) f.size() );
	::std::printf( "pos(0)  = %zi\n", (usize) f.pos( 0 ) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );

	::std::printf( "write(5)= %zi\n", (usize) f.write({ "Asdf", 5 }) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );

	::std::printf( "pos(4)  = %zi\n", (usize) f.pos( 4 ) );
	::std::printf( "write(3)= %zi\n", (usize) f.write({ "Qw\0", 3 }) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );

	::std::printf( "pos(8)  = %zi\n", (usize) f.pos( 8 ) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );
	::std::printf( "write(2)= %zi\n", (usize) f.write({ "Ok", 2 }) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );

	::std::printf( "pos(8)  = %zi\n", (usize) f.pos( 2 ) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );

	::std::printf( "write(1)= %zi\n", (usize) f.write({ "!", 1 }) );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );

	char buff[32] = {};
	::std::printf( "read(3) = %zi\n", (usize) f.read({ buff, 3 }) );
	::std::printf( "buff    = %s\n", buff );
	::std::printf( "pos     = %zi\n", (usize) f.pos() );
	::std::printf( "size    = %zi\n", (usize) f.size() );
	::std::printf( "flush   = %s\n", f.flush()?"true":"false" );
}

} // namespace lib::stream::impl
