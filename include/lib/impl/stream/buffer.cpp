/* File: /lib/impl/stream/buffer.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstring>

#include <algorithm>
#include <utility>

#include <cpp/lib_debug>

#include "../../../lib/system/error.hpp"

#include "./buffer.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::buffer

buffer::buffer( usize size )
	: stream {nullptr}
	, read_pos_ {0}
{
	read_buff.reserve( size );
	write_buff.reserve( size );
}

void buffer::reset( data::rwstream_t * stream_/* = nullptr*/ ) {
	stream = stream_;
	read_pos_ = 0;
	read_buff.clear();
	write_buff.clear();
}

bool buffer::empty() const {
	return stream == nullptr;
}

// IMPLEMENTATION lib::stream::impl::buffer: lib::data::rstream_t

data::result_t buffer::read( const data::buffer_t & buffer )/* override*/ {
	const auto & peek_count = peek( buffer );
	if ( peek_count.success() )
		read_pos_ += peek_count;
	return peek_count;
}

data::result_t buffer::peek( const data::buffer_t & buffer )/* override*/ {
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	const auto count = ::std::min( buffer.size(), read_size_.value() );
	::memcpy( buffer.data(), &read_buff[read_pos_], count );
	return count;
}

bool buffer::read_flush()/* override*/ {
	if ( empty() )
		return true;
	read_buff.erase( read_buff.begin(), read_buff.begin() + (isize) read_pos_ );
	read_pos_ = 0;
	const auto current_size = read_buff.size();
	const auto available_size = read_buff.capacity() - current_size;
	auto read_size_ = stream->read_size();
	if ( read_size_.failed() )
		return false;
	const auto count = ::std::min( read_size_.value(), available_size );
	if ( count == 0 )
		return true;
	read_buff.resize( read_buff.capacity() );
	read_size_ = stream->read({ &read_buff[current_size], count });
	if ( read_size_.failed() )
		return false;
	read_buff.resize( current_size + read_size_.value() );
	return true;
}

data::result_t buffer::read_size()/* override*/ {
	return read_buff.size() - read_pos_;
}

data::result_t buffer::read_pos( usize position )/* override*/ {
	return read_pos_ = ::std::min( position, read_buff.size() );
}

data::result_t buffer::read_pos()/* override*/ {
	return read_pos_;
}

data::cbuffer_t buffer::read_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	return {&read_buff[read_pos_], read_size_.value()};
}

// IMPLEMENTATION lib::stream::impl::buffer: lib::data::wstream_t

data::result_t buffer::write( const data::cbuffer_t & buffer )/* override*/ {
	const auto & write_size_ = write_size();
	CPP_ASSERT( write_size_.success() );
	const auto count = ::std::min( buffer.size(), write_size_.value() );
	const auto write_pos_ = write_buff.size();
	write_buff.resize( write_buff.size() + count );
	::memcpy( &write_buff[write_pos_], buffer.data(), count );
	return count;
}

bool buffer::write_flush()/* override*/ {
	if ( empty() or write_buff.empty() )
		return true;
	const auto & write_size_ = stream->write( write_buff );
	if ( write_size_.failed() )
		return false;
	write_buff.erase( write_buff.begin(), write_buff.begin() + (isize) write_size_.value() );
	return true;
}

data::result_t buffer::write_size()/* override*/ {
	return write_buff.capacity() - write_buff.size();
}

data::result_t buffer::write_pos( usize position ) {
	if ( empty() )
		return make_error_not_configured();
	write_buff.resize( position );
	return position;
}

data::result_t buffer::write_pos() {
	return write_buff.size();
}

data::buffer_t buffer::write_buffer( bool flush ) {
	if ( flush and not write_flush() )
		return {};
	return write_buff;
}

// IMPLEMENTATION lib::stream::impl::buffer: lib::data::rwstream_t

::std::error_condition buffer::error() const/* override*/ {
	return empty() ? make_error_no_error() : stream->error();
}

} // namespace lib::stream::impl
