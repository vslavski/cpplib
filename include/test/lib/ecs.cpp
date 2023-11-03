/* File: /test/lib/ecs.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

//#define ENABLE_TEST_LIB_ECS_LOG

#ifdef ENABLE_TEST_LIB_ECS_LOG
#include <cstdio>
#endif // ENABLE_TEST_LIB_ECS_LOG

#include <string>

#include <lib/types.hpp>
#include <lib/cstring.hpp>
#include <lib/utils/enum.hpp>

#include <lib/ecs.hpp>

#include "./ecs.hpp"

namespace test::lib {
namespace {

using namespace ::lib::ecs;

// DECLARATION test::lib::{}::ComponentType

LIB_UTILS_ENUM( ComponentType : ::lib::u16
	, POSITION
	, HEALTH
	, NAME
	)

// DECLARATION test::lib::{}::Component

class Component
	: public ComponentBase< ::lib::u32/*EntityId*/, ComponentType, Component >
{
	using Super = ComponentBase< ::lib::u32, ComponentType, Component >;
public:
	static inline ::lib::Ptr<Component> create( Type type );

	inline Component( Type type ) : Super( type ) {}

	::lib::u32 own_property = 123;
};

// DECLARATION test::lib::{}: Position, Health, Name

struct Position final {
	static constexpr auto TYPE = Component::Type::POSITION;
	int x, y;
};
struct Health final {
	static constexpr auto TYPE = Component::Type::HEALTH;
	int current, maximum;
};
struct Name final {
	static constexpr auto TYPE = Component::Type::NAME;
	::std::string name;
};

// INLINE test::lib::{}::Component::create()

inline ::lib::Ptr<Component> Component::create( Type type ) {
	switch ( type ) {
	case Position::TYPE:	return Super::create<Position>();
	case Health::TYPE:		return Super::create<Health>();
	case Name::TYPE:		return Super::create<Name>();
	default:				return {};
	};
}

} // namespace
} // namespace test::lib

LIB_UTILS_ENUM_NAMES( ::test::lib::ComponentType
	, "Position"
	, "Health"
	, "Name"
	)

// TEST test::lib::Ecs()

namespace test::lib {

void Ecs::test_execute() noexcept/* override*/ {
	using Manager = ::lib::ecs::Manager<Component>;

#ifdef ENABLE_TEST_LIB_ECS_LOG
	struct DbgLog : tag_tl_listener<DbgLog> {
		void onEntityAdded( const Manager::Entity & entity )
			{ ::std::printf( "%s( %d )\n", __FUNCTION__, entity.id ); }
		void onEntityRemoved( const Manager::Entity & entity )
			{ ::std::printf( "%s( %d )\n", __FUNCTION__, entity.id ); }
		void onComponentAdded( const Manager::Entity & entity, const Component & component )
			{ ::std::printf( "%s( %d, %s )\n", __FUNCTION__, entity.id, str( component.type ).data() ); }
		void onComponentRemoved( const Manager::Entity & entity, const Component & component )
			{ ::std::printf( "%s( %d, %s )\n", __FUNCTION__, entity.id, str( component.type ).data() ); }
	} log;
#endif // ENABLE_TEST_LIB_ECS_LOG
	Manager manager;
#ifdef ENABLE_TEST_LIB_ECS_LOG
	manager.on_entity_added = { log, &DbgLog::onEntityAdded };
	manager.on_entity_removed = { log, &DbgLog::onEntityRemoved };
	manager.on_component_added = { log, &DbgLog::onComponentAdded };
	manager.on_component_removed = { log, &DbgLog::onComponentRemoved };
#endif // ENABLE_TEST_LIB_ECS_LOG

	auto entity_1 = manager.add();
	CPPLIB__TEST__TRUE( entity_1 );
	CPPLIB__TEST__EQ( entity_1->id, 1 );

	CPPLIB__TEST__TRUE( manager.add( 100 ) );

	auto entity_101 = manager.add();
	CPPLIB__TEST__EQ( entity_101->id, 101 );

	CPPLIB__TEST__TRUE( manager.get( 100 ) );
	CPPLIB__TEST__TRUE( manager.remove( 100 ) );

	CPPLIB__TEST__TRUE( manager.add( *entity_1, Position::TYPE ) );
	//ASSERT: CPPLIB__TEST__FALSE( manager.add( *entity_1, "UndefinedComponent" ) );

	CPPLIB__TEST__TRUE( manager.add( *entity_101, Health::TYPE ) );
	CPPLIB__TEST__TRUE( manager.add( *entity_101, "Name" ) );
	entity_1->add< Name >().name = "entity_1";

	auto entity_1_name = entity_1->get( Name::TYPE );
	CPPLIB__TEST__EQ( entity_1_name->object< Position >(), nullptr );
	CPPLIB__TEST__EQ( entity_1_name->as< Name >().name, "entity_1" );

	CPPLIB__TEST__EQ( entity_1_name->own_property, 123 );

	CPPLIB__TEST__TRUE( manager.remove( *entity_1, Name::TYPE ) );
	CPPLIB__TEST__TRUE( manager.remove( *entity_101, "Health" ) );

	CPPLIB__TEST__NE( entity_1->add( Health::TYPE ), nullptr );
	CPPLIB__TEST__TRUE( entity_101->add( "Position" ) );

	CPPLIB__TEST__FALSE( entity_101->remove( Health::TYPE ) );
	CPPLIB__TEST__FALSE( entity_1->remove( "Name" ) );
	CPPLIB__TEST__TRUE( entity_101->remove< Position >() );

	CPPLIB__TEST__EQ( entity_1->get( "Position" )->object<Health>(), nullptr );
	entity_1->get< Health >()->current = 123;

	CPPLIB__TEST__FALSE( entity_1->has( Name::TYPE ) );
	CPPLIB__TEST__TRUE( entity_1->has( "Position" ) );
	CPPLIB__TEST__TRUE( entity_1->has< Health >() );

#ifdef ENABLE_TEST_LIB_ECS_LOG
	for ( const auto & it : manager.get_entities() )
		for ( const auto & cit : it.second->get_components() )
			::std::printf( "E%03d\t%s\n", it.first, str( cit.first ).data() );
#endif // ENABLE_TEST_LIB_ECS_LOG

	entity_1->clear_components();
	entity_1.reset();

	entity_101->clear_components();
	entity_101.reset();

	manager.remove( 1 );
	manager.remove( 101 );
}

} // namespace test::lib
