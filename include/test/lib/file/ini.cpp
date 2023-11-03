/* File: /test/lib/file/ini.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <type_traits>
#include <tuple>

#include <lib/literals.hpp>

#include <lib/impl/stream/data.hpp>

#include <lib/file/ini.hpp>

#include "./ini.hpp"

namespace test::lib::file {

void Ini::test_execute() noexcept/* override*/ {
	using namespace ::lib;

	static constexpr cstring INI_FILE =
		"\n\n\n"
		" "
		"\r\r\r"
		"\n"
		"\n "
		"\n #key1"
		"\n\t[section2]"
		"\n\r \r\n "
		"\nkey2="
		"\n\r \r\n "
		"\nkey3= "
		"\n [section1] "
		"\n key4 = #value1"
		;
	static constexpr ::std::tuple<cstring, cstring, cstring> INI_CONTENTS[] =
			{ { "",			"#key1",	""			}
			, { "section2",	"",			""			}
			, { "section2",	"key2",		""			}
			, { "section2",	"key3",		" "			}
			, { "section1",	"",			""			}
			, { "section1",	"key4 ",	" #value1"	}
		};
	static constexpr auto INI_CONTENTS_SIZE = ::std::extent_v<decltype(INI_CONTENTS)>;

	auto idx = 0_sz;

	::lib::stream::impl::data ini_stream{ INI_FILE };
	::lib::file::Ini ini_file{ ini_stream };
	const auto success = ini_file.foreach(
		[&, this]( const cstring & section, const cstring & key, const cstring & value ) {
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__LTv( idx, INI_CONTENTS_SIZE );
		CPPLIB__TEST__EQv( INI_CONTENTS[idx], ::std::tie( section, key, value ) );
		++idx;
		return not CPPLIB__TEST__IS_FAILED();
		});
	CPPLIB__TEST__TRUE( success );
	CPPLIB__TEST__EQ( idx, INI_CONTENTS_SIZE );
}

} // namespace test::lib::file
