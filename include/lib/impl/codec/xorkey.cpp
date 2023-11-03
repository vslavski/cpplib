/* File: /lib/impl/codec/xorkey.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <algorithm>
#include <iterator>
#include <memory>

#include "../../../lib/literals.hpp"
#include "../../../lib/system/error.hpp"

#include "./xorkey.hpp"

namespace lib::codec::impl {

// IMPLEMENTATION lib::codec::impl::xorkey

data::result_t xorkey::Process( const data::buffer_t & buffer, const data::cbuffer_t & key, usize key_idx/* = 0*/ ) {
	if ( key.empty() )
		return make_error_empty_data();
	if ( buffer.empty() )
		return 0_sz;
	for ( auto & value : buffer ) {
		value ^= key[key_idx];
		++key_idx;
		key_idx %= key.size();
	}
	return buffer.size();
}

void xorkey::reset( const data::cbuffer_t & key_buff/* = {}*/ ) {
	key_idx = 0;
	key_ = {key_buff.begin(),key_buff.end()};
	buffer_.clear();
}

// IMPLEMENTATION lib::codec::impl::xorkey: lib::data::rstream_t, lib::data::wstream_t

data::result_t xorkey::read( const data::buffer_t & buffer )/* override*/ {
	const auto count = ::std::min( buffer.size(), buffer_.size() );
	::std::copy_n( buffer_.begin(), count, buffer.begin() );
	return count;
}

data::result_t xorkey::write( const data::cbuffer_t & buffer )/* override*/ {
	if ( key_.empty() )
		return make_error_empty_data();
	const auto offset = buffer_.size();
	buffer_.resize( offset + buffer.size() );
	auto copy_dst = buffer_.begin();
	::std::advance( copy_dst, offset );
	::std::copy( buffer.begin(), buffer.end(), copy_dst );
	return Process( {::std::to_address( copy_dst ), buffer.size()}, key_, key_idx );
}

} // namespace lib::codec::impl
