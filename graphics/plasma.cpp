/* File: graphics/plasma.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cstdlib>
#include <cmath>
#include <ctime>

#include <array>
#include <vector>
#include <algorithm>
#include <compare>
#include <chrono>
#include <thread>

#include <lib/literals.hpp>
#include <lib/cstring.hpp>
#include <lib/utils/ansi_colors.hpp>
#include <lib/impl/stream/stdio.hpp>
#include <lib/impl_posix/application/termios_display.hpp>

#define PARTIAL

using vec3 = ::std::array<float, 3>;

template< class/*float*/...Args >
inline float gen_wave
	( float offset, float x, float amplitude1, float frequency1, Args...args ) noexcept
{
	using ::std::sin;
	const float wave = amplitude1 * sin( offset + x * frequency1 );
	if constexpr ( sizeof...(Args) > 0 )
		return wave + gen_wave( offset, x, args... );
	return wave;
}

namespace std {

inline vec3 sin( const vec3 & vec ) {
	using ::std::sin;
	return { sin( vec[0] ), sin( vec[1] ), sin( vec[2] ) };
}

inline vec3 cos( const vec3 & vec ) {
	using ::std::cos;
	return { cos( vec[0] ), cos( vec[1] ), cos( vec[2] ) };
}

inline vec3 pow( const vec3 & vec, float exp ) {
	using ::std::pow;
	return { pow( vec[0], exp ), pow( vec[1], exp ), pow( vec[2], exp ) };
}

} // namespace std

inline vec3 fx_main( float time, float x, float y ) {
	(void) time;
	(void) x;
	(void) y;

	using ::std::sin;
	using ::std::cos;
	using ::std::pow;
	using ::std::min;
	using ::std::max;
	using ::std::abs;

	x *= .03f;
	time *= 4.8f;

	vec3 mix;(void)mix;

	x *= .47f;
	y *= 1.5f;
	y -= .25f;

	auto wave_1 = [time]( float x, float y ) -> float {
		return sin
			( gen_wave( time, x
				, .18f, 13.f, .15f, 2.67f, .10f, 32.1f )
			+ y * float (M_PI) * .94f
			);
	};(void)wave_1;
	auto wave_2 = [time]( float x, float y ) -> float {
		return sin
			( gen_wave( time, x
				, .17f, 9.3f, .20f, 26.67f, .11f, 11.1f )
			+ y * float (M_PI)
			);
	};(void)wave_2;
	auto wave_3 = [time]( float x, float y ) -> float {
		return sin
			( gen_wave( time, x
				, .14f, 64.19f, .29f, 3.67f, .21f, 22.67f )
			+ y * float (M_PI)
			);
	};(void)wave_3;

	y += .2f;
	vec3 col_1 =
		{ .51111f + .5f * wave_1( x + .05f, y - .05f )
		, .51111f + .5f * wave_1( x, y + .05f )
		, .51111f + .5f * wave_1( x - .05f, y ) };
	col_1 = pow( col_1, 7.7f );
	y -= .2f;

	vec3 col_2 =
		{ .51111f + .5f * wave_2( x, y + .01f )
		, .51111f + .5f * wave_2( x - .01f, y )
		, .51111f + .5f * wave_2( x + .01f, y - .01f ) };
	col_2 = pow( col_2, 5.36f );

	y -= .2f;
	vec3 col_3 =
		{ .51111f + .5f * wave_3( x - .01f, y )
		, .51111f + .5f * wave_3( x + .01f, y )
		, .51111f + .5f * wave_3( x, y + .01f ) };
	col_3 = pow( col_3, 4.17f );
	y += .2f;

#ifndef MAT1________
	float r = .45f * ( col_1[0] + col_2[0] + col_3[0] );
	float g = .45f * ( col_1[1] + col_2[1] + col_3[1] );
	float b = .45f * ( col_1[2] + col_2[2] + col_3[2] );
#else
	float r = .34f * ( col_1[0] + col_1[1] + col_1[2] );
	float g = .34f * ( col_2[0] + col_2[1] + col_2[2] );
	float b = .34f * ( col_3[0] + col_3[1] + col_3[2] );
#endif // ...
	mix = { r, g, b };

	r *= .81f;
	r += .04f * mix[1] * sin( time * .33f );
	r += .15f * mix[2] * cos( time * .45f );
	g *= .72f;
	g += .12f * mix[0] * cos( time * .59f );
	g += .16f * mix[2] * sin( time * .71f );
	b *= .97f;
	b += .02f * mix[0] * sin( time * .16f );
	b += .01f * mix[1] * cos( time * .81f );

/*	r = max( 0.f, r );
	g = max( 0.f, g );
	b = max( 0.f, b );
*/	r = abs( r );
	g = abs( g );
	b = abs( b );

	r = pow( r, .1f + ( 1.f * abs( sin( time * .3f ) ) ) );
	g = pow( g, .1f + ( 1.f * abs( cos( time * .4f ) ) ) );
	b = pow( b, .1f + ( 1.f * abs( sin( time * .2f ) ) ) );

#if 0
	// grayscale (fast)
	float r_ = pow( r - .085f, 8.f );
	float g_ = pow( g - .085f, 8.f );
	float b_ = pow( b - .085f, 8.f );
	float rgb = .33333f * ( r_ + g_ + b_ );
	const float factor = .28f;
	r_ = r_ * factor + rgb * ( 1.f - factor );
	g_ = g_ * factor + rgb * ( 1.f - factor );
	b_ = b_ * factor + rgb * ( 1.f - factor );
	r = r * factor + r_ * ( 1.f - factor );
	g = g * factor + g_ * ( 1.f - factor );
	b = b * factor + b_ * ( 1.f - factor );
#endif // grayscale on

#if 0
	r *= 1.1f * ( 1 + sin( time * .95f + y * float (M_PI) ) );
	r = (float)(int) r;
	r /= 1.1f * ( 1 + sin( time * .97f + y * float (M_PI) ) );
	g *= 49.f;
	g = (float)(int) g;
	g /= 55.f;
	b *= 46.f;
	b = (float)(int) b;
	b /= 37.f;
#endif // 0 or 1

	return { r, g, b };
}

