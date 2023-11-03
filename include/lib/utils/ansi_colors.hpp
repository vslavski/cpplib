/* File: /lib/utils/ansi_colors.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__ansi_colors__hpp
#define CPPLIB__lib__utils__ansi_colors__hpp

/// @see https://en.wikipedia.org/wiki/ANSI_escape_code#Colors
/// @see https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

// 3 bit colors

#define LIB_UTILS_ANSI_FG_BLACK						"30"
#define LIB_UTILS_ANSI_BG_BLACK						"40"
#define LIB_UTILS_ANSI_FG_RED						"31"
#define LIB_UTILS_ANSI_BG_RED						"41"
#define LIB_UTILS_ANSI_FG_GREEN						"32"
#define LIB_UTILS_ANSI_BG_GREEN						"42"
#define LIB_UTILS_ANSI_FG_YELLOW					"33"
#define LIB_UTILS_ANSI_BG_YELLOW					"43"
#define LIB_UTILS_ANSI_FG_BLUE						"34"
#define LIB_UTILS_ANSI_BG_BLUE						"44"
#define LIB_UTILS_ANSI_FG_MAGENTA					"35"
#define LIB_UTILS_ANSI_BG_MAGENTA					"45"
#define LIB_UTILS_ANSI_FG_CYAN						"36"
#define LIB_UTILS_ANSI_BG_CYAN						"46"
#define LIB_UTILS_ANSI_FG_WHITE						"37"
#define LIB_UTILS_ANSI_BG_WHITE						"47"

// 4 bit colors (bright)

#define LIB_UTILS_ANSI_FG_BRIGHT_BLACK				"90"
#define LIB_UTILS_ANSI_BG_BRIGHT_BLACK				"100"
#define LIB_UTILS_ANSI_FG_BRIGHT_RED				"91"
#define LIB_UTILS_ANSI_BG_BRIGHT_RED				"101"
#define LIB_UTILS_ANSI_FG_BRIGHT_GREEN				"92"
#define LIB_UTILS_ANSI_BG_BRIGHT_GREEN				"102"
#define LIB_UTILS_ANSI_FG_BRIGHT_YELLOW				"93"
#define LIB_UTILS_ANSI_BG_BRIGHT_YELLOW				"103"
#define LIB_UTILS_ANSI_FG_BRIGHT_BLUE				"94"
#define LIB_UTILS_ANSI_BG_BRIGHT_BLUE				"104"
#define LIB_UTILS_ANSI_FG_BRIGHT_MAGENTA			"95"
#define LIB_UTILS_ANSI_BG_BRIGHT_MAGENTA			"105"
#define LIB_UTILS_ANSI_FG_BRIGHT_CYAN				"96"
#define LIB_UTILS_ANSI_BG_BRIGHT_CYAN				"106"
#define LIB_UTILS_ANSI_FG_BRIGHT_WHITE				"97"
#define LIB_UTILS_ANSI_BG_BRIGHT_WHITE				"107"

// color

#define LIB_UTILS_ANSI_COLOR( Code )				"\033[" Code "m"
#define LIB_UTILS_ANSI_COLOR_FG_BG( Fg, Bg )		"\033[" Fg ";" Bg "m"
#define LIB_UTILS_ANSI_COLOR_DEFAULT				LIB_UTILS_ANSI_COLOR( "" )

// rgb

#define LIB_UTILS_ANSI_RGB_FG( R, G, B )			"\033[38;2;" #R ";" #G ";" #B "m"
#define LIB_UTILS_ANSI_RGB_BG( R, G, B )			"\033[48;2;" #R ";" #G ";" #B "m"
#define LIB_UTILS_ANSI_RGB( FR, FG, FB, BR, BG, BB )								\
	LIB_UTILS_ANSI_RGB_FG( FR, FG, FB ) LIB_UTILS_ANSI_RGB_BG( BR, BG, BB )

#endif // CPPLIB__lib__utils__ansi_colors__hpp
