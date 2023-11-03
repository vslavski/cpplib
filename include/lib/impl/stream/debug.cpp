/* File: /lib/impl/stream/debug.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstdlib>

#include <algorithm>

#include "../../../lib/system/error.hpp"

#include "./debug.hpp"

namespace lib::stream::impl {

// IMPLEMENTATION lib::stream::impl::debug

debug::debug()
{ reset(); }

debug::debug( data::rwstream_t * stream, usize min_rw, usize max_rw )
{ reset( stream, min_rw, max_rw ); }

void debug::reset() {
	stream = nullptr;
	min_rw = 0;
	max_rw = ::std::numeric_limits<decltype(max_rw)>::max();
	next_read = ::std::numeric_limits<decltype(next_read)>::max();
	next_write = ::std::numeric_limits<decltype(next_write)>::max();
}

void debug::reset( data::rwstream_t * stream_, usize min_rw_, usize max_rw_ ) {
	stream = stream_;
	min_rw = min_rw_;
	max_rw = max_rw_;
	shuffle();
}

void debug::shuffle() {
	next_read = min_rw + (usize) ::std::rand() % ( max_rw - min_rw );
	next_write = min_rw + (usize) ::std::rand() % ( max_rw - min_rw );
}

// IMPLEMENTATION lib::stream::impl::debug: lib::data::rstream_t

data::result_t debug::read( const data::buffer_t & buffer )/* override*/ {
	if ( stream == nullptr )
		return make_error_not_configured();
	const auto count = ::std::min( next_read, buffer.size() );
	shuffle();
	return stream->read({ buffer.data(), count });
}

data::result_t debug::peek( const data::buffer_t & buffer )/* override*/ {
	if ( stream == nullptr )
		return make_error_not_configured();
	const auto count = ::std::min( next_read, buffer.size() );
	shuffle();
	return stream->peek({ buffer.data(), count });
}

bool debug::read_flush()/* override*/ {
	return stream != nullptr ? stream->read_flush() : false;
}

data::result_t debug::read_size()/* override*/ {
	if ( stream == nullptr )
		return make_error_not_configured();
	auto result = stream->read_size();
	if ( result.success() )
		result = ::std::min( next_read, result.value() );
	return result;
}

::std::error_condition debug::read_error() const/* override*/ {
	return stream != nullptr ? stream->read_error() : make_error_no_error();
}

// IMPLEMENTATION lib::stream::impl::debug: lib::data::wstream_t

data::result_t debug::write( const data::cbuffer_t & buffer )/* override*/ {
	if ( stream == nullptr )
		return make_error_not_configured();
	const auto count = ::std::min( next_write, buffer.size() );
	shuffle();
	return stream->write({ buffer.data(), count });
}

bool debug::write_flush()/* override*/ {
	return stream != nullptr ? stream->write_flush() : false;
}

data::result_t debug::write_size()/* override*/ {
	if ( stream == nullptr )
		return make_error_not_configured();
	auto result = stream->write_size();
	if ( result.success() )
		result = ::std::min( next_write, result.value() );
	return result;
}

::std::error_condition debug::write_error() const/* override*/ {
	return stream != nullptr ? stream->write_error() : make_error_no_error();
}

// IMPLEMENTATION lib::stream::impl::debug: lib::data::rwstream_t

bool debug::flush()/* override*/ {
	return stream != nullptr ? stream->flush() : false;
}

data::result_t debug::size()/* override*/ {
	return make_error_not_implemented();
}

::std::error_condition debug::error() const/* override*/ {
	return stream != nullptr ? stream->error() : make_error_no_error();
}

} // namespace lib::stream::impl
