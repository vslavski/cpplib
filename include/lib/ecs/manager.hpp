/* File: /lib/ecs/manager.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__manager__hpp
#define CPPLIB__lib__ecs__manager__hpp

#include <map>

#include <cpp/lib_algorithm>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/listener.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/ptr.hpp"
#include "../../lib/ref.hpp"

#include "./entity.hpp"

namespace lib::ecs {

// DECLARATION lib::ecs::Manager<>

template< class Component >
class Manager final
	: tl::noncopymovable< Manager<Component> >
{
public:
	using EntityType = Entity< Component >;
	using EntityPtr = SPtr< EntityType >;
	using EntityList = ::std::map< typename EntityType::Id, EntityPtr >;
	using ComponentPtr = Component *;
	using ComponentMeta = Ref< EntityType >;
	using ComponentList = ::std::map< ComponentMeta, Ptr<Component>, ::cpp::ref_less<ComponentMeta> >;

	constexpr Manager() noexcept;
	constexpr ~Manager() noexcept;

	constexpr const EntityList & get_entities() const noexcept;
	constexpr const ComponentList & get_components( typename Component::Type type ) const noexcept;
	constexpr void reset() noexcept;

	constexpr EntityPtr add( typename EntityType::Id id = EntityType::INVALID_ID ) noexcept;
	constexpr bool remove( typename EntityType::Id id ) noexcept;
	constexpr bool remove( SPtr<EntityType> & entity ) noexcept;
	constexpr EntityPtr get( typename EntityType::Id id ) const noexcept;

	constexpr ComponentPtr add( EntityType & entity, typename Component::Type type ) noexcept;
	constexpr ComponentPtr add( EntityType & entity, const cstring & name ) noexcept;
	constexpr bool remove( EntityType & entity, typename Component::Type type ) noexcept;
	constexpr bool remove( EntityType & entity, const cstring & name ) noexcept;

	tl::listener<void, EntityType & > on_entity_added;
	tl::listener<void, EntityType & > on_entity_removed;
	tl::listener<void, EntityType &, Component & > on_component_added;
	tl::listener<void, EntityType &, Component & > on_component_removed;
private:
	::std::map< typename Component::Type, ComponentList > components;
	EntityList entities;
};

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__manager__hpp
