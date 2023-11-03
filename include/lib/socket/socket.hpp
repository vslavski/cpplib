/* File: /lib/socket/socket.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__socket__socket__hpp
#define CPPLIB__lib__socket__socket__hpp

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/data/stream.hpp"
#include "../../lib/utils/updateable.hpp"

namespace lib::socket {

// DECLARATION lib::socket::socket

class socket
	: tl::noncopymovable<socket>
	, public data::rwstream_t
	, public tag_utils_updateable<bool>
{
public:
	socket() = default;
	virtual ~socket() { /*close();*/ }

	using tag_utils_updateable<bool>::update;

	virtual bool open() = 0;
	virtual bool shutdown() { return close(); }
	virtual bool close() = 0;

	virtual bool is_open() const = 0;

	virtual cstring getName() const { return "<not implemented>"; }
};

} // namespace lib::socket

#endif // CPPLIB__lib__socket__socket__hpp
