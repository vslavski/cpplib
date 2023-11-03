/* File: /lib/flags.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__flags__hpp
#define CPPLIB__lib__flags__hpp

#include <limits>
#include <type_traits>

#include <cpp/lib_concepts>

#include "../lib/tl/noncopyable.hpp"
#include "../lib/types.hpp"

namespace lib {

// DECLARATION lib::Flag<>

template< class Type >
requires ( ::cpp::Integral<Type> or ::cpp::Enumeration<Type> )
struct Flag final
	: tl::noncopymovable< Flag<Type> >
{
private:
	template< ::cpp::Integral T >
	static constexpr T get_integral_type() noexcept;
	template< ::cpp::Enumeration T >
	static constexpr ::std::underlying_type_t<T> get_integral_type() noexcept;
public:
	using integral_type = decltype(get_integral_type<Type>());


	constexpr Flag( Type & flag ) noexcept : flag {flag} {}

	constexpr Flag & operator += ( const integral_type & value ) noexcept
	{ return add( value ); }

	template< ::cpp::Enumeration Value >
	constexpr Flag & operator += ( Value value ) noexcept
	{ return add( value ); }


	constexpr Flag & operator -= ( const integral_type & value ) noexcept
	{ return remove( value ); }

	template< ::cpp::Enumeration Value >
	constexpr Flag & operator -= ( Value value ) noexcept
	{ return remove( value ); }


	constexpr Flag & add( const integral_type & value ) noexcept
	{ flag = (Type) ((integral_type) flag bitor value); return *this; }

	template< auto Value/* = integral constant*/ > requires ( ::cpp::Integral<decltype(Value)>
		and Value <= ::std::numeric_limits<integral_type>::max() )
	constexpr Flag & add() noexcept
	{ return add( (integral_type) Value ); }

	template< ::cpp::Enumeration Value >
	constexpr Flag & add( Value value ) noexcept
	{ return add( (integral_type) value ); }


	constexpr Flag & remove( const integral_type & value ) noexcept
	{ flag = (Type) ((integral_type) flag bitand compl value); return *this; }

	template< auto Value/* = integral constant*/ > requires ( ::cpp::Integral<decltype(Value)>
		and Value <= ::std::numeric_limits<integral_type>::max() )
	constexpr Flag & remove() noexcept
	{ return remove( (integral_type) Value ); }

	template< ::cpp::Enumeration Value >
	constexpr Flag & remove( Value value ) noexcept
	{ return remove( (integral_type) value ); }


	constexpr bool check( const integral_type & value ) const noexcept
	{ return ((integral_type) flag bitand value) == value; }

	template< auto Value/* = integral constant*/ > requires ( ::cpp::Integral<decltype(Value)>
		and Value <= ::std::numeric_limits<integral_type>::max() )
	constexpr bool check() const noexcept
	{ return check( (integral_type) Value ); }

	template< ::cpp::Enumeration Value >
	constexpr bool check( Value value ) const noexcept
	{ return check( (integral_type) value ); }


	Type & flag;
};

} // namespace lib

#endif // CPPLIB__lib__flags__hpp
