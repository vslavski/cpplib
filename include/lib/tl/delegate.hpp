/* File: /lib/tl/delegate.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__delegate__hpp
#define CPPLIB__lib__tl__delegate__hpp

#include <algorithm>
#include <optional>
#include <utility>
#include <iterator>
#include <list>

#include <cpp/lib_debug>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/listener.hpp"

/// @todo	Review.
/// @note	With no RTTI support we can't use ::std::function<> objects,
/// 		'cause comparison is required to support Listener::operator == ().
namespace lib::tl {

// DECLARATION lib::tl::delegate<>

template< class...Args >
class delegate final
	: noncopymovable< delegate<Args...> >
{
public:
	using Listener = listener<void, Args...>;
	constexpr void operator () ( Args...args ) noexcept;
	constexpr delegate & operator += ( Listener && listener_ ) noexcept;
	constexpr delegate & operator -= ( const Listener & listener_ ) noexcept;
	constexpr void clear() noexcept;
	constexpr bool empty() const noexcept;
private:
	::std::list<Listener> listeners;

	using Iterator = typename decltype(listeners)::iterator;
	::std::optional<Iterator> current_invocation;
	::std::optional<Iterator> next_invocation;
};

// INLINES lib::tl::delegate<>

template< class...Args >
inline constexpr void delegate<Args...>::operator () ( Args...args ) noexcept {
	CPP_ASSERT( not current_invocation );
	current_invocation = listeners.begin();
	while ( current_invocation and *current_invocation != listeners.end() ) {
		next_invocation = ::std::next( *current_invocation );
		auto & listener_ = **current_invocation;
		listener_( ::std::forward<Args>(args)... );
		if ( current_invocation and not listener_ )
			next_invocation = listeners.erase( *current_invocation );
		current_invocation = next_invocation;
	}
	current_invocation = {};
	next_invocation = {};
}

template< class...Args >
inline constexpr delegate<Args...> & delegate<Args...>::operator += ( Listener && listener ) noexcept {
	/// @note Can be invoked if delegate is "on invocation".
	listeners.emplace_back( ::std::forward<Listener>( listener ) );
	if ( current_invocation ) {
		const auto new_listener = ::std::next( listeners.rbegin() ).base();
		if ( ::std::next( *current_invocation ) == new_listener )
			next_invocation = new_listener;
	}
	return *this;
}

template< class...Args >
inline constexpr delegate<Args...> & delegate<Args...>::operator -= ( const Listener & listener ) noexcept {
	const auto rit = ::std::find( listeners.rbegin(), listeners.rend(), listener );
	CPP_ASSERT( rit != listeners.rend() );
	const auto it = ::std::next( rit ).base();
	if ( current_invocation and *current_invocation == it )
		current_invocation = {};
	if ( next_invocation and *next_invocation == it )
		next_invocation = ::std::next( it );
	listeners.erase( it );
	return *this;
}

template< class...Args >
inline constexpr void delegate<Args...>::clear() noexcept {
	CPP_ASSERT( not current_invocation );
	current_invocation = {};
	next_invocation = {};
	listeners.clear();
}

template< class...Args >
inline constexpr bool delegate<Args...>::empty() const noexcept {
	return listeners.empty();
}

} // namespace lib::tl

#endif // CPPLIB__lib__tl__delegate__hpp
