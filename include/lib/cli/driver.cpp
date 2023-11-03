/* File: /lib/cli/driver.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./driver.hpp"

namespace lib::cli {

// IMPLEMENTATION lib::cli::IDriver

void IDriver::prompt( const cstring & prompt ) {
	beforePromptChanged();
	prompt_ = prompt;
	afterPromptChanged();
}

} // namespace lib::cli
