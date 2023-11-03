/* File: /lib/ecs/entity.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__entity__hpp
#define CPPLIB__lib__ecs__entity__hpp

#include <map>
#include <memory>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/ref.hpp"

namespace lib::ecs {

// PREDECLARATIONS

template< class Component >
class Manager;

// DECLARATION lib::ecs::Entity<>

template< class Component >
class Entity final
	: tl::noncopymovable< Entity<Component> >
	, public ::std::enable_shared_from_this< Entity<Component> >
{
	friend class Manager<Component>;
	Manager<Component> & manager;
public:
	using Id = typename Component::EntityId;
	using ComponentPtr = Component *;
	using ComponentList = ::std::map< typename Component::Type, Ref<Component> >;

	static constexpr auto INVALID_ID = Id{};

	constexpr Entity( Manager<Component> & manager, Id id ) noexcept;
	constexpr ~Entity() noexcept;

	constexpr bool operator < ( const Entity & other ) const noexcept;

	constexpr const ComponentList & get_components() const noexcept;
	constexpr void clear_components() noexcept;

	constexpr ComponentPtr add( typename Component::Type type ) noexcept;
	constexpr ComponentPtr add( const cstring & name ) noexcept;
	template< class T > constexpr T & add() noexcept;

	constexpr bool remove( typename Component::Type type ) noexcept;
	constexpr bool remove( const cstring & name ) noexcept;
	template< class T > constexpr bool remove() noexcept;

	constexpr ComponentPtr get( typename Component::Type type ) const noexcept;
	constexpr ComponentPtr get( const cstring & name ) const noexcept;
	template< class T > constexpr T * get() const noexcept;

	constexpr bool has( typename Component::Type type ) const noexcept;
	constexpr bool has( const cstring & name ) const noexcept;
	template< class T > constexpr bool has() const noexcept;

	const Id id;
private:
	ComponentList components;
};

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__entity__hpp
