/* File: /lib/impl_posix/cli/termios_driver.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__cli__termios_driver__hpp
#define CPPLIB__lib__impl_posix__cli__termios_driver__hpp

#include <termios.h>

#include <string>

#include "../../../lib/utils/string.hpp"
#include "../../../lib/utils/updateable.hpp"
#include "../../../lib/impl/stream/stdio.hpp"

#include "../../../lib/cli/driver.hpp"

namespace lib::cli::impl {

// DECLARATION lib::cli::impl::TermiosDriver

class TermiosDriver final
	: public IDriver
	, public tag_utils_updateable<bool>
{
public:
	TermiosDriver();
	virtual ~TermiosDriver();

	/// IDriver
	void out( const cstring & line ) override;

	/// tag_utils_updateable
	bool update() override;
private:
	static constexpr const char * COMMAND_NOT_FOUND = "Command not found or bad syntax: ";
	void beforePromptChanged() override;
	void afterPromptChanged() override;
	void clear_line();
	void redraw_input();
	stream::impl::stdio stream;
	struct ::termios orig_tc_attr;
	::std::string input_string;
};

} // namespace lib::cli::impl

#endif // CPPLIB__lib__impl_posix__cli__termios_driver__hpp
