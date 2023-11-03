/* File: /lib/ecs/entity.inl
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__entity__inl
#define CPPLIB__lib__ecs__entity__inl

#include <cpp/lib_debug>

#include "../../lib/utils/enum.hpp"

namespace lib::ecs {

// INLINES lib::ecs::Entity<>

template< class Component >
inline constexpr Entity<Component>::Entity( Manager<Component> & manager, Id id ) noexcept
	: manager{ manager }
	, id{ id }
{}

template< class Component >
inline constexpr Entity<Component>::~Entity() noexcept
{ CPP_ASSERT( components.empty() ); }

template< class Component >
inline constexpr bool Entity<Component>::operator < ( const Entity & other ) const noexcept
{ return id < other.id; }

template< class Component >
inline constexpr const typename Entity<Component>::ComponentList &
Entity<Component>::get_components() const noexcept
{ return components; }

template< class Component >
inline constexpr void
Entity<Component>::clear_components() noexcept {
	utils::foreach< typename Component::Type >(
		[this]( typename Component::Type type, utils::EnumId< typename Component::Type > id ) {
			/*[[maybe_unused]]*/CPP_UNUSED( id );
			CPP_UNUSED( remove( type ) );
			return true;
		});
}

template< class Component >
inline constexpr typename Entity<Component>::ComponentPtr
Entity<Component>::add( typename Component::Type type ) noexcept
{ return manager.add( *this, type ); }

template< class Component >
inline constexpr typename Entity<Component>::ComponentPtr
Entity<Component>::add( const cstring & name ) noexcept {
	const auto type = unstr< typename Component::Type >( name );
	return utils::is_valid( type ) ? add( type ) : nullptr;
}

template< class Component >
template< class T >
inline constexpr T &
Entity<Component>::add() noexcept {
	auto * component = add( T::TYPE );
	CPP_ASSERT( component != nullptr );
	return component->template as<T>();
}

template< class Component >
inline constexpr bool
Entity<Component>::remove( typename Component::Type type ) noexcept
{ return manager.remove( *this, type ); }

template< class Component >
inline constexpr bool
Entity<Component>::remove( const cstring & name ) noexcept {
	const auto type = unstr< typename Component::Type >( name );
	return utils::is_valid( type ) ? remove( type ) : false;
}

template< class Component >
template< class T >
inline constexpr bool
Entity<Component>::remove() noexcept
{ return remove( T::TYPE ); }

template< class Component >
inline constexpr typename Entity<Component>::ComponentPtr
Entity<Component>::get( typename Component::Type type ) const noexcept {
	const auto & it = components.find( type );
	return it != components.end() ? &it->second.get() : nullptr;
}

template< class Component >
inline constexpr typename Entity<Component>::ComponentPtr
Entity<Component>::get( const cstring & name ) const noexcept
{ return get( unstr< typename Component::Type >( name ) ); }

template< class Component >
template< class T >
inline constexpr T *
Entity<Component>::get() const noexcept {
	if ( auto * component = get( T::TYPE ) )
		return component->template object<T>();
	return nullptr;
}

template< class Component >
inline constexpr bool
Entity<Component>::has( typename Component::Type type ) const noexcept
{ return get( type ) != nullptr; }

template< class Component >
inline constexpr bool
Entity<Component>::has( const cstring & name ) const noexcept
{ return has( unstr< typename Component::Type >( name ) ); }

template< class Component >
template< class T >
inline constexpr bool
Entity<Component>::has() const noexcept
{ return has( T::TYPE ); }

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__entity__inl
