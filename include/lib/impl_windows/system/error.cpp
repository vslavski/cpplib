/* File: /lib/impl_windows/system/error.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <windows.h>

#include <cpp/lib_debug>

#include "./error.hpp"

namespace lib {

// IMPLEMENTATION lib::windows_error_category_t

::std::string windows_error_category_t::message( int condition ) const noexcept/* override*/ {
	char * buffer = nullptr;
	const auto length = ::FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_MAX_WIDTH_MASK
		, nullptr, (DWORD) condition, LANG_USER_DEFAULT, (LPTSTR) &buffer, 1024, nullptr );

	::std::string result;
	if ( buffer != nullptr ) {
		result.assign( buffer, (decltype(result)::size_type) length );
		const auto free_result = ::LocalFree( (HLOCAL) buffer );
		CPP_UNUSED( free_result );
		CPP_ASSERT( free_result == nullptr );
	}

	CPP_ASSERT( not result.empty() );
	return result;
}

// IMPLEMENTATION lib::windows_error_category()

const ::std::error_category & windows_error_category() noexcept {
	static windows_error_category_t category;
	return category;
}

} // namespace lib
