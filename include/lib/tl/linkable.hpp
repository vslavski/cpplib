/* File: /lib/tl/linkable.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__linkable__hpp
#define CPPLIB__lib__tl__linkable__hpp

#include <type_traits>

#include <cpp/lib_debug>

#include "../../lib/tl/noncopyable.hpp"

namespace lib::tl {

// DECLARATION lib::tl::linkable<>

template< class Self, class LinkWith >
class linkable
	: noncopymovable< linkable<Self, LinkWith> >
{
public:
	constexpr bool link( LinkWith & link_ ) noexcept;
	constexpr bool unlink( LinkWith & link_ ) noexcept;
protected:
	constexpr linkable() noexcept;
	constexpr ~linkable() noexcept;

	template< class T = LinkWith >
	constexpr T * get_link() const noexcept;
private:
	constexpr LinkWith * get_link_( LinkWith * ) const noexcept;
	LinkWith * link_ = nullptr;
};

// INLINES lib::tl::linkable<>

template< class Self, class LinkWith >
inline constexpr bool linkable<Self, LinkWith>::link( LinkWith & link_with ) noexcept {
	static_assert(
		::std::is_base_of_v< linkable<Self, LinkWith>, Self >
		and
		::std::is_base_of_v< linkable<LinkWith, Self>, LinkWith >
		);
	if ( link_ == &link_with )
		return true;
	if ( link_ != nullptr )
		return false;
	link_ = &link_with;
	if ( link_with.link( static_cast<Self&>( *this ) ) )
		return true;
	link_ = nullptr;
	return false;
}

template< class Self, class LinkWith >
inline constexpr bool linkable<Self, LinkWith>::unlink( LinkWith & link_with ) noexcept {
	if ( link_ != &link_with )
		return false;
	link_ = nullptr;
	CPP_UNUSED( link_with.unlink( static_cast<Self&>( *this ) ) );
	return true;
}

template< class Self, class LinkWith >
inline constexpr linkable<Self, LinkWith>::linkable() noexcept
	: link_{}
{}

template< class Self, class LinkWith >
inline constexpr linkable<Self, LinkWith>::~linkable() noexcept {
	if ( link_ != nullptr )
		unlink( *link_ );
}

template< class Self, class LinkWith >
template< class T /*= LinkWith*/ >
inline constexpr T * linkable<Self, LinkWith>::get_link() const noexcept {
	return get_link_( (T*)nullptr );
}

template< class Self, class LinkWith >
inline constexpr LinkWith * linkable<Self, LinkWith>::get_link_( LinkWith * ) const noexcept {
	return link_;
}

} // namespace lib::tl

#endif // CPPLIB__lib__tl__linkable__hpp