inline vec3 fx_artefacts( vec3 col, float time, float x, float y ) {
	y *= 2.f;
	y-= 0.5f;
	auto wave_1 = [time]( float x, float y ) -> float {
		using ::std::sin;
		return sin
			( gen_wave( time * 1.27f, x
				, .88f, 13.f, 0.85f, 4.67f, .22f, 22.1f )
			+ y * float (M_PI) * .94f
			);
	};(void)wave_1;
	using ::std::pow;
	using ::std::min;
	using ::std::max;
	auto v = wave_1( x, y );
	v = pow( v, 15.f );
	v = min( 1.f, v );
	v = max( 0.f, v );
	col[0] *= v;
	col[1] *= v;
	col[2] *= v;
	return col;
}

inline vec3 process( float time, float x, float y ) {
	//::std::swap( x, y );
	auto col = fx_main( time, x, y );
	col = fx_artefacts( col, time, x, y );
	return col;
}

int main() {
	using namespace ::lib;
	using namespace ::std::chrono_literals;

	::std::srand( (u32) ::std::time( nullptr ) );

	application::impl::TermiosDisplay display;
	display.clear_bg({ 0x00, 0x00, 0x00 });

	::std::chrono::system_clock::duration duration = 0ms;
	f32 time = (f32) ( ::std::rand() % 100 );
	usize frames_count = 0;
	f32 frames_time = 0.f;

	lib::stream::impl::stdio stdio_stream;

	while ( stdio_stream.read_size() == 0_sz ) {
		const auto frame_begin = ::std::chrono::system_clock::now();
		f32 dt = (f32) duration.count() / (f32) decltype(duration)::period::den;

#ifdef PARTIAL
		static constexpr u32 SAMPLES_PART = 1;
		static constexpr u32 SAMPLES_LEAVE = 500;
		::std::vector<bool> samples_map( display.width() * display.height(), false );
		u32 samples = display.width() * display.height() / SAMPLES_PART - SAMPLES_LEAVE;
		const auto max_display_dim = ::std::max( display.width(), display.height() );
		const f32 sub_dt = dt / (f32) max_display_dim;
		while ( samples-- > 0 ) {{
			if ( samples % max_display_dim == 0 )
				time += sub_dt;
			u32 x, y;
			do {
				x = (u32) ::std::rand() % display.width();
				y = (u32) ::std::rand() % ( display.height() - 1 );
			} while ( samples_map[ y * display.width() + x ] );
			samples_map[ y * display.width() + x ] = true;
			display.set_cursor( x, y );
#else
		time += dt;
		display.set_cursor( 0, 0 );
		for ( u32 y = 0; y < display.height() - 1; ++y ) {
		for ( u32 x = 0; x < display.width(); ++x ) {
#endif // PARTIAL
			auto col = process( time
				, (f32) x / (f32) display.width(), (f32) y / (f32) display.height() );
			for ( auto & c : col ) c = ::std::max( 0.f, ::std::min( 1.f, c ) );
			display.draw_bg({
					(u8) ( col[0] * 255.f ), (u8) ( col[1] * 255.f ), (u8) ( col[2] * 255.f ) });
			/// @bug No display cursor shift while optimization enabled in set_pixel.
		}}

		const auto frame_end = ::std::chrono::system_clock::now();
		duration = ::std::chrono::duration_cast<decltype(duration)>( frame_end - frame_begin );
		if ( duration < 16.555ms )
			::std::this_thread::sleep_for( 16.555ms - duration );

		++frames_count;
		frames_time += dt;
		if ( frames_time > 0.5f ) {
			const auto fps = (f32) frames_count / frames_time;
			frames_count = 0;
			frames_time -= 1.0f;
			display.set_cursor( 0, display.height() - 1 );
			stdio_stream.printf( LIB_UTILS_ANSI_COLOR_DEFAULT
								"FPS: %#8.3f   Last Frame Time: %#9.5f sec ", fps, dt );
			static constexpr cstring PRESS_ENTER_TO_EXIT_MSG {"Press <ENTER> to close"};
			display.set_cursor( display.width() - (u32) PRESS_ENTER_TO_EXIT_MSG.size(), display.height() - 1 );
			stdio_stream.printf( PRESS_ENTER_TO_EXIT_MSG.data() );
		}
	}

	stdio_stream.printf( LIB_UTILS_ANSI_COLOR_DEFAULT "\n" );

	return 0;
}
