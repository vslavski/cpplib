/* File: /lib/bus/message.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__bus__message__hpp
#define CPPLIB__lib__bus__message__hpp

#include <cpp/lib_concepts>

#include "../../lib/tl/noncopyable.hpp"

namespace lib::bus {

/// @todo MessageSync?
// DECLARATION lib::bus::Message<>

template< ::cpp::IntOrEnum IdType >
class Message
	: tl::noncopymovable< Message<IdType> >
{
public:
	using id_type = IdType;

	const id_type id;

	constexpr Message( id_type id ) noexcept : id{ id } {}
	virtual ~Message() noexcept = default;
};

} // namespace lib::bus

#endif // CPPLIB__lib__bus__message__hpp
