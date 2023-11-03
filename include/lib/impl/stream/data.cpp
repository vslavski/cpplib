/* File: /lib/impl/stream/data.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstring>

#include <algorithm>

#include <cpp/lib_debug>

#include "./data.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::data

data::data( const lib::data::cbuffer_t & read_buff )
	: read_buff {read_buff}
{}

data::data( const lib::data::buffer_t & write_buff )
	: read_buff {write_buff}
	, write_buff {write_buff}
{}

void data::reset( const lib::data::cbuffer_t & read_buff_/* = {}*/ ) {
	read_buff = read_buff_;
	write_buff = {};
	read_pos_ = write_pos_ = 0;
}

void data::reset( const lib::data::buffer_t & write_buff_ ) {
	read_buff = write_buff_;
	write_buff = write_buff_;
	read_pos_ = write_pos_ = 0;
}

// IMPLEMENTATION lib::stream::impl::data: lib::data::rstream_t

lib::data::result_t data::read( const lib::data::buffer_t & buffer )/* override*/ {
	const auto & peek_count = peek( buffer );
	CPP_ASSERT( peek_count.success() );
	read_pos_ += peek_count;
	return peek_count;
}

lib::data::result_t data::peek( const lib::data::buffer_t & buffer )/* override*/ {
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	const auto count = ::std::min( buffer.size(), read_size_.value() );
	::std::memcpy( buffer.data(), &read_buff[read_pos_], count );
	return count;
}

bool data::read_flush()/* override*/ {
	read_pos_ = 0;
	return true;
}

lib::data::result_t data::read_size()/* override*/ {
	return read_buff.size() - read_pos_;
}

lib::data::result_t data::read_pos( usize position )/* override*/ {
	return read_pos_ = ::std::min( position, read_buff.size() );
}

lib::data::result_t data::read_pos()/* override*/ {
	return read_pos_;
}

lib::data::cbuffer_t data::read_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	return {&read_buff[read_pos_], read_size_.value()};
}

// IMPLEMENTATION lib::stream::impl::data: lib::data::wstream_t

lib::data::result_t data::write( const lib::data::cbuffer_t & buffer )/* override*/ {
	const auto & write_size_ = write_size();
	CPP_ASSERT( write_size_.success() );
	const auto count = ::std::min( buffer.size(), write_size_.value() );
	::std::memcpy( &write_buff[write_pos_], buffer.data(), count );
	write_pos_ += count;
	return count;
}

bool data::write_flush()/* override*/ {
	write_pos_ = 0;
	return true;
}

lib::data::result_t data::write_size()/* override*/ {
	return write_buff.size() - write_pos_;
}

lib::data::result_t data::write_pos( usize position )/* override*/ {
	return write_pos_ = ::std::min( position, write_buff.size() );
}

lib::data::result_t data::write_pos()/* override*/ {
	return write_pos_;
}

lib::data::buffer_t data::write_buffer( bool flush )/* override*/ {
	if ( flush and not write_flush() )
		return {};
	const auto & write_size_ = write_size();
	CPP_ASSERT( write_size_.success() );
	return {&write_buff[write_pos_], write_size_.value()};
}

// IMPLEMENTATION lib::stream::impl::data: lib::data::rwstream_t

bool data::flush()/* override*/ {
	return read_flush() and write_flush();
}

lib::data::result_t data::size()/* override*/ {
	return readonly() ? read_size() : write_size();
}

lib::data::result_t data::pos( usize position )/* override*/ {
	return readonly() ? read_pos( position ) : write_pos( position );
}

lib::data::result_t data::pos()/* override*/ {
	return readonly() ? read_pos() : write_pos();
}

} // namespace lib::stream::impl
