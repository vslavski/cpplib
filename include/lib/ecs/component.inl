/* File: /lib/ecs/component.inl
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__component__inl
#define CPPLIB__lib__ecs__component__inl

#include <cpp/lib_debug>

#include "../../lib/utils/enum.hpp"

namespace lib::ecs {

// DECLARATION lib::ecs::detail::Component<>

namespace detail {

template< class T, class Impl >
struct Component final : Impl {
	constexpr Component() noexcept : Impl{ T::TYPE } {}
	T object;
};

} // namespace detail

// INLINES lib::ecs::ComponentBase<>

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
template< class T >
inline constexpr Ptr<Component>
ComponentBase<EntityId_, ComponentType, Component>::create() noexcept
{ return MkPtr< detail::Component<T, Component> >(); }

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
inline constexpr ComponentBase<EntityId_, ComponentType, Component>::ComponentBase( ComponentType type ) noexcept
	: type{ type }
{ CPP_ASSERT( utils::is_valid( type ) ); }

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
template< class T >
inline constexpr T & ComponentBase<EntityId_, ComponentType, Component>::as() noexcept {
	auto * ptr = object<T>();
	CPP_ASSERT( ptr != nullptr );
	return *ptr;
}

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
template< class T >
inline constexpr const T & ComponentBase<EntityId_, ComponentType, Component>::as() const noexcept {
	const auto * ptr = object<T>();
	CPP_ASSERT( ptr != nullptr );
	return *ptr;
}

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
template< class T >
inline constexpr T * ComponentBase<EntityId_, ComponentType, Component>::object() noexcept {
	if ( type != T::TYPE )
		return nullptr;
	return &static_cast< detail::Component<T, Component> * >( this )->object;
}

template< ::cpp::Integral EntityId_, ::cpp::Enumeration ComponentType, class Component >
template< class T >
inline constexpr const T * ComponentBase<EntityId_, ComponentType, Component> ::object() const noexcept {
	if ( type != T::TYPE )
		return nullptr;
	return &static_cast< const detail::Component<T, Component> * >( this )->object;
}

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__component__inl
