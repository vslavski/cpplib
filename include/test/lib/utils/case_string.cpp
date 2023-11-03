/* File: /test/lib/utils/case_string.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/utils/case_string.hpp>

#include "./case_string.hpp"

namespace test::lib::utils {

void CaseString::test_execute() noexcept/* override*/ {
	using ::lib::utils::case_string;

	/// @todo Check all possible conditions: "", "_", "__", "_a_"...
	case_string case_str;

	CPPLIB__TEST__TRUE( case_str.lower().empty() );
	CPPLIB__TEST__TRUE( case_str.upper().empty() );
	CPPLIB__TEST__TRUE( case_str.pascal().empty() );
	CPPLIB__TEST__TRUE( case_str.camel().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_lower().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_upper().empty() );

	case_str = "_";
	CPPLIB__TEST__TRUE( case_str.lower().empty() );
	CPPLIB__TEST__TRUE( case_str.upper().empty() );
	CPPLIB__TEST__TRUE( case_str.pascal().empty() );
	CPPLIB__TEST__TRUE( case_str.camel().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_lower().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_upper().empty() );

	case_str = "__";
	CPPLIB__TEST__TRUE( case_str.lower().empty() );
	CPPLIB__TEST__TRUE( case_str.upper().empty() );
	CPPLIB__TEST__TRUE( case_str.pascal().empty() );
	CPPLIB__TEST__TRUE( case_str.camel().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_lower().empty() );
	CPPLIB__TEST__TRUE( case_str.snake_upper().empty() );

	case_str = "PasCal";
	CPPLIB__TEST__EQ( case_str.lower()			, "pascal" );
	CPPLIB__TEST__EQ( case_str.upper()			, "PASCAL" );
	CPPLIB__TEST__EQ( case_str.pascal()			, "PasCal" );
	CPPLIB__TEST__EQ( case_str.camel()			, "pasCal" );
	CPPLIB__TEST__EQ( case_str.snake_lower()	, "pas_cal" );
	CPPLIB__TEST__EQ( case_str.snake_upper()	, "PAS_CAL" );

	case_str = "caMel";
	CPPLIB__TEST__EQ( case_str.lower()			, "camel" );
	CPPLIB__TEST__EQ( case_str.upper()			, "CAMEL" );
	CPPLIB__TEST__EQ( case_str.pascal()			, "CaMel" );
	CPPLIB__TEST__EQ( case_str.camel()			, "caMel" );
	CPPLIB__TEST__EQ( case_str.snake_lower()	, "ca_mel" );
	CPPLIB__TEST__EQ( case_str.snake_upper()	, "CA_MEL" );

	case_str = "snake_lower";
	CPPLIB__TEST__EQ( case_str.lower()			, "snakelower" );
	CPPLIB__TEST__EQ( case_str.upper()			, "SNAKELOWER" );
	CPPLIB__TEST__EQ( case_str.pascal()			, "SnakeLower" );
	CPPLIB__TEST__EQ( case_str.camel()			, "snakeLower" );
	CPPLIB__TEST__EQ( case_str.snake_lower()	, "snake_lower" );
	CPPLIB__TEST__EQ( case_str.snake_upper()	, "SNAKE_LOWER" );

	case_str = "SNAKE_UPPER";
	CPPLIB__TEST__EQ( case_str.lower()			, "snakeupper" );
	CPPLIB__TEST__EQ( case_str.upper()			, "SNAKEUPPER" );
	CPPLIB__TEST__EQ( case_str.pascal()			, "SnakeUpper" );
	CPPLIB__TEST__EQ( case_str.camel()			, "snakeUpper" );
	CPPLIB__TEST__EQ( case_str.snake_lower()	, "snake_upper" );
	CPPLIB__TEST__EQ( case_str.snake_upper()	, "SNAKE_UPPER" );

	case_str = "snake_Mixed";
	CPPLIB__TEST__EQ( case_str.lower()			, "snakemixed" );
	CPPLIB__TEST__EQ( case_str.upper()			, "SNAKEMIXED" );
	CPPLIB__TEST__EQ( case_str.pascal()			, "SnakeMixed" );
	CPPLIB__TEST__EQ( case_str.camel()			, "snakeMixed" );
	CPPLIB__TEST__EQ( case_str.snake_lower()	, "snake_mixed" );
	CPPLIB__TEST__EQ( case_str.snake_upper()	, "SNAKE_MIXED" );
}

} // namespace test::lib::utils
