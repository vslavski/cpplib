/* File: /lib/utils/case_string.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstddef>

#include <cpp/lib_debug>

#include "../../lib/utils/string.hpp"

#include "./case_string.hpp"

namespace lib::utils {

// IMPLEMENTATION lib::utils::case_string

case_string & case_string::operator = ( const cstring & name_ ) {
	tokenize( name_ );
	return *this;
}

::std::string case_string::lower() const {
	::std::string result;
	result.reserve( name.length() );
	for ( const auto & t : tokens )
		result += str( t );
	return result;
}

::std::string case_string::upper() const {
	return to_upper( lower() );
}

::std::string case_string::pascal() const {
	::std::string result;
	result.reserve( name.length() );
	for ( const auto & t : tokens ) {
		auto token = str( t );
		token[0] = to_upper( token[0] );
		result += token;
	}
	return result;
}

::std::string case_string::camel() const {
	auto result = pascal();
	result[0] = to_lower( result[0] );
	return result;
}

::std::string case_string::snake_lower() const {
	::std::string result;
	result.reserve( name.length() + tokens.size() );
	for ( const auto & t : tokens )
		{ result += str( t ); result += '_'; }
	if ( not result.empty() )
		result.pop_back();
	return result;
}

::std::string case_string::snake_upper() const {
	return to_upper( snake_lower() );
}

void case_string::tokenize( const cstring & name_ ) {
	name = name_;
	tokens.clear();
	/*
	 * .--------.-----------.	SYMBOL TYPES:
	 * | INPUT	| TYPES		|	0 - null-terminator
	 * :--------+-----------:	_ - underscore symbol
	 * | foobar	| aaaaaa0	|	U - uppercase symbol
	 * | Foobar	| Uaaaaa0	|	a - any other symbol
	 * | fooBar	| aaaUaa0	|
	 * | FooBar	| UaaUaa0	|	NEXT TOKEN:
	 * | FOOBAR	| UUUUUU0	|	if a -> U
	 * | foo_bar| aaa_aaa0	|	_ - always
	 * | Foo_bar| Uaa_aaa0	|	0 - always
	 * | foo_Bar| aaa_Uaa0	|
	 * | Foo_Bar| Uaa_Uaa0	|
	 * | FOO_BAR| UUU_UUU0	|
	 * '--------'-----------'
	 */
	enum class Symbol { any, UPPER, UNDER, END };
	Symbol last_symbol = Symbol::any;
	auto token_begin = name.begin();
	for ( auto it = token_begin ;; ++it ) {
		Symbol								symbol = Symbol::any;
		if				( it == name.end() )symbol = Symbol::END;
		else if			( is_upper( *it ) )	symbol = Symbol::UPPER;
		else if			( *it == '_' )		symbol = Symbol::UNDER;
		else CPP_ASSERT	( is_lower( *it ) );

		if ( ( last_symbol == Symbol::any and symbol == Symbol::UPPER )
			or symbol == Symbol::UNDER
			or symbol == Symbol::END ) {
			if ( token_begin < it )
				tokens.emplace_back( token_begin, it );
			token_begin = it;
		}
		if		( symbol == Symbol::any )	{}
		else if	( symbol == Symbol::UPPER )	*it = to_lower( *it );
		else if	( symbol == Symbol::UNDER )	++token_begin;
		else if	( symbol == Symbol::END )	break;
		last_symbol = symbol;
	}
}

} // namespace lib::utils
