/* File: /lib/impl/cli/cstdio_driver.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__cli__cstdio_driver__hpp
#define CPPLIB__lib__impl__cli__cstdio_driver__hpp

#include <string>

#include "../../../lib/cstring.hpp"
#include "../../../lib/utils/updateable.hpp"

#include "../../../lib/cli/driver.hpp"

namespace lib::cli::impl {

// DECLARATION lib::cli::impl::CstdioDriver

class CstdioDriver final
	: public IDriver
	, public tag_utils_updateable<bool>
{
public:
	CstdioDriver() = default;
	virtual ~CstdioDriver();

	/// IDriver
	void out( const cstring & line ) override;

	/// tag_utils_updateable
	bool update() override;
private:
	static constexpr const char * COMMAND_NOT_FOUND = "Command not found or bad syntax: ";
	void afterPromptChanged() override;
	void redraw_input();

	bool input_clear = true;
	::std::string input_string;
};

} // namespace lib::cli::impl

#endif // CPPLIB__lib__impl__cli__cstdio_driver__hpp
