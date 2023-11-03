/* File: /lib/debug/instances_counter.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__instances_counter__hpp
#define CPPLIB__lib__debug__instances_counter__hpp

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace lib::debug {

// DECLARATION lib::debug::InstancesCounter

template< class T, class CounterType = ::std::size_t >
struct InstancesCounter {
	struct DbgCounter {
		void dump() const noexcept {
			::std::fprintf( stderr,
				"%s"
				"\n\tinstances alive: %lu"
				"\tctor(): %lu"
				"\tctor(&): %lu"
				"\tctor(&&): %lu"
				"\top =(&): %lu"
				"\top =(&&): %lu"
				"\n"
				, __PRETTY_FUNCTION__
				, (::std::uint64_t) instances
				, (::std::uint64_t) max_instances
				, (::std::uint64_t) ctor_copied
				, (::std::uint64_t) ctor_moved
				, (::std::uint64_t) copied
				, (::std::uint64_t) moved
				);
		}
		CounterType instances = 0;
		CounterType max_instances = 0;
		CounterType ctor_copied = 0;
		CounterType ctor_moved = 0;
		CounterType copied = 0;
		CounterType moved = 0;
	private:
		friend struct InstancesCounter<T, CounterType>;
		~DbgCounter() noexcept { dump(); assert( instances == 0 ); }
		static DbgCounter & get() noexcept { static DbgCounter counter; return counter; }
	};
	static const DbgCounter & dbg_counter() noexcept { DbgCounter::get(); }
	InstancesCounter() noexcept { ++DbgCounter::get().instances; ++DbgCounter::get().max_instances; }
	InstancesCounter( const InstancesCounter & ) noexcept : InstancesCounter() { ++DbgCounter::get().ctor_copied; }
	InstancesCounter( InstancesCounter && ) noexcept : InstancesCounter() { ++DbgCounter::get().ctor_moved; }
	~InstancesCounter() noexcept { --DbgCounter::get().instances; }
	InstancesCounter & operator = ( const InstancesCounter & ) noexcept { ++DbgCounter::get().copied; }
	InstancesCounter & operator = ( InstancesCounter && ) noexcept { ++DbgCounter::get().moved; }
};

} // namespace lib::debug

#endif // CPPLIB__lib__debug__instances_counter__hpp
