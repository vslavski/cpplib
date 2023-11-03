/* File: /lib/debug/hexdump.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__hexdump__hpp
#define CPPLIB__lib__debug__hexdump__hpp

#include <cstdio>

#include <algorithm>
#include <limits>

#include "../../lib/types.hpp"
#include "../../lib/literals.hpp"
#include "../../lib/flags.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/data/buffer.hpp"
#include "../../lib/utils/cp437.hpp"
#include "../../lib/utils/string.hpp"

namespace lib::debug {

// DECLARATION lib::debug::hexdump

/** @brief Hexdump printer
 *  @note The idea of using CP437 codepage was taken from [Blinkenlights](https://github.com/jart/blink) project,
 *  developed by Justine Alexandra Roberts Tunney.
 *  @see https://github.com/jart/blink
 */

struct hexdump {
	static constexpr usize COLUMNS = 16;

	enum Options
		{ NONE		= 0x00
		, ASCII		= 0x01
		, CP437		= 0x02
		, OFFSET	= 0x04
		, ADDRESS	= 0x08
		, STDOUT	= 0x10
		, STDERR	= 0x20
		, internal	= CP437 | ADDRESS | STDERR
		, external	= CP437 | OFFSET | STDOUT
	};

	hexdump( const data::cbuffer_t & buffer, Options options_ = internal, usize offset = 0 ) noexcept;

	template< class T >
	hexdump( const T * object, Options options = internal ) noexcept
		: hexdump { {(const u8*) object, sizeof(*object)}, options }
	{}

	hexdump( const hexdump & ) = delete;
	hexdump( hexdump && ) = delete;

private:
	static const char * select_prefix( usize size, const Flag<Options> & options ) noexcept;
};

// INLINES lib::debug::hexdump

inline hexdump::hexdump( const data::cbuffer_t & buffer, Options options_/* = internal*/, usize offset/* = 0*/ ) noexcept {
	Flag<Options> options {options_};
	auto * f = options.check( STDOUT ) ? stdout : stderr;
	const char * prefix = select_prefix( buffer.size(), options );
	for ( auto i = 0_sz; i < buffer.size(); i += COLUMNS ) {
		const auto prefix_value = options.check( ADDRESS ) ? (usize) &buffer[i] : i + offset;
		::std::fprintf( f, prefix, prefix_value );
		const auto count = ::std::min( buffer.size() - i, COLUMNS );
		const auto & dump = hex( { &buffer[i], count }, " " );
		::std::fprintf( f, "%s", dump.c_str() );
		if ( not options.check( ASCII ) and not options.check( CP437 ) ) {
			::std::fputc( '\n', f );
			continue;
		}
		for ( auto j = count; j < COLUMNS; ++j )
			::std::fputs( "   "/*hex + space*/, f );
		::std::fputs( " | ", f );
		for ( auto j = 0_sz; j < count; ++j ) {
			if ( options.check( ASCII ) ) {
				const char c = (char) buffer[ i + j ];
				::std::fputc( is_ascii_graphical( c ) ? c : ' ', f );
			} else/* if ( options.check( CP437 ) )*/ {
				const auto s = utils::cp437( buffer[ i + j ] );
				::std::fputs( s./*c_str*/data(), f );
			}
		}
		for ( auto j = count; j < COLUMNS; ++j )
			::std::fputs( "  "/*char + space*/, f );
		::std::fputc( '\n', f );
	}
}

/*static */inline const char * hexdump::select_prefix( usize size, const Flag<Options> & options ) noexcept {
	if ( options.check( ADDRESS ) ) {
		switch ( sizeof(usize) ) {
		case sizeof(u32):		return "%08x | ";
		case sizeof(u64):		return "%016x | ";
		default:				return "ERROR | ";
		}
	}
	if ( not options.check( OFFSET ) )
		return "";
	if ( size <= ::std::numeric_limits<u8>::max() )		return "%02x | ";
	if ( size <= ::std::numeric_limits<u16>::max() )	return "%04x | ";
	if ( size <= ::std::numeric_limits<u32>::max() )	return "%08x | ";
	if ( size <= ::std::numeric_limits<u64>::max() )	return "%016x | ";
	return "ERROR | ";
}

} // namespace lib::debug

#endif // CPPLIB__lib__debug__hexdump__hpp
