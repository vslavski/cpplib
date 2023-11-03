/* File: /lib/impl/stream/fifo.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstring>

#include <limits>
#include <algorithm>

#include <cpp/lib_debug>

#include "./fifo.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::fifo: lib::data::rstream_t

data::result_t fifo::read( const data::buffer_t & buffer )/* override*/ {
	const auto & peek_count = peek( buffer );
	CPP_ASSERT( peek_count.success() );
	read_pos_ += peek_count;
	return peek_count;
}

data::result_t fifo::peek( const data::buffer_t & buffer )/* override*/ {
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	const auto count = ::std::min( buffer.size(), read_size_.value() );
	::memcpy( buffer.data(), &buffer_[read_pos_], count );
	return count;
}

bool fifo::read_flush()/* override*/ {
	buffer_.erase( buffer_.begin(), buffer_.begin() + (isize) read_pos_ );
	read_pos_ = 0;
	return true;
}

data::result_t fifo::read_size()/* override*/ {
	return buffer_.size() - read_pos_;
}

data::result_t fifo::read_pos( usize position )/* override*/ {
	return read_pos_ = ::std::min( position, buffer_.size() );
}

data::result_t fifo::read_pos()/* override*/ {
	return read_pos_;
}

data::cbuffer_t fifo::read_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	const auto & read_size_ = read_size();
	CPP_ASSERT( read_size_.success() );
	return {&buffer_[read_pos_], read_size_.value()};
}

// IMPLEMENTATION lib::stream::impl::fifo: lib::data::wstream_t

data::result_t fifo::write( const data::cbuffer_t & buffer )/* override*/ {
	const auto & write_size_ = write_size();
	CPP_ASSERT( write_size_.success() );
	const auto count = ::std::min( buffer.size(), write_size_.value() );
	const auto write_pos_ = buffer_.size();
	buffer_.resize( buffer_.size() + count );
	::memcpy( &buffer_[write_pos_], buffer.data(), buffer.size() );
	return count;
}

bool fifo::write_flush()/* override*/ {
	buffer_.clear();
	read_pos_ = 0;
	return true;
}

data::result_t fifo::write_size()/* override*/ {
	return ::std::numeric_limits<usize>::max() - buffer_.size();
}

data::result_t fifo::write_pos( usize position )/* override*/ {
	buffer_.resize( position );
	return position;
}

data::result_t fifo::write_pos()/* override*/ {
	return buffer_.size();
}

data::buffer_t fifo::write_buffer( bool flush )/* override*/ {
	if ( flush and not write_flush() )
		return {};
	return buffer_;
}

// IMPLEMENTATION lib::stream::impl::fifo: lib::data::rwstream_t

bool fifo::flush()/* override*/ {
	return write_flush()/* and read_flush()*/;
}

data::result_t fifo::size()/* override*/ {
	return buffer_.size();;
}

} // namespace lib::stream::impl
