/* File: /lib/impl/codec/base64.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstring>

#include <algorithm>

#include <cpp/lib_debug>

#include "../../../lib/literals.hpp"
#include "../../../lib/system/error.hpp"

#include "./base64.hpp"

namespace lib::codec::impl {

// IMPLEMENTATION lib::codec::impl::base64::Word

namespace { namespace WORD {
static constexpr u8 FIRST_OFFSET_L = 2;
static constexpr u8 SECOND_OFFSET_L = 4;
static constexpr u8 SECOND_OFFSET_R = 4;
static constexpr u8 THIRD_OFFSET_L = 6;
static constexpr u8 THIRD_OFFSET_R = 2;
} } // namespace WORD

inline constexpr base64::Word::Word( u8 first, u8 second, u8 third, u8 fourth ) noexcept
	: data{ (u8) ( first << WORD::FIRST_OFFSET_L bitor second >> WORD::SECOND_OFFSET_R )
		, (u8) ( second << WORD::SECOND_OFFSET_L bitor third >> WORD::THIRD_OFFSET_R )
		, (u8) ( third << WORD::THIRD_OFFSET_L bitor fourth ) }
{}

inline constexpr u8 base64::Word::first() const noexcept {
	return ( BASE - 1 )
		bitand ( data[0] >> WORD::FIRST_OFFSET_L );
}

inline constexpr u8 base64::Word::second() const noexcept {
	return ( BASE - 1 )
		bitand (u8) ( data[0] << WORD::SECOND_OFFSET_R bitor data[1] >> WORD::SECOND_OFFSET_L );
}

inline constexpr u8 base64::Word::third() const noexcept {
	return ( BASE - 1 )
		bitand (u8) ( data[1] << WORD::THIRD_OFFSET_R bitor data[2] >> WORD::THIRD_OFFSET_L );
}

inline constexpr u8 base64::Word::fourth() const noexcept {
	return ( BASE - 1 )
		bitand ( data[2] );
}

inline constexpr u8 base64::Word::first( u8 value ) noexcept {
	value &= ( BASE - 1 );
	data[0] = (u8) ( value << WORD::FIRST_OFFSET_L bitor second() >> WORD::SECOND_OFFSET_R );
	return value;
}

inline constexpr u8 base64::Word::second( u8 value ) noexcept {
	value &= ( BASE - 1 );
	data[0] = (u8) ( first() << WORD::FIRST_OFFSET_L bitor value >> WORD::SECOND_OFFSET_R );
	data[1] = (u8) ( value << WORD::SECOND_OFFSET_L bitor third() >> WORD::THIRD_OFFSET_R );
	return value;
}

inline constexpr u8 base64::Word::third( u8 value ) noexcept {
	value &= ( BASE - 1 );
	data[1] = (u8) ( second() << WORD::SECOND_OFFSET_L bitor value >> WORD::THIRD_OFFSET_R );
	data[2] = (u8) ( value << WORD::THIRD_OFFSET_L bitor fourth() );
	return value;
}

inline constexpr u8 base64::Word::fourth( u8 value ) noexcept {
	value &= ( BASE - 1 );
	data[2] = (u8) ( third() << WORD::THIRD_OFFSET_L bitor value );
	return value;
}

// IMPLEMENTATION lib::codec::impl::base64

namespace {
static constexpr char ALPHABET[ base64::BASE ] =
	{ 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P'
	, 'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f'
	, 'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v'
	, 'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
	};
static constexpr char PAD = '=';
static constexpr u8 BAD_IDX = 0xFF;
inline static constexpr u8 AlphaToIdx( char value ) noexcept {
	const auto * it = ::std::find( ALPHABET, ALPHABET + base64::BASE, value );
	return it != ALPHABET + base64::BASE ? (u8) ( it - ALPHABET ) : BAD_IDX;
}
} // namespace

bool base64::IsValid( const data::cbuffer_t & coded ) {
	if ( coded.size() % WORD_SIZE != 0 )
		return false;
	for ( auto i = 0_sz; i + WORD_SIZE <= coded.size(); i += WORD_SIZE ) {
		const u8 first = AlphaToIdx( (char) coded[ i + 0 ] );
		const u8 second = AlphaToIdx( (char) coded[ i + 1 ] );
		if ( first == BAD_IDX or second == BAD_IDX )
			return false;
		const char third_char = (char) coded[ i + 2 ];
		const char fourth_char = (char) coded[ i + 3 ];
		const u8 third = AlphaToIdx( third_char );
		const u8 fourth = AlphaToIdx( fourth_char );
		if ( third == BAD_IDX )
			return third_char == PAD and fourth_char == PAD and i + WORD_SIZE == coded.size();
		if ( fourth == BAD_IDX )
			return fourth_char == PAD and i + WORD_SIZE == coded.size();
	}
	return true;
}

base64::base64( const data::cbuffer_t & buffer, Mode mode_/* = Mode::ENCODE*/ )
	: mode_{ mode_ }
{
	CPP_ASSERT( mode_ != Mode::UNDEFINED );
	if ( mode_ == Mode::ENCODE )
		CPP_UNUSED( encode( buffer ) );
	else if ( mode_ == Mode::DECODE )
		CPP_UNUSED( decode( buffer ) );
}

