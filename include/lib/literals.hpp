/* File: /lib/literals.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__literals__hpp
#define CPPLIB__lib__literals__hpp

#include <limits>

#include <cpp/lib_algorithm>
#include <cpp/lib_concepts>
#include <cpp/lib_debug>

#include "./types.hpp"

namespace lib {

namespace detail {

template< ::cpp::Integral T >
constexpr T validated_user_literal( unsigned long long int value ) noexcept {
	constexpr auto VALID_VALUE = 0;
	constexpr auto OUT_OF_RANGE_VALUE = -1;
	using ValueT = decltype(value);
	const auto check = ::cpp::is_in_range<ValueT>( value, 0, (ValueT) ::std::numeric_limits<T>::max() )
		? VALID_VALUE : OUT_OF_RANGE_VALUE;
	CPP_ASSERT( check != OUT_OF_RANGE_VALUE );
	const T out_of_range_check[1] = { static_cast<T>( value ) };
	return out_of_range_check[check];
}

} // namespace detail

consteval i8 operator ""_i8( unsigned long long int value ) noexcept { return detail::validated_user_literal<i8>( value ); }
constexpr i16 operator ""_i16( unsigned long long int value ) noexcept { return detail::validated_user_literal<i16>( value ); }
constexpr i32 operator ""_i32( unsigned long long int value ) noexcept { return detail::validated_user_literal<i32>( value ); }
constexpr i64 operator ""_i64( unsigned long long int value ) noexcept { return detail::validated_user_literal<i64>( value ); }

constexpr u8 operator ""_u8( unsigned long long int value ) noexcept { return detail::validated_user_literal<u8>( value ); }
constexpr u16 operator ""_u16( unsigned long long int value ) noexcept { return detail::validated_user_literal<u16>( value ); }
constexpr u32 operator ""_u32( unsigned long long int value ) noexcept { return detail::validated_user_literal<u32>( value ); }
constexpr u64 operator ""_u64( unsigned long long int value ) noexcept { return detail::validated_user_literal<u64>( value ); }

constexpr f32 operator ""_f32( long double value ) noexcept { return static_cast<f32>( value ); }
constexpr f64 operator ""_f64( long double value ) noexcept { return static_cast<f64>( value ); }

constexpr usize operator ""_sz( unsigned long long int value ) noexcept { return detail::validated_user_literal<usize>( value ); }
constexpr isize operator ""_iz( unsigned long long int value ) noexcept { return detail::validated_user_literal<isize>( value ); }

constexpr c8 operator ""_c8( c8 value ) noexcept { return detail::validated_user_literal<c8>( value ); }
constexpr c16 operator ""_c16( c16 value ) noexcept { return detail::validated_user_literal<c16>( value ); }
constexpr c32 operator ""_c32( c32 value ) noexcept { return detail::validated_user_literal<c32>( value ); }

} // namespace lib

#endif // CPPLIB__lib__literals__hpp
