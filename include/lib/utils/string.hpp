/* File: /lib/utils/string.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__string__hpp
#define CPPLIB__lib__utils__string__hpp

#include <cctype>

#include <string>
#include <vector>

#include <cpp/lib_debug>

#include "../../lib/types.hpp"
#include "../../lib/literals.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/data/buffer.hpp"

/// @todo Support all char types (c8, c16, c32).

namespace lib {

// DECLARATION lib::str<>(), lib::str()

template< class T >
requires requires( const T & t ) { ::std::to_string( t ); }
inline ::std::string str( const T & object ) {
	return ::std::to_string( object );
}

inline ::std::string str( const char * object ) {
	return ::std::string {object};
}

inline ::std::string str( const cstring & str ) {
	return ::std::string {str.begin(), str.end()};
}

inline constexpr cstring str( bool value ) noexcept {
	return value ? "true" : "false";
}

// DECLARATION lib::split()

inline ::std::vector<cstring>
split( const cstring & str, const char * separators, const char * group = "" ) {
	CPP_ASSERT( separators != nullptr );
	CPP_ASSERT( group != nullptr );
	::std::vector<cstring> result;
	auto str_begin = 0_sz;
	while ( str_begin < str.size() ) {
		str_begin = str.find_first_not_of( separators, str_begin );
		if ( str_begin == cstring::npos )
			break;
		const char * g = group;
		for ( ; *g != '\0'; ++g )
			if ( *g == str[ str_begin ] )
				break;
		usize str_end;
		if ( *g != '\0' and str_begin + 1 < str.size() )
			str_end = str.find_first_of( *g, ++str_begin );
		else
			str_end = str.find_first_of( separators, str_begin );
		if ( str_end == cstring::npos )
			str_end = str.size();
		result.emplace_back( str.substr( str_begin, str_end - str_begin ) );
		str_begin = str_end + 1;
	}
	return result;
}

// DECLARATION lib::to_lower(), lib::to_upper()

inline /*constexpr */char to_lower( const char c ) {
	return (char)::std::tolower( c );
}

inline /*constexpr */char to_upper( const char c ) {
	return (char)::std::toupper( c );
}

inline ::std::string to_lower( const cstring & str_ ) {
	auto result = str( str_ );
	for ( auto & c : result ) c = to_lower( c );
	return result;
}

inline ::std::string to_upper( const cstring & str_ ) {
	auto result = str( str_ );
	for ( auto & c : result ) c = to_upper( c );
	return result;
}

// DECLARATION lib::is_lower(), lib::is_upper()

inline /*constexpr */bool is_lower( const char c ) {
	return ::std::islower( c ) != 0;
}

inline /*constexpr */bool is_upper( const char c ) {
	return ::std::isupper( c ) != 0;
}

inline constexpr bool is_lower( const cstring & str ) {
	for ( const auto c : str )
		if ( not is_lower( c ) )
			return false;
	return true;
}

inline constexpr bool is_upper( const cstring & str ) {
	for ( const auto c : str )
		if ( not is_upper( c ) )
			return false;
	return true;
}

// DECLARATION lib::is_digit()
/// @todo <cctype> ::std::isdigit()

inline constexpr bool is_digit( const char c ) {
	return '0' <= c and c <= '9';
}

inline constexpr bool is_digit( const cstring & str ) {
	if ( str.empty() )
		return false;
	for ( const auto c : str )
		if ( not is_digit( c ) )
			return false;
	return true;
}

inline constexpr bool is_digit( const char * str ) {
	return is_digit( cstring {str} );
}

// DECLARATION lib::is_integer()

inline constexpr bool is_integer( const cstring & str ) {
	/// @regex /^(\+|-|)\d+$/
	if ( str.empty() )
		return false;
	auto str_begin = str.begin();
	if ( str.front() == '+' or str.front() == '-' )
		++str_begin;
	return is_digit( cstring {str_begin, str.end()} );
}

inline constexpr bool is_integer( const char * str ) {
	return is_integer( cstring {str} );
}

// DECLARATION lib::is_float()

