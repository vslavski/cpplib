/* File: /lib/impl_posix/cli/termios_driver.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <unistd.h>

#include <cstring>

#include <vector>

#include <cpp/lib_debug>

#include "../../../lib/types.hpp"
#include "../../../lib/literals.hpp"
#include "../../../lib/flags.hpp"
#include "../../../lib/utils/string.hpp"

#include "../../../lib/cli/interface.hpp"

#include "./termios_driver.hpp"

// IMPLEMENTATION lib::cli::impl::TermiosDriver

namespace lib::cli::impl {

TermiosDriver::TermiosDriver() {
	::tcgetattr( STDIN_FILENO, &orig_tc_attr );
	struct ::termios new_tc_attr;
	::std::memcpy( &new_tc_attr, &orig_tc_attr, sizeof(new_tc_attr) );
	Flag( new_tc_attr.c_lflag ).remove<ICANON>().remove<ECHO>();
	new_tc_attr.c_cc[VTIME] = 0;
	new_tc_attr.c_cc[VMIN] = 1;
	::tcsetattr( STDIN_FILENO, TCSANOW, &new_tc_attr );
}

TermiosDriver::~TermiosDriver() {
	::tcsetattr( STDIN_FILENO, TCSANOW, &orig_tc_attr );
	CPP_UNUSED( stream.putc( '\n' ) );
}

void TermiosDriver::out( const cstring & line )/* override*/ {
	clear_line();
	const auto & line_str = str( line );
	CPP_UNUSED( stream.printf( "\r%s\n", line_str.c_str() ) );
	redraw_input();
}

bool TermiosDriver::update()/* override*/ {
	auto * interface = get_link<Interface>();
	if ( interface == nullptr )
		return false; ///< @todo Possible it's legal to return "true" here.

	const auto & input_size = stream.read_size();
	if ( input_size.failed() )
		return false;
	if ( input_size == 0_sz )
		return true;

	::std::vector<u8> input( input_size.value() );
	const auto & read_size = stream.read( input );
	if ( read_size != input_size )
		return false;

	for ( const auto code : input ) {
		const char chr = (char)code;
		if ( is_ascii_printable( chr ) ) {
			input_string += chr;
			redraw_input();
		} else if ( chr == '\n' ) {
			CPP_UNUSED( stream.putc( '\n' ) );
			::std::string command;
			::std::swap( command, input_string );
			if ( not (*interface)( command ) )
				out( COMMAND_NOT_FOUND + command );
			redraw_input();
		} else if ( chr == 0x7F ) {
			if ( not input_string.empty() ) {
				clear_line();
				input_string.resize( input_string.size() - 1 );
				redraw_input();
			}
		} else if ( chr == '\t' ) {
			if ( interface->autocomplete( input_string ) )
				redraw_input();
		} else {
/*			clear_line();
			CPP_UNUSED( stream.printf( "\rcode 0x%02X [%c]\n", (int)chr, chr ) );
			redraw_input();
*/		}
	}
	return true;
}

void TermiosDriver::beforePromptChanged()/* override*/ {
	clear_line();
}

void TermiosDriver::afterPromptChanged()/* override*/ {
	redraw_input();
}

void TermiosDriver::clear_line() {
	CPP_UNUSED( stream.putc( '\r' ) );
	for ( auto i = 0_sz; i < prompt().size() + input_string.size(); ++i )
		CPP_UNUSED( stream.putc( ' ' ) );
}

void TermiosDriver::redraw_input() {
	CPP_UNUSED( stream.printf( "\r%s%s", prompt().c_str(), input_string.c_str() ) );
	CPP_UNUSED( stream.write_flush() );
}

} // namespace lib::cli::impl
