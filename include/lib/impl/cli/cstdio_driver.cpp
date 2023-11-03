/* File: /lib/impl/cli/cstdio_driver.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstdio>

#include "../../../lib/utils/string.hpp"

#include "../../../lib/cli/interface.hpp"

#include "./cstdio_driver.hpp"

// IMPLEMENTATION lib::cli::impl::CstdioDriver

namespace lib::cli::impl {

CstdioDriver::~CstdioDriver() {
	::std::fputc( '\n', stdout );
	::std::fflush( stdout );
}

void CstdioDriver::out( const cstring & line )/* override*/ {
	if ( not input_clear )
		::std::fputc( '\n', stdout );
	const auto & line_str = str( line );
	::std::printf( "%s\n", line_str.c_str() );
	::std::fflush( stdout );
	input_clear = true;
}

bool CstdioDriver::update()/* override*/ {
	auto * interface = get_link<Interface>();
	if ( interface == nullptr )
		return false; ///< @todo Possible it's legal to return "true" here.

	redraw_input();
	const char chr = (char)::std::getchar();
	if ( is_ascii_printable( chr ) ) {
		input_clear = false;
		input_string += chr;
	} else if ( chr == '\n' ) {
		input_clear = true;
		::std::string command;
		::std::swap( command, input_string );
		if ( not (*interface)( command ) )
			out( COMMAND_NOT_FOUND + command );
	}
	return true;
}

void CstdioDriver::afterPromptChanged()/* override*/ {
	redraw_input();
}

void CstdioDriver::redraw_input() {
	if ( input_clear ) {
		::std::printf( "%s%s", prompt().c_str(), input_string.c_str() );
		::std::fflush( stdout );
	}
	input_clear = false;
}

} // namespace lib::cli::impl
