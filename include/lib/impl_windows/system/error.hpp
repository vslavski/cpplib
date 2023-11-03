/* File: /lib/impl_windows/system/error.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_windows__system__error__hpp
#define CPPLIB__lib__impl_windows__system__error__hpp

#include <string>
#include <system_error>

namespace lib {

// DECLARATION lib::windows_error_category_t

class windows_error_category_t final
	: public ::std::error_category
{
public:
	windows_error_category_t() noexcept = default;
	virtual ~windows_error_category_t() noexcept = default;

	const char * name() const noexcept override { return "windows"; }

	::std::string message( int condition ) const noexcept override;
};

// DECLARATION lib::windows_error_category()

const ::std::error_category & windows_error_category() noexcept;

// DECLARATION lib::make_windows_error()

inline ::std::error_condition make_windows_error( int code ) noexcept
	{ return {code, windows_error_category()}; }

} // namespace lib

#endif // CPPLIB__lib__impl_windows__system__error__hpp
