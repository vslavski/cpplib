/* File: /lib/impl/hash/sha1.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <algorithm>
#include <bit>
#include <limits>

#include <cpp/lib_bit>
#include <cpp/lib_debug>

#include "../../../lib/literals.hpp"
#include "../../../lib/system/error.hpp"

#include "./sha1.hpp"

namespace lib::hash::impl {

// INLINES lib::hash::impl::sha1

inline constexpr u32 sha1::f( u32 t, u32 B, u32 C, u32 D ) noexcept {
	if ( t < 20 )				return ( B bitand C ) bitor ( ( compl B ) bitand D );
	if ( t < 40 )				return B xor C xor D;
	if ( t < 60 )				return ( B bitand C ) bitor ( B bitand D ) bitor ( C bitand D );
	/*CPP_ASSERT( t < 80 );*/	return B xor C xor D;
}

inline constexpr u32 sha1::K( u32 t ) noexcept {
	if ( t < 20 )				return 0x5A827999;
	if ( t < 40 )				return 0x6ED9EBA1;
	if ( t < 60 )				return 0x8F1BBCDC;
	/*CPP_ASSERT( t < 80 );*/	return 0xCA62C1D6;
}

// IMPLEMENTATION lib::hash::impl::sha1

sha1::sha1() {
	reset();
}
sha1::sha1( const data::cbuffer_t & buffer ) {
	reset();
	CPP_UNUSED( update( buffer ) );
}

void sha1::pack_hash() {
	const data::buffer_t H_buff{ H };
	::std::copy( hash_buff.begin(), hash_buff.end(), H_buff.begin() );
	for ( auto & h : H )
		h = ::cpp::from_big_endian( h );
}

void sha1::unpack_hash() {
	for ( auto & h : H )
		h = ::cpp::to_big_endian( h );
	const data::cbuffer_t H_buff{ H };
	::std::copy( H_buff.begin(), H_buff.end(), hash_buff.begin() );
	for ( auto & h : H )
		h = ::cpp::from_big_endian( h );
}

void sha1::process_block() {
	Word W[80];
	const data::buffer_t W_buff{ W, BLOCK_BYTES };
	::std::copy( block_buff.begin(), block_buff.end(), W_buff.begin() );
	for ( auto i = 0_sz; i < BLOCK_WORDS; ++i )
		W[i] = ::cpp::from_big_endian( W[i] );
	for ( auto t = 16_sz; t < 80; ++t )
		W[t] = ::std::rotl( W[t-3] xor W[t-8] xor W[t-14] xor W[t-16], 1 );

	Word A = H[0], B = H[1], C = H[2], D = H[3], E = H[4];
	for ( u32 t = 0; t < 80; ++t ) {
		const auto TEMP = ::std::rotl( A, 5 ) + f( t, B, C, D ) + E + W[t] + K( t );
		E = D; D = C; C = ::std::rotl( B, 30 ); B = A; A = TEMP bitand 0xFFFFFFFF;
	}
	H[0] += A; H[1] += B; H[2] += C; H[3] += D; H[4] += E;
}

usize sha1::flush_bit( usize offset ) {
	block_buff[offset] = 0b10000000;
	++offset;
	if ( offset == BLOCK_BYTES )
		process_block();
	return offset % BLOCK_BYTES;
}

void sha1::flush_pad( usize offset ) {
	auto remain = BLOCK_BYTES - offset;
	if ( remain < LENGTH_BYTES ) {
		::std::fill_n( block_buff.begin() + (isize) offset, remain, (u8) 0/*_u8*/ );
		process_block();
		offset = 0;
	}
	remain = BLOCK_BYTES - LENGTH_BYTES - offset;
	::std::fill_n( block_buff.begin() + (isize) offset, remain, (u8) 0/*_u8*/ );
}

void sha1::flush_length() {
	const auto length_bits = ::cpp::to_big_endian( length * 8/*bits*/ );
	const auto offset = BLOCK_BYTES - LENGTH_BYTES;
	const data::cbuffer_t length_buff{ &length_bits, LENGTH_BYTES };
	::std::copy( length_buff.begin(), length_buff.end(), block_buff.begin() + (isize) offset );
	process_block();
}

// IMPLEMENTATION lib::hash::impl::sha1: lib::data::hash_t

void sha1::reset()/* override*/ {
	flushed = false;
	length = 0;
	hash_buff = ::std::to_array<u8>({ 0x67, 0x45, 0x23, 0x01, 0xEF, 0xCD, 0xAB, 0x89
			, 0x98, 0xBA, 0xDC, 0xFE, 0x10, 0x32, 0x54, 0x76, 0xC3, 0xD2, 0xE1, 0xF0 });
	pack_hash();
}

// IMPLEMENTATION lib::hash::impl::sha1: lib::data::rstream_t, lib::data::wstream_t

data::result_t sha1::read( const data::buffer_t & buffer )/* override*/ {
	const auto count = ::std::min( buffer.size(), hash_buff.size() );
	::std::copy_n( hash_buff.begin(), count, buffer.begin() );
	return count;
}

bool sha1::read_flush()/* override*/ {
	if ( flushed )
		return true;

	usize offset = length % BLOCK_BYTES;
	offset = flush_bit( offset );
	flush_pad( offset );
	flush_length();

	unpack_hash();
	return flushed = true;
}

data::cbuffer_t sha1::read_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	return hash_buff;
}

data::result_t sha1::write( const data::cbuffer_t & buffer )/* override*/ {
	if ( flushed )
		reset();

	static constexpr Length MAX_LENGTH_BYTES = ::std::numeric_limits<Length>::max() / 8/*as bit-length is stored*/;
	if ( length + buffer.size() > MAX_LENGTH_BYTES )
		return make_error_data_overflow();
	const auto count = (usize) ::std::min<Length>( MAX_LENGTH_BYTES - length, buffer.size() );

	auto i = 0_sz;
	const auto offset = (usize) length % BLOCK_BYTES;
	const auto remain = BLOCK_BYTES - offset;
	if ( remain < count ) {
		::std::copy_n( buffer.begin() + (isize) i, remain, block_buff.begin() + (isize) offset );
		process_block();
		i += remain;
	}

	while ( i + BLOCK_BYTES <= count ) {
		::std::copy_n( buffer.begin() + (isize) i, block_buff.size(), block_buff.begin() );
		process_block();
		i += BLOCK_BYTES;
	}

	if ( i < count )
		::std::copy_n( buffer.begin() + (isize) i, count - i, block_buff.data() );

	unpack_hash();

	length += count;
	return count;
}

data::buffer_t sha1::write_buffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	return block_buff;
}

data::cbuffer_t sha1::write_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	return block_buff;
}

} // namespace lib::hash::impl
