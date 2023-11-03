/* File: /lib/socket/client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__socket__client__hpp
#define CPPLIB__lib__socket__client__hpp

#include "../../lib/tl/listener.hpp"
#include "../../lib/utils/enum.hpp"
#include "../../lib/utils/string.hpp"

#include "./socket.hpp"

namespace lib::socket {

// DECLARATION lib::socket::client

/// @todo Fix callback recursion, if any. e.g. if close() failed it fires state change,
/// which could possibly lead to callback loop. Need to ensure.

class client
	: public virtual socket
{
public:
	LIB_UTILS_ENUM( State
		, INITIAL
		, CONNECTING
		, CONNECTED
		, DISCONNECTING
		, DISCONNECTED
		, FAILED
	)

	client() = default;
	virtual ~client() = default;

	State getState() const noexcept { return state; }
	bool isFailed() const noexcept { return state == State::FAILED; }
	bool isActive() const noexcept;

	tl::listener< void, const client& > on_state_changed;
protected:
	virtual void setState( State new_state );
	virtual void onStateChanged();
private:
	State state = State::INITIAL;
};

} // namespace lib::socket

LIB_UTILS_ENUM_NAMES( lib::socket::client::State
	, "initial"
	, "connecting"
	, "connected"
	, "disconnecting"
	, "disconnected"
	, "failed"
	)

// SPECIALIZATION lib::str()

#include <string>
namespace lib {
inline ::std::string str( const lib::socket::client & client )
	{ return str( client.getName() ); }
} // namespace lib

#endif // CPPLIB__lib__socket__client__hpp
