/* File: /lib/socket/server_client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./server_client.hpp"

// IMPLEMENTATION lib::socket::server::client

namespace lib::socket {

bool server::client::isActive() const noexcept {
	return state == State::CONNECTED
		or state == State::CLOSING;
}

/*virtual */void server::client::setState( State new_state ) {
	if ( state == new_state )
		return;
	state = new_state;
	onStateChanged();
}

/*virtual */void server::client::onStateChanged() {
	on_state_changed( *this );
}

} // namespace lib::socket
