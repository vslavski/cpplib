/* File: /lib/impl/serialize/ptr.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__serialize__ptr__hpp
#define CPPLIB__lib__impl__serialize__ptr__hpp

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

#include "../../../lib/ptr.hpp"

namespace lib::data {

// IMPLEMENTATION lib::data::serialized_size<>

template< class T >
constexpr inline result_t serialized_size( wstream_t & stream, const Ptr<T> & value )
{ return serialized_size( stream, value.get() ); }

template< class T >
constexpr inline result_t serialized_size( wstream_t & stream, const SPtr<T> & value )
{ return serialized_size( stream, value.get() ); }

template< class T >
constexpr inline result_t serialized_size( wstream_t & stream, const WPtr<T> & value )
{ return serialized_size( stream, value.lock() ); }

// IMPLEMENTATION lib::data::can_deserialize<>

template< class T >
constexpr inline bool can_deserialize( rstream_t & stream, const Ptr<T> & value )
{ return can_deserialize( stream, value.get() ); }

template< class T >
constexpr inline bool can_deserialize( rstream_t & stream, const SPtr<T> & value )
{ return can_deserialize( stream, value.get() ); }

template< class T >
constexpr inline bool can_deserialize( rstream_t & stream, const WPtr<T> & value )
{ return can_deserialize( stream, value.lock() ); }

// IMPLEMENTATION lib::data::serialize<>

template< class T >
constexpr inline result_t serialize( wstream_t & stream, const Ptr<T> & value )
{ return serialize( stream, value.get() ); }

template< class T >
constexpr inline result_t serialize( wstream_t & stream, const SPtr<T> & value )
{ return serialize( stream, value.get() ); }

template< class T >
constexpr inline result_t serialize( wstream_t & stream, const WPtr<T> & value )
{ return serialize( stream, value.lock() ); }

// IMPLEMENTATION lib::data::deserialize<>

template< class T >
constexpr inline result_t deserialize( rstream_t & stream, const Ptr<T> & value )
{ return deserialize( stream, value.get() ); }

template< class T >
constexpr inline result_t deserialize( rstream_t & stream, const SPtr<T> & value )
{ return deserialize( stream, value.get() ); }

template< class T >
constexpr inline result_t deserialize( rstream_t & stream, const WPtr<T> & value )
{ return deserialize( stream, value.lock() ); }

// IMPLEMENTATION lib::data::deserialize<>( context )

template< class T >
constexpr inline result_t deserialize( void * context, rstream_t & stream, const Ptr<T> & value )
{ return deserialize( context, stream, value.get() ); }

template< class T >
constexpr inline result_t deserialize( void * context, rstream_t & stream, const SPtr<T> & value )
{ return deserialize( context, stream, value.get() ); }

template< class T >
constexpr inline result_t deserialize( void * context, rstream_t & stream, const WPtr<T> & value )
{ return deserialize( context, stream, value.lock() ); }

template< class T >
constexpr inline result_t deserialize( void * context, rstream_t & stream, WPtr<T> & value ) {
	SPtr<T> sptr_value;
	const auto & value_bytes = deserialize( context, stream, sptr_value );
	if ( not sptr_value or value_bytes.failed() )
		return value_bytes;
	value = sptr_value;
	return value_bytes;
}

} // namespace lib::data

#endif // CPPLIB__lib__impl__serialize__ptr__hpp
