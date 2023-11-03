/* File: /lib/data/codec.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include "./codec.hpp"

namespace lib::data {

// IMPLEMENTATION lib::data::codec_t: lib::data::wstream_t

result_t codec_t::write( const cbuffer_t & buffer )/* override*/ {
	switch ( mode() ) {
	case Mode::UNDEFINED:	break;
	case Mode::ENCODE:		return encode( buffer );
	case Mode::DECODE:		return decode( buffer );
	case Mode::IGNORED:		break;
	}
	return make_error_not_configured();
}

} // namespace lib::data
