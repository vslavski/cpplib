/* File: /lib/ecs/component.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__ecs__component__hpp
#define CPPLIB__lib__ecs__component__hpp

#include <cpp/lib_concepts>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/ptr.hpp"

namespace lib::ecs {

// DECLARATION lib::ecs::ComponentBase<>

template< ::cpp::Integral EntityId_
		, ::cpp::Enumeration ComponentType
		, class Component >
class ComponentBase
	: tl::noncopymovable< ComponentBase<EntityId_, ComponentType, Component> >
{
protected:
	template< class T >
	static constexpr Ptr<Component> create() noexcept;
public:
	using EntityId = EntityId_;
	using Type = ComponentType;

	constexpr ComponentBase( Type type ) noexcept;
	virtual ~ComponentBase() noexcept = default;

	template< class T > constexpr T & as() noexcept;
	template< class T > constexpr const T & as() const noexcept;
	template< class T > constexpr T * object() noexcept;
	template< class T > constexpr const T * object() const noexcept;

	const Type type;
};

} // namespace lib::ecs

#endif // CPPLIB__lib__ecs__component__hpp
