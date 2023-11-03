/* File: /lib/ecs/manager.inl
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__manager__inl
#define CPPLIB__lib__ecs__manager__inl

#include <cpp/lib_debug>

#include <limits>

#include "../../lib/utils/enum.hpp"

namespace lib::ecs {

// INLINES lib::ecs::Manager<>

template< class Component >
inline constexpr Manager<Component>::Manager() noexcept {}

template< class Component >
inline constexpr Manager<Component>::~Manager() noexcept {
	CPP_ASSERT( entities.empty() );
	CPP_ASSERT( components.empty() );
}

template< class Component >
inline constexpr const typename Manager<Component>::EntityList &
Manager<Component>::get_entities() const noexcept
{ return entities; }

template< class Component >
inline constexpr const typename Manager<Component>::ComponentList &
Manager<Component>::get_components( typename Component::Type type ) const noexcept {
	const auto & it = components.find( type );
	CPP_ASSERT( it != components.end() );
	return it->second;
}

template< class Component >
inline constexpr void
Manager<Component>::reset() noexcept {
	for ( auto & it : entities )
		it.second->components.clear();
	entities.clear();
	components.clear();
}

template< class Component >
inline constexpr typename Manager<Component>::EntityPtr
Manager<Component>::add( typename EntityType::Id id/* = EntityType::INVALID_ID*/ ) noexcept {
	static_assert( EntityType::INVALID_ID == 0 );
	id = /*find suitable id*/([&](){
		if ( id != 0 ) {
			CPP_ASSERT( not entities.contains( id ) );
			return id;
		}
		if ( entities.empty() )
			return typename EntityType::Id{} + 1;
		const auto & rit = entities.crbegin();
		CPP_ASSERT( rit->first < ::std::numeric_limits<decltype(rit->first)>::max() );
		return rit->first + 1;
	})();
	const auto & entity = MkSPtr<EntityType>( *this, id );
	entities[id] = entity;
	on_entity_added( *entity );
	return entity;
}

template< class Component >
inline constexpr bool
Manager<Component>::remove( typename EntityType::Id id ) noexcept {
	const auto & it = entities.find( id );
	if ( it == entities.end() )
		return false;
	auto & entity = *it->second;
	on_entity_removed( entity );
	CPP_ASSERT( it->second.use_count() == 1 );
	for ( const auto & [ type, component ] : entity.components )
		components[ type ].erase( entity );
	entity.components.clear();
	entities.erase( it );
	return true;
}

template< class Component >
inline constexpr bool
Manager<Component>::remove( SPtr<EntityType> & entity ) noexcept {
	CPP_ASSERT( entity );
	const auto id = entity->id;
	entity.reset();
	return remove( id );
}

template< class Component >
inline constexpr typename Manager<Component>::EntityPtr
Manager<Component>::get( typename EntityType::Id id ) const noexcept {
	const auto & it = entities.find( id );
	return it != entities.end() ? it->second : EntityPtr{};
}

template< class Component >
inline constexpr typename Manager<Component>::ComponentPtr
Manager<Component>::add( EntityType & entity, typename Component::Type type ) noexcept {
	CPP_ASSERT( this == &entity.manager );
	CPP_ASSERT( entities.contains( entity.id ) );
	CPP_ASSERT( utils::is_valid( type ) );
	auto & container = components[type];
	if ( const auto & it = container.find( entity ); it != container.end() )
		return it->second.get();
	auto & component = *(
		container[entity] = Component::create( type )
		);
	CPP_ASSERT( not entity.components.contains( type ) );
	entity.components.emplace( type, MkRef( component ) );
	on_component_added( entity, component );
	return &component;
}

template< class Component >
inline constexpr typename Manager<Component>::ComponentPtr
Manager<Component>::add( EntityType & entity, const cstring & name ) noexcept
{ return add( entity, unstr< typename Component::Type >( name ) ); }

template< class Component >
inline constexpr bool
Manager<Component>::remove( EntityType & entity, typename Component::Type type ) noexcept {
	CPP_ASSERT( this == &entity.manager );
	CPP_ASSERT( entities.contains( entity.id ) );
	CPP_ASSERT( utils::is_valid( type ) );
	auto it = components.find( type );
	if ( it == components.end() )
		return false;
	auto & container = it->second;
	auto cit = container.find( entity );
	if ( cit == container.end() )
		return false;
	entity.components.erase( type );
	on_component_removed( entity, *cit->second );
	container.erase( entity );
	if ( container.empty() )
		components.erase( type );
	return true;
}

template< class Component >
inline constexpr bool
Manager<Component>::remove( EntityType & entity, const cstring & name ) noexcept
{ return remove( entity, unstr< typename Component::Type >( name ) ); }

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__manager__inl
