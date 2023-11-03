/* File: /lib/impl_posix/application/termios_display.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__application__termios_display__hpp
#define CPPLIB__lib__impl_posix__application__termios_display__hpp

#include <termios.h>

#include <vector>
#include <compare>

#include "../../../lib/types.hpp"
#include "../../../lib/ptr.hpp"
#include "../../../lib/impl/stream/stdio.hpp"

namespace lib::application::impl {

// DECLARATION: lib::application::impl::TermiosDisplay

class TermiosDisplay {
public:
	/// @todo Move 'Color' to something general, e.g. 'glsl'-like.
	struct Color {
		u32 r, g, b;
		constexpr auto operator <=> ( const Color & other ) const noexcept = default;
		constexpr bool operator == ( const Color & other ) const noexcept {
			return r == other.r and g == other.g and b == other.b;
		}
	};

	/*constexpr */TermiosDisplay() noexcept;
	/*constexpr */~TermiosDisplay() noexcept;

	/*constexpr */void show_cursor( bool show = true ) noexcept;
	/*constexpr */void hide_cursor() noexcept;

	/*constexpr */void clear_bg( const TermiosDisplay::Color & color ) noexcept;

	/*constexpr */void set_cursor( u32 x, u32 y ) noexcept;
	/*constexpr */void draw_bg( const TermiosDisplay::Color & color ) noexcept;
	/*constexpr */void set_bg( u32 x, u32 y, const TermiosDisplay::Color & color ) noexcept;

	constexpr u32 width() const noexcept { return width_; }
	constexpr u32 height() const noexcept { return height_; }
	constexpr u32 width_px() const noexcept { return width_px_; }
	constexpr u32 height_px() const noexcept { return height_px_; }
private:
	struct Buffer;

	/*constexpr */void update_size() noexcept;
	constexpr void shift_cursor() noexcept;
	constexpr void update( u32 x, u32 y ) noexcept;

	struct ::termios termios_settings;

	u32 width_ = 0;
	u32 height_ = 0;
	u32 width_px_ = 0;
	u32 height_px_ = 0;

	/// @todo Get initial value from "\033[6n" command.
	u32 cursor_x;
	u32 cursor_y;

	lib::stream::impl::stdio stream;
	Ptr<Buffer> buffer;
};

// DECLARATION: lib::application::impl::TermiosDisplay::Buffer

struct TermiosDisplay::Buffer final {
	struct Color {
		TermiosDisplay::Color fg, bg;
	};

	/*constexpr */Buffer( TermiosDisplay & display ) noexcept;

	/*constexpr */void reset() noexcept;

	/*constexpr */void clear( const Color & color ) noexcept;
	/*constexpr */void clear_bg( const TermiosDisplay::Color & color ) noexcept;
	/*constexpr */void clear( char symbol ) noexcept;

	/*constexpr */Color & color( u32 x, u32 y ) noexcept;
	/*constexpr */char & symbol( u32 x, u32 y ) noexcept;
	/*constexpr */auto get( u32 x, u32 y ) noexcept;
private:
	TermiosDisplay & display;
	::std::vector<Color> colors;
	::std::vector<char> symbols;
};

} // lib::application::impl

#endif // CPPLIB__lib__impl_posix__application__termios_display__hpp
