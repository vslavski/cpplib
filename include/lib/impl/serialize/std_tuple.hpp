/* File: /lib/impl/serialize/std_tuple.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__serialize__std_tuple__hpp
#define CPPLIB__lib__impl__serialize__std_tuple__hpp

#include <tuple>
#include <utility>

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

namespace lib::data {

// IMPLEMENTATION lib::data::serialized_size<>

namespace detail {
template< class...Args, ::std::size_t...I >
constexpr inline result_t serialized_size
( wstream_t & stream, const ::std::tuple<Args...> & value, ::std::index_sequence<I...> ) {
	return serialized_size( stream, ::std::get<I>( value )... );
}
} // namespace detail

template< class...Args >
constexpr inline result_t serialized_size
( wstream_t & stream, const ::std::tuple<Args...> & value ) {
	return detail::serialized_size( stream, value, ::std::index_sequence_for<Args...>{} );
}

// IMPLEMENTATION lib::data::can_deserialize<>

namespace detail {
template< class...Args, ::std::size_t...I >
constexpr inline bool can_deserialize
( rstream_t & stream, const ::std::tuple<Args...> & value, ::std::index_sequence<I...> ) {
	return can_deserialize( stream, ::std::get<I>( value )... );
}
} // namespace detail

template< class...Args >
constexpr inline bool can_deserialize
( rstream_t & stream, const ::std::tuple<Args...> & value ) {
	return detail::can_deserialize( stream, value, ::std::index_sequence_for<Args...>{} );
}

// IMPLEMENTATION lib::data::serialize<>

namespace detail {
template< class...Args, ::std::size_t...I >
constexpr inline result_t serialize
( wstream_t & stream, const ::std::tuple<Args...> & value, ::std::index_sequence<I...> ) {
	return serialize( stream, ::std::get<I>( value )... );
}
} // namespace detail

template< class...Args >
constexpr inline result_t serialize
( wstream_t & stream, const ::std::tuple<Args...> & value ) {
	return detail::serialize( stream, value, ::std::index_sequence_for<Args...>{} );
}

// IMPLEMENTATION lib::data::deserialize<>

namespace detail {
template< class...Args, ::std::size_t...I >
constexpr inline result_t deserialize
( rstream_t & stream, ::std::tuple<Args...> & value, ::std::index_sequence<I...> ) {
	return deserialize( stream, ::std::get<I>( value )... );
}
} // namespace detail

template< class...Args >
constexpr inline result_t deserialize
( rstream_t & stream, ::std::tuple<Args...> & value ) {
	return detail::deserialize( stream, value, ::std::index_sequence_for<Args...>{} );
}

// IMPLEMENTATION lib::data::deserialize<>( context )

namespace detail {
template< class...Args, ::std::size_t...I >
constexpr inline result_t deserialize
( void * context, rstream_t & stream, const ::std::tuple<Args...> & value, ::std::index_sequence<I...> ) {
	return deserialize( context, stream, ::std::get<I>( value )... );
}
} // namespace detail

template< class...Args >
constexpr inline result_t deserialize
( void * context, rstream_t & stream, const ::std::tuple<Args...> & value ) {
	return detail::deserialize( context, stream, value, ::std::index_sequence_for<Args...>{} );
}

} // namespace lib::data

#endif // CPPLIB__lib__impl__serialize__std_tuple__hpp
