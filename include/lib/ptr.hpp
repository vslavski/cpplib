/* File: /lib/ptr.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ptr__hpp
#define CPPLIB__lib__ptr__hpp

#include <memory>
#include <utility>

namespace lib {

// DECLARATION lib::Ptr, lib::PtrC

template< class T, class...Args > using Ptr		= ::std::unique_ptr< T, Args... >;
template< class T, class...Args > using PtrC	= ::std::unique_ptr< const T, Args... >;

// DECLARATION lib::SPtr, lib::SPtrC

template< class T > using SPtr					= ::std::shared_ptr< T >;
template< class T > using SPtrC					= ::std::shared_ptr< const T >;

// DECLARATION lib::WPtr, lib::WPtrC

template< class T > using WPtr					= ::std::weak_ptr< T >;
template< class T > using WPtrC					= ::std::weak_ptr< const T >;

// DECLARATION lib::MkPtr, lib::MkSPtr

template< class T, class...Args > inline auto MkPtr( Args&&...args )
{ return ::std::make_unique<T>( ::std::forward<Args>(args)... ); }

template< class T, class...Args > inline auto MkSPtr( Args&&...args )
{ return ::std::make_shared<T>( ::std::forward<Args>(args)... ); }

} // namespace lib

#endif // CPPLIB__lib__ptr__hpp
