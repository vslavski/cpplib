/* File: /lib/utils/updateable.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__updateable__hpp
#define CPPLIB__lib__utils__updateable__hpp

// DECLARATION lib::tag_utils_updateable

namespace lib {

template< class R = void, class...Args >
class tag_utils_updateable {
public:
	virtual ~tag_utils_updateable() = default;
	virtual R update( Args...args ) = 0;
};

} // namespace lib

#endif // CPPLIB__lib__utils__updateable__hpp
