/* File: /lib/bus/tag.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__bus__tag__hpp
#define CPPLIB__lib__bus__tag__hpp

#include <cpp/lib_concepts>

namespace lib {

// PREDECLARATION lib::bus::Manager<>

namespace bus { template<::cpp::IntOrEnum MessageIdType> class Manager; }

// DECLARATION lib::tag_bus<>

template< class Bus >
requires( ::cpp::SameAs< Bus, bus::Manager<typename Bus::message_id_type> > )
class tag_bus {
	public:		tag_bus( Bus & bus ) : bus{ bus } {};
	protected:	Bus & bus;
};

} // namespace lib

#endif // CPPLIB__lib__bus__tag__hpp
