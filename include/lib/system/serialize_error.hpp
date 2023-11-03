/* File: /lib/system/serialize_error.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__system__serialize_error__hpp
#define CPPLIB__lib__system__serialize_error__hpp

#include <string>
#include <system_error>

namespace lib {

// DECLARATION lib::serialize_error_code_t

enum class serialize_error_code_t : int {
	NO_ERROR			= 0,
	NULL_DATA			= 1,
};

// DECLARATION lib::serialize_error_category_t

class serialize_error_category_t final
	: public ::std::error_category
{
public:
	serialize_error_category_t() noexcept = default;
	virtual ~serialize_error_category_t() noexcept = default;

	const char * name() const noexcept override { return "cpplib.serialize"; }

	::std::string message( int condition ) const noexcept override;
};

// DECLARATION lib::serialize_error_category()

const ::std::error_category & serialize_error_category() noexcept;

// DECLARATION lib::make_error_condition(), lib::make_serialize_error_...()

inline ::std::error_condition make_error_condition( serialize_error_code_t e ) noexcept
	{ return {(int) e, serialize_error_category()}; }

inline auto make_serialize_error_no_error() noexcept { return make_error_condition( serialize_error_code_t::NO_ERROR ); }
inline auto make_serialize_error_null_data() noexcept { return make_error_condition( serialize_error_code_t::NULL_DATA ); }

} // namespace lib

#endif // CPPLIB__lib__system__serialize_error__hpp
