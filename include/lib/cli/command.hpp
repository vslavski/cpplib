/* File: /lib/cli/command.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__cli__command__hpp
#define CPPLIB__lib__cli__command__hpp

#include <vector>

#include "../../lib/tl/noncopyable.hpp"

#include "./param.hpp"

namespace lib::cli {

// PREDECLARATIONS

class Interface;

// DECLARATION lib::cli::Command

struct Command final
	: tl::noncopymovable<Command>
{
	inline Command( Interface & interface ) : interface( interface ) {}
	Interface & interface;
	::std::vector<Param> params;
};

} // namespace lib::cli

#endif // CPPLIB__lib__cli__command__hpp
