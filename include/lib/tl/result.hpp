/* File: /lib/tl/result.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__result__hpp
#define CPPLIB__lib__tl__result__hpp

#include <compare>
#include <utility>
#include <variant>

#include <cpp/lib_concepts>
#include <cpp/lib_debug>
#include <cpp/lib_system_error>

namespace lib::tl {

// DECLARATION lib::tl::result_t<>

template< class T >
class result_t {
public:
	using value_type = T;
	using error_type = ::cpp::error_condition;

	constexpr result_t() noexcept = default;

	constexpr result_t( result_t && other ) noexcept
		: result {::std::move( other.result )} {}
	constexpr result_t( const result_t & other ) noexcept
		: result {other.result} {}
	constexpr result_t( value_type && value ) noexcept
		: result {::std::move( value )} {}
	constexpr result_t( const value_type & value ) noexcept
		: result {value} {}
	constexpr explicit result_t( const error_type & error ) noexcept
		: result {error} {}

	constexpr result_t & operator = ( result_t && other ) noexcept
		{ result = ::std::move( other.result ); return *this; }
	constexpr result_t & operator = ( const result_t & other ) noexcept
		{ result = other.result; return *this; }
	constexpr result_t & operator = ( value_type && value ) noexcept
		{ result = ::std::move( value ); return *this; }
	constexpr result_t & operator = ( const value_type & value ) noexcept
		{ result = value; return *this; }

	constexpr bool operator == ( const result_t & other ) const noexcept
		{ return result == other.result; }

	constexpr value_type & value() noexcept
		{ CPP_ASSERT( success() ); return ::std::get<value_type>( result ); }
	constexpr const value_type & value() const noexcept
		{ CPP_ASSERT( success() ); return ::std::get<value_type>( result ); }

	constexpr const error_type & error() const noexcept
		{ CPP_ASSERT( failed() ); return ::std::get<error_type>( result ); }

	constexpr bool success() const noexcept
		{ return ::std::holds_alternative<value_type>( result ); }
	constexpr bool failed() const noexcept
		{ return ::std::holds_alternative<error_type>( result ); }
protected:
	::std::variant<value_type, error_type> result;
};

// DECLARATION lib::tl::result_numeric_t<>

template< ::cpp::Numeric T >
class result_numeric_t
	: public result_t<T>
{
	using Super = result_t<T>;
public:
	using value_type = typename Super::value_type;

	constexpr result_numeric_t( auto&&...args ) noexcept : Super {args...} {}

	constexpr explicit operator value_type () const noexcept { return Super::value(); }

	constexpr bool operator == ( const result_numeric_t & other ) const noexcept
		{ return Super::operator == ( other ); }

	template< ::cpp::Numeric V >
	constexpr bool operator == ( const V & other ) const noexcept
		{ return Super::success() ? Super::value() == other : false; }
	template< ::cpp::Numeric V >
	constexpr auto operator <=> ( const V & other ) const noexcept
		{ return Super::value() <=> other; }

	constexpr result_numeric_t & operator ++ () noexcept
		{ if ( Super::success() ) ++Super::value(); return *this; }
	constexpr result_numeric_t & operator -- () noexcept
		{ if ( Super::success() ) --Super::value(); return *this; }
	constexpr result_numeric_t operator ++ ( int ) noexcept
		{ auto r = *this; if ( r.success() ) ++Super::value(); return r; }
	constexpr result_numeric_t operator -- ( int ) noexcept
		{ auto r = *this; if ( r.success() ) --Super::value(); return r; }

	constexpr result_numeric_t operator + ( const result_numeric_t & other ) const noexcept
		{ auto r = *this; if ( other.success() ) r += other.value(); return r; }
	constexpr result_numeric_t operator - ( const result_numeric_t & other ) const noexcept
		{ auto r = *this; if ( other.success() ) r -= other.value(); return r; }
	constexpr result_numeric_t operator * ( const result_numeric_t & other ) const noexcept
		{ auto r = *this; if ( other.success() ) r *= other.value(); return r; }
	constexpr result_numeric_t operator / ( const result_numeric_t & other ) const noexcept
		{ auto r = *this; if ( other.success() ) r /= other.value(); return r; }

	constexpr result_numeric_t operator + ( const value_type & other ) const noexcept
		{ return Super::success() ? result_numeric_t {Super::value() + other} : *this; }
	constexpr result_numeric_t operator - ( const value_type & other ) const noexcept
		{ return Super::success() ? result_numeric_t {Super::value() - other} : *this; }
	constexpr result_numeric_t operator * ( const value_type & other ) const noexcept
		{ return Super::success() ? result_numeric_t {Super::value() * other} : *this; }
	constexpr result_numeric_t operator / ( const value_type & other ) const noexcept
		{ return Super::success() ? result_numeric_t {Super::value() / other} : *this; }

	constexpr result_numeric_t & operator += ( const value_type & other ) noexcept
		{ if ( Super::success() ) Super::value() += other; return *this; }
	constexpr result_numeric_t & operator -= ( const value_type & other ) noexcept
		{ if ( Super::success() ) Super::value() -= other; return *this; }
	constexpr result_numeric_t & operator *= ( const value_type & other ) noexcept
		{ if ( Super::success() ) Super::value() *= other; return *this; }
	constexpr result_numeric_t & operator /= ( const value_type & other ) noexcept
		{ if ( Super::success() ) Super::value() /= other; return *this; }
};

// DECLARATION lib::tl::result_numeric_t<> operator + - * /

template< ::cpp::Numeric T >
inline constexpr result_numeric_t<T> operator + ( const T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return rhv.success() ? result_numeric_t<T> {lhv + rhv.value()} : rhv; }

template< ::cpp::Numeric T >
inline constexpr result_numeric_t<T> operator - ( const T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return rhv.success() ? result_numeric_t<T> {lhv - rhv.value()} : rhv; }

template< ::cpp::Numeric T >
inline constexpr result_numeric_t<T> operator * ( const T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return rhv.success() ? result_numeric_t<T> {lhv * rhv.value()} : rhv; }

template< ::cpp::Numeric T >
inline constexpr result_numeric_t<T> operator / ( const T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return rhv.success() ? result_numeric_t<T> {lhv / rhv.value()} : rhv; }


template< ::cpp::Numeric T >
inline constexpr T & operator += ( T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return lhv += rhv.value(); }

template< ::cpp::Numeric T >
inline constexpr T & operator -= ( T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return lhv -= rhv.value(); }

template< ::cpp::Numeric T >
inline constexpr T & operator *= ( T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return lhv *= rhv.value(); }

template< ::cpp::Numeric T >
inline constexpr T & operator /= ( T & lhv, const result_numeric_t<T> & rhv ) noexcept
{ return lhv /= rhv.value(); }

} // namespace lib::tl

#endif // CPPLIB__lib__tl__result__hpp
