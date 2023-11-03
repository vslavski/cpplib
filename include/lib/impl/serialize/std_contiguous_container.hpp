/* File: /lib/impl/serialize/std_contiguous_container.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__serialize__std_contiguous_container__hpp
#define CPPLIB__lib__impl__serialize__std_contiguous_container__hpp

#include <cpp/lib_concepts>
#include <cpp/lib_debug>

#include "../../../lib/types.hpp"
#include "../../../lib/literals.hpp"
#include "../../../lib/data/stream.hpp"

namespace lib::data {

// IMPLEMENTATION lib::data::serialized_size<>

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Trivial< typename T::value_type >
	and not ::cpp::Pointer< typename T::value_type > )
constexpr inline result_t serialized_size( wstream_t &/* stream*/, const T & value )
{ return sizeof(u32/*data_length*/) + value.size() * sizeof(typename T::value_type); }

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Pointer< typename T::value_type >
	or not ::cpp::Trivial< typename T::value_type > )
constexpr inline result_t serialized_size( wstream_t & stream, const T & value ) {
	auto size = sizeof(u32/*data_length*/);
	for ( const auto & element : value ) {
		const auto & element_size = serialized_size( stream, element );
		if ( element_size.failed() )
			return element_size;
		size += element_size;
	}
	return size;
}

// IMPLEMENTATION lib::data::can_deserialize<>

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Trivial< typename T::value_type >
	and not ::cpp::Pointer< typename T::value_type > )
constexpr inline bool can_deserialize( rstream_t & stream, const T & ) {
	u32 data_length;
	const auto & peek_result = stream.peek({ &data_length, sizeof(data_length) });
	if ( peek_result.failed() or peek_result != sizeof(data_length) )
		return false;
	const auto & read_size = stream.read_size();
	return read_size.success() and read_size >= sizeof(data_length) + data_length * sizeof(typename T::value_type);
}

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Pointer< typename T::value_type >
	or not ::cpp::Trivial< typename T::value_type > )
constexpr inline bool can_deserialize( rstream_t & stream, const T &/* value*/ ) {
	const auto & read_size = stream.read_size();
	return read_size.success() and read_size >= sizeof(u32/*data_length*/);
}

// IMPLEMENTATION lib::data::serialize<>

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Trivial< typename T::value_type >
	and not ::cpp::Pointer< typename T::value_type > )
constexpr inline result_t serialize( wstream_t & stream, const T & value ) {
	const u32 data_length = (u32) value.size();
	auto written_bytes = stream.write({ &data_length, sizeof(data_length) });
	if ( written_bytes != sizeof(data_length) )
		return written_bytes;
	written_bytes = stream.write({ value.data(), data_length * sizeof(typename T::value_type) });
	if ( written_bytes != data_length * sizeof(typename T::value_type) )
		return written_bytes;
	written_bytes += sizeof(data_length);
	CPP_ASSERT( written_bytes == serialized_size( stream, value ) );
	return written_bytes;
}

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Pointer< typename T::value_type >
	or not ::cpp::Trivial< typename T::value_type > )
constexpr inline result_t serialize( wstream_t & stream, const T & value ) {
	const u32 data_length = (u32) value.size();
	auto written_bytes = stream.write({ &data_length, sizeof(data_length) });
	if ( written_bytes != sizeof(data_length) )
		return written_bytes;
	for ( const auto & element : value ) {
		const auto & element_written = serialize( stream, element );
		if ( element_written.failed() or element_written == 0_sz )
			/** @note Can't be zero, because elements counting is
			 * required to support container serialization. */
			return element_written;
		written_bytes += element_written;
	}
	return written_bytes;
}

// IMPLEMENTATION lib::data::deserialize<>

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Trivial< typename T::value_type >
	and not ::cpp::Pointer< typename T::value_type > )
