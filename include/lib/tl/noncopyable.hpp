/* File: /lib/tl/noncopyable.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__noncopyable__hpp
#define CPPLIB__lib__tl__noncopyable__hpp

#include "../../lib/debug/features.hpp"

namespace lib::tl {

// DECLARATION lib::tl::noncopyable<>

template< class T >
class noncopyable {
protected:
	constexpr noncopyable() noexcept = default;
	constexpr ~noncopyable() noexcept = default;
	noncopyable( const noncopyable & ) = delete;
	noncopyable & operator = ( const noncopyable & ) = delete;
};

// DECLARATION lib::tl::nonmovable<>

template< class T >
class nonmovable {
protected:
	constexpr nonmovable() noexcept = default;
	constexpr ~nonmovable() noexcept = default;
	nonmovable( nonmovable && ) = delete;
	nonmovable & operator = ( nonmovable && ) = delete;
};

// DECLARATION lib::tl::noncopymovable<>

template< class T >
class CPPLIB_ENABLE_EBO noncopymovable
	: protected noncopyable< T >
	, protected nonmovable< T >
{};

namespace detail {

struct EBO_noncopymovable_check
	: noncopymovable< EBO_noncopymovable_check >
{
	const void * check;
};
static_assert( sizeof(EBO_noncopymovable_check) == sizeof(EBO_noncopymovable_check::check)
	, "Empty Base Optimization not applied for noncopymovable<>." );

} // namespace detail

} // namespace lib::tl

#endif // CPPLIB__lib__tl__noncopyable__hpp
