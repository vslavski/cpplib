/* File: /lib/test/unit.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__test__unit__hpp
#define CPPLIB__lib__test__unit__hpp

#include <cstddef>

#include <system_error>

#include <cpp/lib_math>
#include <cpp/lib_system_error>

namespace lib::test {

struct Result {
	constexpr Result( const char * name ) noexcept : name {name} {}

	constexpr operator bool () const noexcept { return line == 0; }

	const char * name;
	const char * subname = nullptr;
	::std::size_t iteration = 0;
	int line = 0;

	::cpp::error_condition error;
};

class IUnit {
public:
	IUnit( const char * name ) noexcept : result_ {name} {}
	virtual ~IUnit() noexcept {}

	const Result & test_run() noexcept;

protected:

	virtual void test_execute() noexcept = 0;
	virtual void test_reset() noexcept {}

	void test_set_failed( int line ) noexcept;
	bool test_failed() const noexcept { return not result_; }

	void test_subname( const char * name ) noexcept { result_.subname = name; }

	void test_reset_iteration() noexcept { result_.iteration = 0; }
	void test_next_iteration() noexcept { ++result_.iteration; }

	void test_error( const ::std::error_condition & error ) noexcept
		{ if ( not result_.error ) result_.error = error; }
private:
	Result result_;
};

} // namespace lib::test

/// HELPERS

#define CPPLIB__TEST__FAILED()						test_set_failed( __LINE__ )
#define CPPLIB__TEST__IS_FAILED()					test_failed()
#define CPPLIB__TEST__STOP_FAILED()					if ( test_failed() ) return

#define CPPLIB__TEST__LOOP_RESET()					test_reset_iteration()
#define CPPLIB__TEST__LOOP_NEXT()					test_next_iteration()

#define CPPLIB__TEST__TRUE( ... )					if ( not (__VA_ARGS__) )		{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__FALSE( ... )					if ( (__VA_ARGS__) )			{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__LT( left, right )				if ( not ((left) <  (right)) )	{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__LE( left, right )				if ( not ((left) <= (right)) )	{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__EQ( left, right )				if ( not ((left) == (right)) )	{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__NE( left, right )				if ( not ((left) != (right)) )	{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__GE( left, right )				if ( not ((left) >= (right)) )	{ CPPLIB__TEST__FAILED(); return; }
#define CPPLIB__TEST__GT( left, right )				if ( not ((left) >  (right)) )	{ CPPLIB__TEST__FAILED(); return; }

#define CPPLIB__TEST__EQf( left, right )			CPPLIB__TEST__TRUE( ::cpp::epsilon_eq<float>( left, right ) )

#define CPPLIB__TEST__TRUEv( ... )					if ( not (__VA_ARGS__) )		CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__FALSEv( ... )					if ( (__VA_ARGS__) )			CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__LTv( left, right )			if ( not ((left) <  (right)) )	CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__LEv( left, right )			if ( not ((left) <= (right)) )	CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__EQv( left, right )			if ( not ((left) == (right)) )	CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__NEv( left, right )			if ( not ((left) != (right)) )	CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__GEv( left, right )			if ( not ((left) >= (right)) )	CPPLIB__TEST__FAILED()
#define CPPLIB__TEST__GTv( left, right )			if ( not ((left) >  (right)) )	CPPLIB__TEST__FAILED()

#define CPPLIB__TEST__EQfv( left, right )			CPPLIB__TEST__TRUEv( ::cpp::epsilon_eq<float>( left, right ) )

#define CPPLIB__TEST__SUBTEST( name, ... )			\
	test_subname( #name );							\
	name( __VA_ARGS__ );							\
	CPPLIB__TEST__STOP_FAILED()

#endif // CPPLIB__lib__test__unit__hpp
