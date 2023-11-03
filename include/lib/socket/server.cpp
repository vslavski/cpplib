/* File: /lib/socket/server.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./server.hpp"

// IMPLEMENTATION lib::socket::server

namespace lib::socket {

bool server::isActive() const noexcept {
	return state == State::BINDING
		or state == State::BOUND
		or state == State::LISTENING;
}

/*virtual */void server::setState( State new_state ) {
	if ( state == new_state )
		return;
	state = new_state;
	onStateChanged();
}

/*virtual */void server::onStateChanged() {
	on_state_changed( *this );
}

/*virtual */void server::onNewClient( NewClientAction & action ) {
	on_new_client( *this, action );
}

/*virtual */void server::onClientStateChanged( const SPtr<client> & client_ ) {
	on_client_state_changed( *this, client_ );
}

} // namespace lib::socket
