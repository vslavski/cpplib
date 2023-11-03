/* File: /lib/file/ini.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <type_traits>
#include <regex>

#include <cpp/lib_debug>

#include "../../lib/types.hpp"
#include "../../lib/literals.hpp"

#include "./ini.hpp"

namespace lib::file {

// IMPLEMENTATION lib::file::Ini

Ini::Ini( data::rstream_t & stream ) {
	CPP_UNUSED( read( stream ) );
}

bool Ini::read( data::rstream_t & stream ) {
	const auto & content_size = stream.read_size();
	if ( content_size.failed() or content_size == 0_sz )
		return content_size.success();
	static_assert( ::std::is_same_v<decltype(content)::value_type, char> );
	content.resize( content_size.value() );
	const auto & read_size = stream.read( content );
	if ( read_size != content_size )
		return false;
	parseContent();
	return true;
}

void Ini::reset() {
	sections_.clear();
	content.clear();
}

bool Ini::foreach( ForeachFn foreach_fn ) const {
	for ( const auto & section : sections_ )
		for ( const auto & entry : section.second )
			if ( not foreach_fn( section.first, entry.first, entry.second ) )
				return false;
	return true;
}

void Ini::parseContent() {
	static const ::std::regex RE_LINE( "^([^\n\r]*)([\n\r]*)" );
	static const ::std::regex RE_SECTION( "^\\s*\\[(.*)\\]\\s*$" );
	static const ::std::regex RE_KEY_VALUE( "^\\s*([^\\s=][^=]+)(=(.*))?$" );

	auto content_ptr = content.c_str();
	auto section_it = sections_.end();

	::std::cmatch match_ptr;
	::std::match_results<cstring::const_iterator> match_it;

	while ( ::std::regex_search( content_ptr, match_ptr, RE_LINE ) ) {
		if ( match_ptr.length(0) == 0 )
			break;

		const cstring line {match_ptr[1].first, match_ptr[1].second};
		content_ptr += match_ptr.length(0);

		if ( ::std::regex_match( line.begin(), line.end(), match_it, RE_SECTION ) ) {
			const cstring section {match_it[1].first, match_it[1].second};
			section_it = sections_.emplace( section, Section{} ).first;
			if ( section_it->second.empty() )
				section_it->second.resize( 1 );
			continue;
		}

		if ( not ::std::regex_match( line.begin(), line.end(), match_it, RE_KEY_VALUE ) )
			continue;

		if ( section_it == sections_.end() )
			section_it = sections_.emplace( cstring{}, Section{} ).first;

		const cstring key {match_it[1].first, match_it[1].second};
		const cstring value {match_it[3].first, match_it[3].second};
		section_it->second.emplace_back( key, value );
	}
}

} // namespace lib::file
