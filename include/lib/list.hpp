/* File: /lib/list.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__list__hpp
#define CPPLIB__lib__list__hpp

namespace lib {

// DECLARATION lib::detail::declare_list<>

namespace detail {

template< class T >
struct declare_list {
	using List = typename T::List;
};

} // namespace detail

// DECLARATION lib::List<>

template< class T >
using List = typename detail::declare_list<T>::List;

} // namespace lib

#endif // CPPLIB__lib__list__hpp
