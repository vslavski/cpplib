/* File: /lib/impl_posix/application/termios_keyboard.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__application__termios_keyboard__hpp
#define CPPLIB__lib__impl_posix__application__termios_keyboard__hpp

#include <termios.h>

#include <vector>

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

#include "../../../lib/impl/application/default_keyboard.hpp"

namespace lib::application::impl {

// DEFINITION: lib::application::impl::TermiosKeyboard

class TermiosKeyboard
	: public IDefaultKeyboard
{
public:
	TermiosKeyboard( data::rstream_t & stream );
	virtual ~TermiosKeyboard();

	bool update() override;
private:
	bool updateBuffer();
	bool parseBuffer();
	bool parseEscape( auto & it );

	struct ::termios orig_tc_attr;
	/// @todo Add ..::stream::termios to share it with ..::TermiosDisplay.
	data::rstream_t & stream;
	::std::vector<u8> buffer;
};

} // namespace lib::application::impl

#endif // CPPLIB__lib__impl_posix__application__termios_keyboard__hpp