inline constexpr bool is_float( const cstring & str ) {
	/// @regex /^(\+|-|)\d+[Ee](\+|-|)\d+$/
	/// @regex /^(\+|-|\d*)\.\d+$/
	/// @regex /^(\+|-|)\d+\.\d*$/
	if ( str.empty() )
		return false;
	const bool sign = str.front() == '+' or str.front() == '-';
	const bool dot = str.front() == '.';
	auto str_begin = str.begin();
	if ( sign or dot )
		++str_begin;
	bool digits = false;
	while ( str_begin != str.end() and is_digit( *str_begin ) ) {
		digits = true;
		++str_begin;
	}
	if ( str_begin == str.end() )
		return dot and digits;										///< e.g. ".123"
	if ( *str_begin == '.' ) {
		if ( dot )
			return false;											///< second "." symbol
		if ( ++str_begin == str.end() )
			return digits;											///< e.g. "456."
		return is_digit( cstring {str_begin, str.end()} );			///< e.g. "78.9"
	}
	if ( *str_begin == 'e' or *str_begin == 'E' )
		return digits
		and is_integer( cstring {str_begin + 1, str.end()} );		///< exponent form
	return false;
}

inline constexpr bool is_float( const char * str ) {
	return is_float( cstring {str} );
}

// DECLARATION lib::is_ascii_control()

inline /*constexpr */bool is_ascii_control( const char c ) noexcept {
	return ::std::iscntrl( (u8) c ) != 0;
}

inline /*constexpr */bool is_ascii_control( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii_control( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_ascii_printable()

inline /*constexpr */bool is_ascii_printable( const char c ) noexcept {
	return ::std::isprint( (u8) c ) != 0;
}

inline /*constexpr */bool is_ascii_printable( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii_printable( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_ascii_space()

inline /*constexpr */bool is_ascii_space( const char c ) noexcept {
	return ::std::isspace( (u8) c ) != 0;
}

inline /*constexpr */bool is_ascii_space( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii_space( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_ascii_blank()

inline /*constexpr */bool is_ascii_blank( const char c ) noexcept {
	return ::std::isblank( (u8) c ) != 0;
}

inline /*constexpr */bool is_ascii_blank( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii_blank( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_ascii_graphical()

inline /*constexpr */bool is_ascii_graphical( const char c ) noexcept {
	return ::std::isgraph( (u8) c ) != 0
		and c != '\x20'; ///< @fix for isgraph('\x20')
}

inline /*constexpr */bool is_ascii_graphical( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii_graphical( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_ascii()

inline /*constexpr */bool is_ascii( const char c ) noexcept {
	return is_ascii_control( c ) or is_ascii_printable( c );
}

inline /*constexpr */bool is_ascii( const cstring & str ) noexcept {
	for ( const auto c : str )
		if ( not is_ascii( c ) )
			return false;
	return not str.empty();
}

// DECLARATION lib::is_AZ(), lib::is_az(), lib::is_Az(), lib::is_09()

inline constexpr bool is_AZ( const char c ) noexcept {
	return 'A' <= c and c <= 'Z';
}

inline constexpr bool is_az( const char c ) noexcept {
	return 'a' <= c and c <= 'z';
}

inline constexpr bool is_Az( const char c ) noexcept {
	return is_AZ( c ) and is_az( c );
}

inline constexpr bool is_09( const char c ) noexcept {
	return '0' <= c and c <= '9';
}

// DECLARATION lib::hex()

inline constexpr char hex( u8 value, bool hi ) noexcept {
	constexpr const char * MAP = "0123456789abcdef";
	if ( hi ) value >>= 4;
	value &= 0x0F;
	return MAP[ value ];
}

inline ::std::string hex( const data::cbuffer_t & buffer, const cstring & separator = {} ) noexcept {
	if ( buffer.empty() )
		return {};
	const auto size = buffer.size() * 2 + separator.size() * ( buffer.size() - 1 );
	::std::string result;
	result.reserve( size );
	result.append( 1, hex( buffer.front(), true ) );
	result.append( 1, hex( buffer.front(), false ) );
	for ( auto i = 1_sz; i < buffer.size(); ++i ) {
		result.append( separator );
		result.append( 1, hex( buffer[i], true ) );
		result.append( 1, hex( buffer[i], false ) );
	}
	return result;
}

} // namespace lib

#endif // CPPLIB__lib__utils__string__hpp
