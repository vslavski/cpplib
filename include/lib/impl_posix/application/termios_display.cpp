/* File: /lib/impl_posix/application/termios_display.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <unistd.h>
#include <sys/ioctl.h>

#include <tuple>

/// @todo Add runtime functionality for "lib/utils/ansi_colors.hpp" and use.

#include "./termios_display.hpp"

namespace lib::application::impl {

// IMPLEMENTATION lib::application::impl::TermiosDisplay

/*constexpr */TermiosDisplay::TermiosDisplay() noexcept
	: buffer{ MkPtr<Buffer>( *this ) }
{
	if ( 0 != ::tcgetattr( STDOUT_FILENO, &termios_settings ) )
		return;

	update_size();
	hide_cursor();

	/// @todo Get initial value from "\033[6n" command.
	set_cursor( 0, 0 );
}

/*constexpr */TermiosDisplay::~TermiosDisplay() noexcept {
	show_cursor();

	if ( 0 != ::tcsetattr( STDOUT_FILENO, 0, &termios_settings ) )
		return;
}

/*constexpr */void TermiosDisplay::show_cursor( bool show/* = true*/ ) noexcept {
	stream.printf( "\033[?25%c", show ? 'h' : 'l' );
}

/*constexpr */void TermiosDisplay::hide_cursor() noexcept {
	show_cursor( false );
}

/*constexpr */void TermiosDisplay::clear_bg( const TermiosDisplay::Color & color ) noexcept {
	/// @todo Suspend draw optimization instead of it.
	buffer->clear_bg( { (u8) ~color.r, (u8) ~color.g, (u8) ~color.b } );

	/// @todo Restore cursor?
	set_cursor( 0, 0 );
	for ( u32 i = 0; i < width_ * height_ ; ++i ) {
		draw_bg( color );
		shift_cursor();
	}
}

/*constexpr */void TermiosDisplay::set_cursor( u32 x, u32 y ) noexcept {
	cursor_x = x;
	cursor_y = y;
	stream.printf( "\033[%d;%dH", y, x );
}

/*constexpr */void TermiosDisplay::draw_bg( const TermiosDisplay::Color & color_ ) noexcept {
	auto & color = buffer->color( cursor_x, cursor_y ).bg;
	if ( color == color_ )
		return;
	color = color_;
	char symbol = ' ';
	if ( const char symbol_ = buffer->symbol( cursor_x, cursor_y ) )
		symbol = symbol_;
	stream.printf( "\033[48;2;%d;%d;%dm%c", color.r, color.g, color.b, symbol );
}

/*constexpr */void TermiosDisplay::set_bg( u32 x, u32 y, const TermiosDisplay::Color & color ) noexcept {
	set_cursor( x, y );
	draw_bg( color );
}

/*constexpr */void TermiosDisplay::update_size() noexcept {
	struct ::winsize window_size;
	if ( 0 != ::ioctl( STDOUT_FILENO, TIOCGWINSZ, &window_size ) )
		return;
	width_ = window_size.ws_col + 1;
	height_ = window_size.ws_row + 1;
	width_px_ = window_size.ws_xpixel;
	height_px_ = window_size.ws_ypixel;

	buffer->reset();
}

constexpr void TermiosDisplay::shift_cursor() noexcept {
	if ( ++cursor_x >= width_ ) {
		++cursor_y;
		cursor_x = 0;
	}
}

// IMPLEMENTATION lib::application::impl::TermiosDisplay::Buffer

/*constexpr */inline TermiosDisplay::Buffer::Buffer( TermiosDisplay & display ) noexcept
	: display{ display } { reset(); }

/*constexpr */inline void TermiosDisplay::Buffer::reset() noexcept {
	const auto size = display.width_ * display.height_;
	colors.resize( size );
	symbols.resize( size + /*'\0' terminator*/display.height_ );
}

/*constexpr */inline void TermiosDisplay::Buffer::clear( const Color & color ) noexcept {
	for ( auto & c : colors )
		c = color;
}

/*constexpr */inline void TermiosDisplay::Buffer::clear_bg( const TermiosDisplay::Color & color ) noexcept {
	for ( auto & c : colors )
		c.bg = color;
}

/*constexpr */inline void TermiosDisplay::Buffer::clear( char symbol_ ) noexcept {
	/// @note Because of '\0'.
	for ( u32 y = 0; y < display.height_; ++y ) {
	for ( u32 x = 0; x < display.width_; ++x ) {
		symbol( x, y ) = symbol_;
	}
	}
}

/*constexpr */inline TermiosDisplay::Buffer::Color & TermiosDisplay::Buffer::color( u32 x, u32 y ) noexcept {
	return colors[ y * display.width_ + x ];
}

/*constexpr */inline char & TermiosDisplay::Buffer::symbol( u32 x, u32 y ) noexcept {
	return symbols[ y * ( display.width_ + 1/*'\0'*/ ) + x ];
}

/*constexpr */inline auto TermiosDisplay::Buffer::get( u32 x, u32 y ) noexcept {
	return ::std::tie( color( x, y ), symbol( x, y ) );
}

} // lib::application::impl
