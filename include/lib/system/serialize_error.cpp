/* File: /lib/system/serialize_error.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./serialize_error.hpp"

namespace lib {

// IMPLEMENTATION lib::serialize_error_category_t

::std::string serialize_error_category_t::message( int condition ) const noexcept/* override*/ {
	switch ( (serialize_error_code_t) condition ) {
	case serialize_error_code_t::NO_ERROR:			return "No error";
	case serialize_error_code_t::NULL_DATA:			return "Null data";
	};
	return "Unknown";
}

// IMPLEMENTATION lib::serialize_error_category

const ::std::error_category & serialize_error_category() noexcept {
	static serialize_error_category_t category;
	return category;
}

} // namespace lib
