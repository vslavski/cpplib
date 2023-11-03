/* File: /lib/utils/log_support.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__log_support__hpp
#define CPPLIB__lib__utils__log_support__hpp

#include <cstdio>

#include <string>

#include "../../lib/tl/listener.hpp"

namespace lib {

// DECLARATION lib::tag_utils_log_support

namespace detail { struct tag_utils_log_support_ {}; }

template<class T>
struct tag_utils_log_support : detail::tag_utils_log_support_ {
	template< class First, class...Args >
	void log( const char * format, First first, Args...args ) const {
		::std::string buff( 1024, '\0' );
		::std::snprintf( buff.data(), buff.size(), format, first, args... );
		on_log( buff );
	}

	void log( const ::std::string & message ) { on_log( message ); }

	tl::listener<void, const ::std::string &> on_log;
};

} // namespace lib

#endif // CPPLIB__lib__utils__log_support__hpp
