/* File: /lib/packets/packet.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__packets__packet__hpp
#define CPPLIB__lib__packets__packet__hpp

#include "../../lib/types.hpp"
//#include "../../lib/debug/features.hpp"

namespace lib::packets {

// DECLARATION lib::packets::Header

struct /*alignas(1)*/ Header {
	using id_type = u32;
	using size_type = u32;
	id_type id;
	size_type size;
};

// DECLARATION lib::packets::Packet

//struct /*alignas(1)*/ Packet
//	: Header
//{
//CPPLIB_GCC_CLANG_DIAGNOSTIC(push)
//CPPLIB_GCC_CLANG_DIAGNOSTIC(ignored "-Wc99-extensions")
//CPPLIB_GCC_CLANG_DIAGNOSTIC(ignored "-Wgnu-empty-struct")
//	u8 data[];
//CPPLIB_GCC_CLANG_DIAGNOSTIC(pop)
//};

} // namespace lib::packets

#endif // CPPLIB__lib__packets__packet__hpp