constexpr inline result_t deserialize( rstream_t & stream, T & value ) {
	u32 data_length;
	auto readen_bytes = stream.read({ &data_length, sizeof(data_length) });
	if ( readen_bytes != sizeof(data_length) )
		return readen_bytes;
	value.resize( data_length );
	readen_bytes = stream.read({ value.data(), data_length * sizeof(typename T::value_type) });
	if ( readen_bytes != data_length * sizeof(typename T::value_type) )
		return readen_bytes;
	return sizeof(data_length) + readen_bytes;
}

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Pointer< typename T::value_type >
	or not ::cpp::Trivial< typename T::value_type > )
constexpr inline result_t deserialize( rstream_t & stream, T & value ) {
	u32 data_length;
	auto readen_bytes = stream.read({ &data_length, sizeof(data_length) });
	if ( readen_bytes != sizeof(data_length) )
		return readen_bytes;
	value.resize( data_length );
	for ( auto & element : value ) {
		const auto & element_readen = deserialize( stream, element );
		if ( element_readen.failed() or element_readen == 0_sz )
			/** @note Can't be zero, because elements counting is
			 * required to support container deserialization. */
			return element_readen;
		readen_bytes += element_readen;
	}
	return readen_bytes;
}

template< ::cpp::ContiguousContainer T, /*::cpp::Invocable*/class Fn >
requires requires ( rstream_t & s, T & v, const Fn & fn )
	{ {fn( s, v[0] )} -> ::cpp::SameAs<result_t>; }
constexpr inline result_t deserialize( rstream_t & stream, T & value, Fn fn ) {
	u32 data_length;
	auto readen_bytes = stream.read({ &data_length, sizeof(data_length) });
	if ( readen_bytes != sizeof(data_length) )
		return readen_bytes;
	value.resize( data_length );
	for ( auto & element : value ) {
		const auto & element_readen = fn( stream, element );
		if ( element_readen.failed() or element_readen == 0_sz )
			/** @note Can't be zero, because elements counting is
			 * required to support container deserialization. */
			return element_readen;
		readen_bytes += element_readen;
	}
	return readen_bytes;
}

// IMPLEMENTATION lib::data::deserialize<>( context )

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Trivial< typename T::value_type >
	and not ::cpp::Pointer< typename T::value_type > )
constexpr inline result_t deserialize( void * /*context*/, rstream_t & stream, T & value ) {
	return deserialize( stream, value );
}

template< ::cpp::ContiguousContainer T >
	requires( ::cpp::Pointer< typename T::value_type >
	or not ::cpp::Trivial< typename T::value_type > )
constexpr inline result_t deserialize( void * context, rstream_t & stream, T & value ) {
	u32 data_length;
	auto readen_bytes = stream.read({ &data_length, sizeof(data_length) });
	if ( readen_bytes != sizeof(data_length) )
		return readen_bytes;
	value.resize( data_length );
	for ( auto & element : value ) {
		const auto & element_readen = deserialize( context, stream, element );
		if ( element_readen.failed() or element_readen == 0_sz )
			/** @note Can't be zero, because elements counting is
			 * required to support container deserialization. */
			return element_readen;
		readen_bytes += element_readen;
	}
	return readen_bytes;
}

template< ::cpp::ContiguousContainer T, /*::cpp::Invocable*/class Fn >
requires requires ( void * c, rstream_t & s, T & v, const Fn & fn )
	{ {fn( c, s, v[0] )} -> ::cpp::SameAs<result_t>; }
constexpr inline result_t deserialize( void * context, rstream_t & stream, T & value, Fn fn ) {
	u32 data_length;
	auto readen_bytes = stream.read({ &data_length, sizeof(data_length) });
	if ( readen_bytes != sizeof(data_length) )
		return readen_bytes;
	value.resize( data_length );
	for ( auto & element : value ) {
		const auto & element_readen = fn( context, stream, element );
		if ( element_readen.failed() or element_readen == 0_sz )
			/** @note Can't be zero, because elements counting is
			 * required to support container deserialization. */
			return element_readen;
		readen_bytes += element_readen;
	}
	return readen_bytes;
}

} // namespace lib::data

#endif // CPPLIB__lib__impl__serialize__std_contiguous_container__hpp
