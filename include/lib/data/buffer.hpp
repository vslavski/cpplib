/* File: /lib/data/buffer.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__data__buffer__hpp
#define CPPLIB__lib__data__buffer__hpp

#include <algorithm>
#include <span>

#include "../../lib/types.hpp"

namespace lib::data {

namespace detail {

	template< class Buffer >
	concept CBufferLike = requires( const Buffer & b ) {
		(void*) b.data();
		(usize) b.size();
		sizeof(typename Buffer::value_type);
	};

	template< class Buffer >
	concept BufferLike = requires( Buffer & b ) {
		*b.data() = {};
		(usize) b.size();
		sizeof(typename Buffer::value_type);
	};

	template< class...Args >
	concept SpanConstU8Constructable = requires( Args...args ) {
		::std::span<const u8>{ args... };
	};

	template< class...Args >
	concept SpanU8Constructable = requires( Args...args ) {
		::std::span<u8>{ args... };
	};

} // namespace detail

// DECLARATION lib::data::cbuffer_t

struct cbuffer_t
	: ::std::span<const u8>
{
	constexpr cbuffer_t( const void * data, usize size ) noexcept
		: ::std::span<const u8>{ (const u8*) data, size }
	{}

	template< class B > requires detail::CBufferLike<B>
	constexpr cbuffer_t( const B & buffer ) noexcept
		: cbuffer_t{ buffer.data(), buffer.size() * sizeof(typename B::value_type) }
	{}

	template< class...Args > requires detail::SpanU8Constructable<Args...>
	constexpr cbuffer_t( Args...args ) noexcept
		: ::std::span<const u8>{ args... }
	{}

	template< class B > requires detail::CBufferLike<B>
	constexpr bool operator == ( const B & buffer ) const noexcept {
		const cbuffer_t cmp_buff{ buffer };
		return ::std::equal( begin(), end(), cmp_buff.begin(), cmp_buff.end() );
	}
};

// DECLARATION lib::data::buffer_t

struct buffer_t
	: ::std::span<u8>
{
	constexpr buffer_t( void * data, usize size ) noexcept
		: ::std::span<u8>{ (u8*) data, size }
	{}

	template< class B > requires detail::BufferLike<B>
	constexpr buffer_t( B & buffer ) noexcept
		: buffer_t{ buffer.data(), buffer.size() * sizeof(typename B::value_type) }
	{}

	template< class...Args > requires detail::SpanU8Constructable<Args...>
	constexpr buffer_t( Args...args ) noexcept
		: ::std::span<u8>{ args... }
	{}

	template< class B > requires detail::CBufferLike<B>
	constexpr bool operator == ( const B & buffer ) const noexcept {
		const cbuffer_t cmp_buff{ buffer };
		return ::std::equal( begin(), end(), cmp_buff.begin(), cmp_buff.end() );
	}
};

} // namespace lib::data

#endif // CPPLIB__lib__data__buffer__hpp
