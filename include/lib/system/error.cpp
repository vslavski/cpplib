/* File: /lib/system/error.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./error.hpp"

namespace lib {

// IMPLEMENTATION lib::error_category_t

::std::string error_category_t::message( int condition ) const noexcept/* override*/ {
	switch ( (error_code_t) condition ) {
	case error_code_t::NO_ERROR:			return "No error";
	case error_code_t::NOT_IMPLEMENTED:		return "Not implemented";
	case error_code_t::LOGIC_BROKEN:		return "Logic broken";
	case error_code_t::BAD_DATA:			return "Bad data";
	case error_code_t::EMPTY_DATA:			return "Empty data";
	case error_code_t::NOT_CONFIGURED:		return "Not configured";
	case error_code_t::DATA_OVERFLOW:		return "Data overflow";
	};
	return "Unknown";
}

// IMPLEMENTATION lib::error_category

const ::std::error_category & error_category() noexcept {
	static error_category_t category;
	return category;
}

} // namespace lib
