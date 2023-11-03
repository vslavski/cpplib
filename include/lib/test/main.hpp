/* File: /lib/test/main.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__test__main__hpp
#define CPPLIB__lib__test__main__hpp

#include <cstddef>
#include <cstdio>

#include "../../lib/utils/ansi_colors.hpp"

#include "../../lib/debug/compiler.hpp"
#include "../../lib/debug/os.hpp"
#include "../../lib/debug/platform.hpp"

/// COLORS

#define CPPLIB__TEST_ColDef							LIB_UTILS_ANSI_COLOR_DEFAULT
#define CPPLIB__TEST_ColWht							LIB_UTILS_ANSI_COLOR( LIB_UTILS_ANSI_FG_WHITE )
#define CPPLIB__TEST_ColYlw							LIB_UTILS_ANSI_COLOR( LIB_UTILS_ANSI_FG_YELLOW )
#define CPPLIB__TEST_ColPrp							LIB_UTILS_ANSI_COLOR( LIB_UTILS_ANSI_FG_MAGENTA )
#define CPPLIB__TEST_ColRed							LIB_UTILS_ANSI_COLOR( LIB_UTILS_ANSI_FG_RED )
#define CPPLIB__TEST_ColGrn							LIB_UTILS_ANSI_COLOR( LIB_UTILS_ANSI_FG_GREEN )

/// MAIN

#define CPPLIB__TEST_SYSTEM_INFO						\
	::std::fprintf( stdout,								\
		CPPLIB__TEST_ColWht "Compiler: "				\
		CPPLIB__TEST_ColPrp "%s"						\
		CPPLIB__TEST_ColWht " OS: "						\
		CPPLIB__TEST_ColPrp "%s"						\
		CPPLIB__TEST_ColWht " Platform: "				\
		CPPLIB__TEST_ColPrp "%s"						\
		CPPLIB__TEST_ColDef "\n"						\
		, ::lib::str( ::lib::debug::compiler ).data()	\
		, ::lib::str( ::lib::debug::os ).data()			\
		, ::lib::str( ::lib::debug::platform ).data()	\
		)


#define CPPLIB__TEST_MAIN_BEGIN							\
	::std::size_t succeed_count = 0;					\
	::std::size_t failed_count = 0

#define CPPLIB__TEST_MAIN_END							\
	::std::fprintf( stdout,								\
		CPPLIB__TEST_ColGrn "Succeed"					\
		CPPLIB__TEST_ColWht ":  %zu\n"					\
		CPPLIB__TEST_ColRed "Failed"					\
		CPPLIB__TEST_ColWht ":   %zu\n"					\
		CPPLIB__TEST_ColDef								\
		, succeed_count, failed_count );				\
	::std::fflush( stdout );							\
	return failed_count > 0 ? 1 : 0


#define CPPLIB__TEST_RUN( Unit ) {						\
		::std::fprintf( stdout,							\
			CPPLIB__TEST_ColWht "Testing unit <"		\
			CPPLIB__TEST_ColYlw #Unit					\
			CPPLIB__TEST_ColWht ">: "					\
			);											\
		Unit unit;										\
		const auto & result = unit.test_run();			\
		if ( result ) {									\
			::std::fprintf( stdout,						\
				CPPLIB__TEST_ColGrn "success\n"			\
				CPPLIB__TEST_ColDef						\
				);										\
			++succeed_count;							\
		} else {										\
			::std::fprintf( stdout,						\
				CPPLIB__TEST_ColRed "failed"			\
				CPPLIB__TEST_ColDef						\
				);										\
			if ( result.subname != nullptr ) {			\
				::std::fprintf( stdout,					\
					CPPLIB__TEST_ColPrp " \"%s\""		\
					, result.subname					\
					);									\
			}											\
			::std::fprintf( stdout,						\
				CPPLIB__TEST_ColWht " on line "			\
				CPPLIB__TEST_ColPrp "%i"				\
				CPPLIB__TEST_ColDef						\
				, result.line							\
				);										\
			if ( result.iteration > 0 ) {				\
				::std::fprintf( stdout,					\
					CPPLIB__TEST_ColWht ", iteration "	\
					CPPLIB__TEST_ColPrp "%zu"			\
					CPPLIB__TEST_ColDef					\
					, result.iteration					\
					);									\
			}											\
			if ( result.error ) {						\
				::std::fprintf( stdout,					\
					CPPLIB__TEST_ColWht " error "		\
					CPPLIB__TEST_ColPrp "%i"			\
					CPPLIB__TEST_ColWht ": "			\
					CPPLIB__TEST_ColPrp "%s"			\
					CPPLIB__TEST_ColDef					\
					, result.error.value()				\
					, result.error.message().c_str()	\
					);									\
			}											\
			::std::fputc( '\n', stdout );				\
			++failed_count;								\
		}												\
		::std::fflush( stdout );						\
	}

#endif // CPPLIB__lib__test__main__hpp