// IMPLEMENTATION lib::codec::impl::base64: lib::data::codec_t

void base64::reset( Mode mode/* = Mode::UNDEFINED*/ )/* override*/ {
	error_.clear();
	mode_ = mode;
	word_idx = 0;
	word = {};
	buffer_.clear();
}

// IMPLEMENTATION lib::codec::impl::base64: lib::data::encoder_t, lib::data::decoder_t

data::result_t base64::encode( const data::cbuffer_t & plain )/* override*/ {
	if ( mode_ != Mode::ENCODE )
		reset( Mode::ENCODE );
	if ( plain.empty() )
		return 0_sz;

	const auto prev_size = buffer_.size();
	const auto cipher_size = ( plain.size() / WORD_BYTES + 1 ) * WORD_SIZE;
	buffer_.reserve( prev_size + cipher_size );

	for ( const u8 p : plain ) {
		word.data[ word_idx++ ] = p;
		if ( word_idx == WORD_BYTES ) {
			buffer_.emplace_back( ALPHABET[ word.first() ] );
			buffer_.emplace_back( ALPHABET[ word.second() ] );
			buffer_.emplace_back( ALPHABET[ word.third() ] );
			buffer_.emplace_back( ALPHABET[ word.fourth() ] );
			word_idx = 0;
			word = Word{};
		}
	}

	return plain.size();
}

data::result_t base64::decode( const data::cbuffer_t & coded )/* override*/ {
	if ( mode_ != Mode::DECODE )
		reset( Mode::DECODE );
	if ( coded.empty() )
		return 0_sz;

	/// @todo Optimize.

	CPP_ASSERT( coded.size() % WORD_SIZE == 0 /**< @todo Add partial data support. */ );

	const auto prev_size = buffer_.size();
	const auto plain_size = coded.size() / WORD_SIZE * WORD_BYTES;
	buffer_.reserve( prev_size + plain_size );

	for ( auto i = 0_sz; i + WORD_SIZE <= coded.size(); i += WORD_SIZE ) {
		const u8 first = AlphaToIdx( (char) coded[ i + 0 ] );
		const u8 second = AlphaToIdx( (char) coded[ i + 1 ] );
		const char third_char = (char) coded[ i + 2 ];
		const char fourth_char = (char) coded[ i + 3 ];
		const u8 third = AlphaToIdx( third_char );
		const u8 fourth = AlphaToIdx( fourth_char );
		if ( first == BAD_IDX or second == BAD_IDX
			or ( third_char != PAD and third == BAD_IDX )
			or ( fourth_char != PAD and fourth == BAD_IDX )
			)
			return error_ = make_error_bad_data();
		word = Word{ first, second, 0, 0 };
		buffer_.emplace_back( word.data[0] );
		if ( third_char != PAD ) {
			word.third( third );
			buffer_.emplace_back( word.data[1] );
			if ( fourth_char != PAD ) {
				word.fourth( fourth );
				buffer_.emplace_back( word.data[2] );
			}
		}
	}

	return coded.size();
}

// IMPLEMENTATION lib::codec::impl::base64: lib::data::rstream_t, lib::data::wstream_t

data::result_t base64::read( const data::buffer_t & buffer )/* override*/ {
	const auto count = ::std::min( buffer.size(), buffer_.size() );
	::std::memcpy( buffer.data(), buffer_.data(), count );
	return count;
}

data::cbuffer_t base64::read_cbuffer( bool flush )/* override*/ {
	if ( flush and not read_flush() )
		return {};
	return buffer_;
}

bool base64::read_flush()/* override*/ {
	if ( mode_ != Mode::ENCODE )
		return mode_ != Mode::UNDEFINED;
	if ( word_idx == 0 )
		return true;

	buffer_.emplace_back( ALPHABET[ word.first() ] );
	buffer_.emplace_back( ALPHABET[ word.second() ] );
	if ( word_idx == 2 )
		buffer_.emplace_back( ALPHABET[ word.third() ] );
	else// if ( word_idx == 1 )
		buffer_.emplace_back( PAD );
	buffer_.emplace_back( PAD );

	word_idx = 0;
	return true;
}

} // namespace lib::codec::impl
