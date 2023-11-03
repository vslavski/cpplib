/* File: /lib/data/serialize.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__data__serialize__hpp
#define CPPLIB__lib__data__serialize__hpp

#include <type_traits>

#include <cpp/lib_concepts>

#include "../../lib/literals.hpp"
#include "../../lib/system/serialize_error.hpp"

#include "./stream.hpp"

// DECLARATION lib::tag_serializeable

namespace lib {

class tag_serializeable {
public:
	virtual ~tag_serializeable() = default;

	virtual data::result_t serialized_size( data::wstream_t & stream ) const = 0;
	virtual bool can_deserialize( data::rstream_t & stream ) const = 0;
	virtual data::result_t serialize( data::wstream_t & stream ) const = 0;
	virtual data::result_t deserialize( data::rstream_t & stream ) = 0;
	virtual data::result_t deserialize( void * /*context*/, data::rstream_t & stream ) { return deserialize( stream ); }
};

} // namespace lib

namespace lib::data {

// DEFINITION lib::data::serialized_size<>

template< ::cpp::Trivial T >requires( not ::cpp::Pointer<T> )
constexpr inline result_t serialized_size( wstream_t &/* stream*/, const T & value )
{ return sizeof(value); }

template< ::cpp::Void T >
constexpr inline result_t serialized_size()
{ return 0_sz; }

template< class T >
constexpr inline result_t serialized_size( wstream_t & stream, const T * value )
{ return value != nullptr ? serialized_size( stream, *value ) : result_t {make_serialize_error_null_data()}; }

template< class T >requires( ::std::is_base_of_v<tag_serializeable, T> )
constexpr inline result_t serialized_size( wstream_t & stream, const T & value )
{ return value.serialized_size( stream ); }

template< class T, class...Args >requires( sizeof...(Args) > 0 )
constexpr inline result_t serialized_size( wstream_t & stream, const T & value, const Args &...others ) {
	const auto & value_sz = serialized_size( stream, value );
	if ( value_sz.failed() ) return value_sz;
	const auto & others_sz = serialized_size( stream, others... );
	if ( others_sz.failed() ) return others_sz;
	return value_sz + others_sz;
}

// DEFINITION lib::data::can_deserialize<>

template< ::cpp::Trivial T >requires( not ::cpp::Pointer<T> )
constexpr inline bool can_deserialize( rstream_t & stream, const T & value ) {
	const auto & read_size = stream.read_size();
	return read_size.success() and read_size >= sizeof(value);
}

template< ::cpp::Void T >
constexpr inline bool can_deserialize()
{ return true; }

template< class T >
constexpr inline bool can_deserialize( rstream_t & stream, const T * value )
{ return value != nullptr ? can_deserialize( stream, *value ) : false; }

template< class T >requires( ::std::is_base_of_v<tag_serializeable, T> )
constexpr inline bool can_deserialize( rstream_t & stream, const T & value )
{ return value.can_deserialize( stream ); }

template< class T, class...Args >requires( sizeof...(Args) > 0 )
constexpr inline bool can_deserialize( rstream_t & stream, const T & value, const Args &...others )
{ return can_deserialize( stream, value ) and can_deserialize( stream, others... ); }

// DEFINITION lib::data::serialize<>

template< ::cpp::Trivial T >requires( not ::cpp::Pointer<T> )
constexpr inline result_t serialize( wstream_t & stream, const T & value ) {
	const auto & write_size = serialized_size( stream, value );
	return write_size.success() ? stream.write({ &value, write_size.value() }) : write_size;
}

template< ::cpp::Void T >
constexpr inline result_t serialize()
{ return 0_sz; }

template< class T >
constexpr inline result_t serialize( wstream_t & stream, const T * value )
{ return value != nullptr ? serialize( stream, *value ) : result_t {make_serialize_error_null_data()}; }

template< class T >requires( ::std::is_base_of_v<tag_serializeable, T> )
constexpr inline result_t serialize( wstream_t & stream, const T & value )
{ return value.serialize( stream ); }

template< class T, class...Args >requires( sizeof...(Args) > 0 )
constexpr inline result_t serialize( wstream_t & stream, const T & value, const Args &...others ) {
	const auto & value_sz = serialize( stream, value );
	if ( value_sz.failed() ) return value_sz;
	const auto & others_sz = serialize( stream, others... );
	if ( others_sz.failed() ) return others_sz;
	return value_sz + others_sz;
}

// DEFINITION lib::data::deserialize<>

template< ::cpp::Trivial T >requires( not ::cpp::Pointer<T> )
constexpr inline result_t deserialize( rstream_t & stream, T & value )
{ return stream.read({ &value, sizeof(value) }); }

template< ::cpp::Void T >
constexpr inline result_t deserialize()
{ return 0_sz; }

template< class T >
constexpr inline result_t deserialize( rstream_t & stream, T * value )
{ return value != nullptr ? deserialize( stream, *value ) : result_t {make_serialize_error_null_data()}; }

template< class T >requires( ::std::is_base_of_v<tag_serializeable, T> )
constexpr inline result_t deserialize( rstream_t & stream, T & value )
{ return value.deserialize( stream ); }

template< class T, class...Args >requires( sizeof...(Args) > 0 )
constexpr inline result_t deserialize( rstream_t & stream, T & value, Args &...others ) {
	const auto & value_sz = deserialize( stream, value );
	if ( value_sz.failed() ) return value_sz;
	const auto & others_sz = deserialize( stream, others... );
	if ( others_sz.failed() ) return others_sz;
	return value_sz + others_sz;
}

// DEFINITION lib::data::deserialize<>( context )

template< ::cpp::Trivial T >requires( not ::cpp::Pointer<T> )
constexpr inline result_t deserialize( void * /*context*/, rstream_t & stream, T & value )
{ return deserialize( stream, value ); }

template< class T >
constexpr inline result_t deserialize( void * context, rstream_t & stream, T * value )
{ return value != nullptr ? deserialize( context, stream, *value ) : result_t {make_serialize_error_null_data()}; }

template< class T >requires( ::std::is_base_of_v<tag_serializeable, T> )
constexpr inline result_t deserialize( void * context, rstream_t & stream, T & value )
{ return value.deserialize( context, stream ); }

template< class T, class...Args >requires( sizeof...(Args) > 0 )
constexpr inline result_t deserialize( void * context, rstream_t & stream, T & value, Args &...others ) {
	const auto & value_sz = deserialize( context, stream, value );
	if ( value_sz.failed() ) return value_sz;
	const auto & others_sz = deserialize( context, stream, others... );
	if ( others_sz.failed() ) return others_sz;
	return value_sz + others_sz;
}

} // namespace lib::data

#endif // CPPLIB__lib__data__serialize__hpp
