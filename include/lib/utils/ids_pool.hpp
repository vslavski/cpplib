/* File: /lib/utils/ids_pool.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__ids_pool__hpp
#define CPPLIB__lib__utils__ids_pool__hpp

#include <algorithm>
#include <iterator>
#include <vector>

#include <cpp/lib_concepts>
#include <cpp/lib_debug>

#include "../../lib/types.hpp"
#include "../../lib/literals.hpp"

namespace lib::utils {

// DECLARATION lib::utils::ids_pool

template< ::cpp::IntegralUnsigned T >
class ids_pool final {
public:
	using value_type = T;

	constexpr ids_pool() : count{} {}

	/*constexpr */value_type retain() {
		if ( count == ids.size() ) {
			ids.emplace_back( true );
			return (value_type)++count;
		}
		CPP_ASSERT( count < ids.size() );
		auto it = ::std::find( ids.begin(), ids.end(), false );
		*it = true;
		++count;
		return (value_type)( ::std::distance( ids.begin(), it ) + 1 );
	}

	constexpr bool release( value_type id ) {
		if ( not contains( id ) )
			return false;
		--count;
		ids[id - 1_sz] = false;
		return true;
	}

	constexpr bool contains( value_type id ) const {
		CPP_ASSERT( id > 0 );
		return ids.size() >= id and ids[id - 1_sz];
	}

	constexpr usize size() const {
		return count;
	}

	constexpr void clear() {
		ids.clear();
		count = 0;
	}
private:
	::std::vector<bool> ids;
	usize count;
};

} // namespace lib::utils

#endif // CPPLIB__lib__utils__ids_pool__hpp
