/* File: /lib/utils/value.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__value__hpp
#define CPPLIB__lib__utils__value__hpp

#include <cstdlib>
#include <cinttypes>

#include <compare>
#include <string>
#include <any>

#include <cpp/lib_concepts>

#include "../../lib/types.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/utils/string.hpp"

// DECLARATION lib::tag_utils_value

namespace lib {

template<class T>
class tag_utils_value {
public:
	virtual ~tag_utils_value() = default;
	virtual void setValue( const cstring & name, const T & value ) = 0;
	virtual T getValue( const cstring &/* name*/ ) { return T{}; }
};

} // namespace lib

// DECLARATION lib::utils::Value

namespace lib::utils {

template
	< ::cpp::IntOrEnum		Int		= isize
	, ::cpp::FloatingPoint	Float	= f64
	, class					String	= ::std::string
	>
class Value {
public:
	using int_type		= Int;
	using float_type	= Float;
	using string_type	= String;

	Value() = default;
	template< ::cpp::IntOrEnum I, ::cpp::FloatingPoint F, class S >
	explicit Value( const Value<I, F, S> & v ) : i_{v.i()}, f_{v.f()}, s_{v.s()} {}
	explicit Value( ::std::any && any_ ) : any_{ ::std::move( any_ ) } {}

	template< ::cpp::IntOrEnum T >
	Value( const T & i )				{ from_int(i); }
	template< ::cpp::FloatingPoint T >
	Value( const T & f )				{ from_float(f); }
	Value( const string_type & s )		{ from_string(s); }

	template< ::cpp::IntOrEnum T >
	Value & operator = ( const T & i )				{ from_int(i); return *this; }
	template< ::cpp::FloatingPoint T >
	Value & operator = ( const T & f )				{ from_float(f); return *this; }
	Value & operator = ( const string_type & s )	{ from_string(s); return *this; }

	template< ::cpp::IntOrEnum T >
	constexpr auto operator <=> ( const T & i ) const noexcept				{ return i_ <=> (int_type)i; }
	template< ::cpp::FloatingPoint T >
	constexpr auto operator <=> ( const T & f ) const noexcept				{ return f_ <=> (float_type)f; }
	constexpr auto operator <=> ( const string_type & s ) const noexcept	{ return s_ <=> s; }

	template< ::cpp::IntOrEnum T >
	constexpr bool operator == ( T i ) const noexcept					{ return i_ == (int_type)i; }
	template< ::cpp::FloatingPoint T >
	constexpr bool operator == ( const T & f ) const noexcept			{ return f_ == (float_type)f; }
	constexpr bool operator == ( const string_type & s ) const noexcept	{ return s_ == s; }

	template< ::cpp::IntOrEnum T >
	operator T () const						{ return (T)i_; }
	template< ::cpp::FloatingPoint T >
	operator T () const						{ return (T)f_; }
	operator const string_type & () const	{ return s_; }

	const int_type &	i() const { return i_; }
	const float_type &	f() const { return f_; }
	const string_type &	s() const { return s_; }
	bool				b() const { return s_ == "true"; }

	::std::any & any() { return any_; }
	const ::std::any & any() const { return any_; }
private:

	void from_int( int_type i ) {
		i_ = i;
		f_ = (float_type)i;
		s_ = str( i );
	}
	void from_float( float_type f ) {
		f_ = f;
		i_ = (int_type)f;
		s_ = str( f );
	}
	void from_string( const string_type & s ) {
		s_ = s;
		i_ = (int_type)::std::strtoimax( s.c_str(), nullptr, 0 );
		f_ = (float_type)::std::strtof( s.c_str(), nullptr );
	}

	int_type i_ = 0;
	float_type f_ = 0.f;
	string_type s_;
	::std::any any_;
};

} // namespace lib::utils

#endif // CPPLIB__lib__utils__value__hpp
