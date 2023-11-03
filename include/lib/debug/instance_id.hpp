/* File: /lib/debug/instance_id.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__debug__instance_id__hpp
#define CPPLIB__lib__debug__instance_id__hpp

#include <cstddef>

namespace lib::debug {

// DECLARATION lib::debug::InstanceId

template< class T, class IdType = ::std::size_t >
struct InstanceId {
	InstanceId() noexcept : dbg_id{ next_id() } {}
	InstanceId( const InstanceId & ) noexcept : InstanceId{} {}
	InstanceId( InstanceId && ) noexcept : InstanceId{} {}
	InstanceId & operator = ( const InstanceId & ) noexcept { return *this; }
	InstanceId & operator = ( InstanceId && ) noexcept { return *this; }

	const IdType dbg_id;
private:
	static IdType next_id() noexcept { static IdType id = 0; return ++id; }
};

} // namespace lib::debug

#endif // CPPLIB__lib__debug__instance_id__hpp
