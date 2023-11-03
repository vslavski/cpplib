/* File: /lib/impl_posix/application/termios_keyboard.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <unistd.h>

#include <algorithm>
#include <regex>

#include <cpp/lib_debug>

#include "../../../lib/literals.hpp"
#include "../../../lib/utils/enum.hpp"
#include "../../../lib/utils/string.hpp"

#include "./termios_keyboard.hpp"

namespace lib::application::impl {

// IMPLEMENTATION lib::application::impl::TermiosKeyboard

TermiosKeyboard::TermiosKeyboard( data::rstream_t & stream )
	: stream{ stream }
{
	::tcgetattr( STDIN_FILENO, &orig_tc_attr );
	struct ::termios new_tc_attr;
	::cfmakeraw( &new_tc_attr );
	::tcsetattr( STDIN_FILENO, TCSANOW, &new_tc_attr );
}

/*virtual */TermiosKeyboard::~TermiosKeyboard() {
	::tcsetattr( STDIN_FILENO, TCSANOW, &orig_tc_attr );
}

bool TermiosKeyboard::update() /* override*/ {
	return updateBuffer() and parseBuffer();
}

bool TermiosKeyboard::updateBuffer() {
	const auto & size = stream.read_size();
	if ( size.failed() or size == 0_sz )
		return size.success();
	const auto offset = buffer.size();
	buffer.resize( buffer.size() + size.value() );
	const auto & read_size = stream.read({ &buffer[offset], size.value() });
	if ( read_size.failed() )
		return false;
	buffer.resize( offset + read_size.value() );
	return true;
}

bool TermiosKeyboard::parseBuffer() {
	auto it = buffer.begin();
	while ( it != buffer.end() ) {
		u32 count = 1u;
		for ( auto jt = ::std::next( it )
			; jt != buffer.end() and *jt == *it
			; jt = ::std::next( it = jt ), ++count
			);

		if ( parseEscape( it ) )
			continue;

		/// @todo Multibyte support.
		const auto c = (char) *it;
		auto key = Key::invalid;
		if ( is_AZ( c ) )			key = MapKeyAZ( 'A', c );
		else if ( is_az( c ) )		key = MapKeyAZ( 'a', c );
		else if ( is_09( c ) )		key = MapKey09( '0', c );
		else if ( c == '\x1B' )		key = Key::ESCAPE;

		if ( key != Key::invalid )
			key_down( key, count );
		if ( is_ascii_printable( c ) )
			symbol( (c32) c, count );
		if ( key != Key::invalid )
			key_up( key );

		++it;
	}
	buffer.erase( buffer.begin(), it );
	return true;
}

bool TermiosKeyboard::parseEscape( auto & it ) {
	if ( *it != '\x1B' )
		return false;

	/*
			? = A B D C
			plain - 1B 4F ?? - "\e0?"				ctrl - 1B 5B 31 3B 35 ?? - "\e[1;5?"
	up		1B(?) 4F(O) 41(A)						1B(?) 5B([) 31(1) 3B(;) 35(5) 41(A)
	down	1B(?) 4F(O) 42(B)						1B(?) 5B([) 31(1) 3B(;) 35(5) 42(B)
	left	1B(?) 4F(O) 44(D)						1B(?) 5B([) 31(1) 3B(;) 35(5) 44(D)
	right	1B(?) 4F(O) 43(C)						1B(?) 5B([) 31(1) 3B(;) 35(5) 43(C)
			shift - 1B 5B 31 3B 32 ?? - "\e[1;2?"	alt - 1B 5B 31 3B 33 ?? - "\e[1;3?"
	up		1B(?) 5B([) 31(1) 3B(;) 32(2) 41(A)		1B(?) 5B([) 31(1) 3B(;) 33(3) 41(A)
	down	1B(?) 5B([) 31(1) 3B(;) 32(2) 42(B)		1B(?) 5B([) 31(1) 3B(;) 33(3) 42(B)
	left	1B(?) 5B([) 31(1) 3B(;) 32(2) 44(D)		1B(?) 5B([) 31(1) 3B(;) 33(3) 44(D)
	right	1B(?) 5B([) 31(1) 3B(;) 32(2) 43(C)		1B(?) 5B([) 31(1) 3B(;) 33(3) 43(C)
	*/

	static const ::std::regex ESCAPE_SEQUENCE{ "^\x1B(0|\\[1;([235]))([ABCD])" };
	static const usize MODIFIER_CODE = 2u;
	static const usize KEY_CODE = 3u;
	::std::cmatch match;
	const char * begin = (const char*) &*it;
	const char * end = (const char*) &*buffer.end();
	if ( not ::std::regex_search( begin, end, match, ESCAPE_SEQUENCE ) )
		return false;
	CPP_ASSERT( match.size() == 4 );
	it += match[0].length();

	auto modifier_key = Key::invalid;
	switch ( match[MODIFIER_CODE].str()[0] ) {
	case '5':	modifier_key = Key::CTRL;	break;
	case '3':	modifier_key = Key::ALT;	break;
	case '2':	modifier_key = Key::SHIFT;	break;
	}

	auto key = Key::invalid;
	switch( match[KEY_CODE].str()[0] ) {
	case 'A':	key = Key::UP;		break;
	case 'B':	key = Key::DOWN;	break;
	case 'D':	key = Key::LEFT;	break;
	case 'C':	key = Key::RIGHT;	break;
	}
	CPP_ASSERT( key != Key::invalid );

	if ( modifier_key != Key::invalid )
		++keys[ utils::EnumValue( modifier_key ) ];

	key_down( key );
	key_up( key );

	if ( modifier_key != Key::invalid )
		--keys[ utils::EnumValue( modifier_key ) ];

	return true;
}

} // namespace lib::application::impl
