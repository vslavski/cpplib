/* File: /lib/system/error.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__system__error__hpp
#define CPPLIB__lib__system__error__hpp

#include <string>
#include <system_error>

/// @note `NO_ERROR` may be defined e.g. in `winerror.h`.
#ifdef NO_ERROR
	#undef NO_ERROR
#endif // NO_ERROR

namespace lib {

// DECLARATION lib::error_code_t

enum class error_code_t : int {
	NO_ERROR			= 0,
	NOT_IMPLEMENTED		= 1,
	LOGIC_BROKEN		= 2,
	BAD_DATA			= 3,
	EMPTY_DATA			= 4,
	NOT_CONFIGURED		= 5,
	DATA_OVERFLOW		= 6,
};

// DECLARATION lib::error_category_t

class error_category_t final
	: public ::std::error_category
{
public:
	error_category_t() noexcept = default;
	virtual ~error_category_t() noexcept = default;

	const char * name() const noexcept override { return "cpplib"; }

	::std::string message( int condition ) const noexcept override;
};

// DECLARATION lib::error_category()

const ::std::error_category & error_category() noexcept;

// DECLARATION lib::make_error_condition(), lib::make_error_...()

inline ::std::error_condition make_error_condition( error_code_t e ) noexcept
	{ return {(int) e, error_category()}; }

inline auto make_error_no_error() noexcept { return make_error_condition( error_code_t::NO_ERROR ); }
inline auto make_error_not_implemented() noexcept { return make_error_condition( error_code_t::NOT_IMPLEMENTED ); }
inline auto make_error_logic_broken() noexcept { return make_error_condition( error_code_t::LOGIC_BROKEN ); }
inline auto make_error_bad_data() noexcept { return make_error_condition( error_code_t::BAD_DATA ); }
inline auto make_error_empty_data() noexcept { return make_error_condition( error_code_t::EMPTY_DATA ); }
inline auto make_error_not_configured() noexcept { return make_error_condition( error_code_t::NOT_CONFIGURED ); }
inline auto make_error_data_overflow() noexcept { return make_error_condition( error_code_t::DATA_OVERFLOW ); }

// SPECIALIZATION lib::str()

inline ::std::string str( const ::std::error_condition & error ) noexcept
	{ return error.message(); }

} // namespace lib

#endif // CPPLIB__lib__system__error__hpp
