/* File: /lib/socket/server.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__socket__server__hpp
#define CPPLIB__lib__socket__server__hpp

#include "../../lib/tl/listener.hpp"
#include "../../lib/ptr.hpp"
#include "../../lib/utils/enum.hpp"
#include "../../lib/utils/string.hpp"

#include "./socket.hpp"

namespace lib::socket {

// DECLARATION lib::socket::server

/// @todo Fix callback recursion, if any. e.g. if close() failed it fires state change,
/// which could possibly lead to callback loop. Need to ensure.

class server
	: public virtual socket
{
public:
	class client;

	LIB_UTILS_ENUM( State
		, INITIAL
		, BINDING
		, BOUND
		, LISTENING
		, CLOSING
		, CLOSED
		, FAILED
	)

	enum class NewClientAction { NONE, ACCEPT, REJECT, AUTO };

	server() = default;
	virtual ~server() = default;

	State getState() const noexcept { return state; }
	bool isFailed() const noexcept { return state == State::FAILED; }
	bool isActive() const noexcept;

	tl::listener< void, const server& > on_state_changed;
	tl::listener< void, const server&, NewClientAction& > on_new_client;
	tl::listener< void, const server&, const SPtr<client>& > on_client_state_changed;
protected:
	virtual void setState( State new_state );
	virtual void onStateChanged();
	virtual void onNewClient( NewClientAction & action );
	virtual void onClientStateChanged( const SPtr<client> & client_ );
private:
	State state = State::INITIAL;
};

} // namespace lib::socket

LIB_UTILS_ENUM_NAMES( lib::socket::server::State
	, "initial"
	, "binding"
	, "bound"
	, "listening"
	, "closing"
	, "closed"
	, "failed"
	)

// SPECIALIZATION lib::str()

#include <string>
namespace lib {
inline ::std::string str( const lib::socket::server & server )
	{ return str( server.getName() ); }
} // namespace lib

#endif // CPPLIB__lib__socket__server__hpp
