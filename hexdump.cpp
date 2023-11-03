/* File: hexdump.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstdio>
#include <cstring>

#include <array>
#include <algorithm>

#include <lib/types.hpp>
#include <lib/cstring.hpp>
#include <lib/data/buffer.hpp>
#include <lib/debug/hexdump.hpp>

#include <lib/impl/stream/file.hpp>

int main( int argc, const char * argv[] ) {
	using namespace lib;

	static constexpr usize BLOCK_SIZE = debug::hexdump::COLUMNS * 64;

	if ( argc != 2 or ::std::strcmp( argv[1], "-h" ) == 0 ) {
		::std::fprintf( stderr, "Usage: %s <filename>\n", argv[0] );
		return 1;
	}

	const cstring filename {argv[1]};
	const bool read_stdin = filename == cstring {"-"};

	stream::impl::file file;
	usize remain_bytes;

	if ( read_stdin ) {
		file = stdin;
		remain_bytes = ::std::numeric_limits<decltype(remain_bytes)>::max();
	} else if ( file.open( filename./*c_str*/data(), "rb" ) ) {
		const auto & read_size = file.read_size();
		if ( read_size.failed() ) {
			::std::fprintf( stderr, "Unable to obtain file size: %s\n", read_size.error().message().c_str() );
			return 2;
		}
		remain_bytes = read_size.value();
	} else {
		::std::fprintf( stderr, "Unable to open file \"%s\": %s\n"
			, filename./*c_str*/data(), file.error().message().c_str() );
		return 3;
	}

	const auto file_size = remain_bytes;
	::std::array< u8, BLOCK_SIZE > block;
	while ( remain_bytes > 0 ) {
		const auto count = ::std::min( remain_bytes, BLOCK_SIZE );
		const auto & read_bytes = file.read({ block.data(), count });
		if ( read_bytes.failed() ) {
			::std::fprintf( stderr, "Unable read file: %s\n", read_bytes.error().message().c_str() );
			return 4;
		}

		const auto offset = file_size - remain_bytes;
		debug::hexdump( {block.data(), read_bytes.value()}, debug::hexdump::external, offset );
		remain_bytes -= count;

		if ( read_bytes != count ) {
			if ( read_stdin )
				break;
			::std::fprintf( stderr, "Error reading block: %zi bytes read, %zu requested.\n"
				, read_bytes.value(), count );
			return 5;
		}
	}

	return 0;
}
