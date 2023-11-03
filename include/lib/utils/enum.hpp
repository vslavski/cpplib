/* File: /lib/utils/enum.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__enum__hpp
#define CPPLIB__lib__utils__enum__hpp

#include <type_traits>
#include <functional>

#include <cpp/lib_concepts>

namespace lib::utils {

// DECLARATION LIB_UTILS_ENUM

/// @note 'count' value includes, 'invalid' is always zero.
#define LIB_UTILS_ENUM( EnumSpec, ... )	\
	enum class EnumSpec {				\
		invalid = 0,					\
		__VA_ARGS__,					\
		count							\
	};

// DECLARATION lib::utils::EnumId<>

template< ::cpp::Enumeration E >
using EnumId = ::std::underlying_type_t< E >;

// DECLARATION lib::utils::EnumValue<>()

template< ::cpp::Enumeration E >
inline constexpr EnumId<E> EnumValue( E e ) noexcept { return (EnumId<E>)e; }

// DECLARATION lib::utils::NAMES<>[]

template< ::cpp::Enumeration E >
inline constexpr const char * NAMES[ (EnumId<E>) E::count ] = { "<LIB_UTILS_ENUM_NAMES missed>" };

// DECLARATION LIB_UTILS_ENUM_NAMES

#define LIB_UTILS_ENUM_NAMES( Enum, ... )										\
namespace lib::utils {															\
	template<>																	\
	inline constexpr const char * NAMES<Enum>[ (EnumId<Enum>) Enum::count ] = {	\
		"<invalid>",															\
		__VA_ARGS__																\
	};																			\
} /* namespace lib::utils */

// DECLARATION lib::utils: is_valid<>, foreach<>

template< ::cpp::Enumeration E, ::cpp::Integral I >
inline constexpr bool is_valid( I i ) noexcept {
	return i > (EnumId<E>)E::invalid and i < (EnumId<E>)E::count;
}

template< ::cpp::Enumeration E >
inline constexpr bool is_valid( E e ) noexcept {
	return is_valid<E>( (EnumId<E>)e );
}

template< ::cpp::Enumeration E >
inline constexpr bool foreach( ::std::function<bool(E, EnumId<E>)> statement ) noexcept {
	auto id = (EnumId<E>)E::invalid;
	while ( ++id < (EnumId<E>)E::count )
		if ( not statement( (E)id, id ) )
			return false;
	return true;
}

} // namespace lib::utils

#include <string>

#include "../../lib/cstring.hpp"

// DECLARATION lib::str<>, lib::unstr<>

namespace lib {

template< ::cpp::Enumeration E >
inline /*constexpr*/ ::std::string str( E e ) noexcept {
	using namespace utils;
	if ( is_valid( e ) )
		return NAMES<E>[ (EnumId<E>)e ];
	return NAMES<E>[ (EnumId<E>)E::invalid ];
}

template< ::cpp::Enumeration E >
inline constexpr E unstr( const cstring & field_name ) noexcept {
	using namespace utils;
	auto id = (EnumId<E>)E::invalid;
	while ( ++id < (EnumId<E>)E::count )
		if ( field_name == NAMES<E>[id] )
			return (E)id;
	return E::invalid;
}

} // namespace lib::utils

#endif // CPPLIB__lib__utils__enum__hpp
