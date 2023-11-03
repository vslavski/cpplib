/* File: /lib/utils/case_string.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__case_string__hpp
#define CPPLIB__lib__utils__case_string__hpp

#include <string>
#include <vector>

#include "../../lib/cstring.hpp"

namespace lib::utils {

// DECLARATION lib::utils::case_string

/// @todo Code check, full test, and code cleanup required.

/// @todo case_string_t<CharT>, case_string, case_wstring
class case_string final {
public:
	case_string & operator = ( const cstring & name );
	::std::string lower() const;
	::std::string upper() const;
	::std::string pascal() const;
	::std::string camel() const;
	::std::string snake_lower() const;
	::std::string snake_upper() const;
private:
	void tokenize( const cstring & name );
	::std::string name;
	::std::vector<cstring> tokens;
};

} // namespace lib::utils

#endif // CPPLIB__lib__utils__case_string__hpp
