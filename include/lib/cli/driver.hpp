/* File: /lib/cli/driver.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__cli__driver__hpp
#define CPPLIB__lib__cli__driver__hpp

#include <string>

#include <cpp/lib_debug>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/linkable.hpp"
#include "../../lib/cstring.hpp"

#include "./interface.hpp"

namespace lib::cli {

// PREDECLARATIONS

class Interface;

// DECLARATION lib::cli::IDriver

class IDriver
	: tl::noncopymovable<IDriver>
	, public tl::linkable<IDriver, Interface>
{
public:
	IDriver() = default;
	virtual ~IDriver() = default;

	virtual void out( const cstring & line ) { CPP_UNUSED( line ); }

	void prompt( const cstring & prompt );
	const ::std::string & prompt() const { return prompt_; }
protected:
	virtual void beforePromptChanged() {}
	virtual void afterPromptChanged() {}
	using linkable< IDriver, Interface >::get_link;
private:
	::std::string prompt_;
};

} // namespace lib::cli

#endif // CPPLIB__lib__cli__driver__hpp
