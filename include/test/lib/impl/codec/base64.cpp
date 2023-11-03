/* File: /test/lib/impl/codec/base64.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/types.hpp>
#include <lib/cstring.hpp>
#include <lib/utils/string.hpp>

#include <lib/impl/codec/base64.hpp>

#include "./base64.hpp"

namespace test::lib::codec::impl {

void Base64::test_execute() noexcept/* override*/ {
	static constexpr ::lib::usize COUNT = 7;
	static constexpr ::lib::cstring PLAIN[COUNT] =
		{ ""
		, "B"
		, "B6"
		, "B64"
		, "Base64 codec work"
		, "Base64 codec works"
		, "Base64 codec works!"
		};
	static constexpr ::lib::cstring CYPHER[COUNT] =
		{ ""
		, "Qg=="
		, "QjY="
		, "QjY0"
		, "QmFzZTY0IGNvZGVjIHdvcms="
		, "QmFzZTY0IGNvZGVjIHdvcmtz"
		, "QmFzZTY0IGNvZGVjIHdvcmtzIQ=="
		};
	::lib::codec::impl::base64 codec;
	for ( ::lib::usize i = 0; i < COUNT; ++i ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__EQ( codec.encode( PLAIN[i] ), PLAIN[i].size() );
		CPPLIB__TEST__EQ( ::lib::str( codec ), CYPHER[i] );
		CPPLIB__TEST__EQ( codec.decode( CYPHER[i] ), CYPHER[i].size() );
		CPPLIB__TEST__EQ( ::lib::str( codec ), PLAIN[i] );
	}
}

} // namespace test::lib::codec::impl
