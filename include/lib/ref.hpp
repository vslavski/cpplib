/* File: /lib/ref.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ref__hpp
#define CPPLIB__lib__ref__hpp

#include <functional>

namespace lib {

// DECLARATION lib::Ref, lib::RefC

template< class T > using Ref	= ::std::reference_wrapper< T >;
template< class T > using RefC	= ::std::reference_wrapper< const T >;

// DECLARATION lib::MkRef, lib::MkRefC

template< class T >
inline constexpr auto MkRef( T & ref ) noexcept
{ return ::std::ref<T>( ref ); }

template< class T >
inline constexpr auto MkRefC( const T & ref ) noexcept
{ return ::std::cref<T>( ref ); }

} // namespace lib

#endif // CPPLIB__lib__ref__hpp
