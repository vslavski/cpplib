/* File: /lib/cli/param.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__cli__param__hpp
#define CPPLIB__lib__cli__param__hpp

#include <string>

#include "../../lib/types.hpp"
#include "../../lib/utils/value.hpp"

namespace lib::cli {

// DECLARATION lib::cli::Param

using Param = utils::Value<i32, f32, ::std::string>;

// DECLARATION lib::cli::param, lib::cli::params_list

static constexpr struct {} param;
static constexpr struct {} params_list;

} // namespace lib::cli

#endif // CPPLIB__lib__cli__param__hpp
