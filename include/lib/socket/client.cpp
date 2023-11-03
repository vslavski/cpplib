/* File: /lib/socket/client.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./client.hpp"

// IMPLEMENTATION lib::socket::client

namespace lib::socket {

bool client::isActive() const noexcept {
	return state == State::CONNECTING
		or state == State::CONNECTED
		or state == State::DISCONNECTING;
}

/*virtual */void client::setState( State new_state ) {
	if ( state == new_state )
		return;
	state = new_state;
	onStateChanged();
}

/*virtual */void client::onStateChanged() {
	on_state_changed( *this );
}

} // namespace lib::socket
