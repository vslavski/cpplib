/* File: /lib/impl/codec/xorkey.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__codec__xorkey__hpp
#define CPPLIB__lib__impl__codec__xorkey__hpp

#include <vector>

#include <cpp/lib_debug>

#include "../../../lib/types.hpp"

#include "../../../lib/data/codec.hpp"

// DECLARATION lib::codec::impl::xorkey

namespace lib::codec::impl {

class xorkey
	: public data::codec_t
{
public:
	static data::result_t Process( const data::buffer_t & buffer, const data::cbuffer_t & key, usize key_idx = 0 );

	xorkey() = default;
	xorkey( const data::cbuffer_t & key_buff ) : key_{ key_buff.begin(), key_buff.end() } {}
	virtual ~xorkey() = default;

	data::cbuffer_t key() const { return key_; }

	void reset( const data::cbuffer_t & key_buff = {} );

	// IMPLEMENTATION lib::data::codec_t

	Mode mode() const override { return Mode::IGNORED; }
	void reset( Mode mode ) override { CPP_UNUSED( mode ); reset(); }

	// IMPLEMENTATION lib::data::encoder_t, lib::data::decoder_t

	data::result_t encode( const data::cbuffer_t & buffer ) override { return write( buffer ); }
	data::result_t decode( const data::cbuffer_t & buffer ) override { return write( buffer ); }

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	bool read_flush() override { return true; }
	data::result_t read_size() override { return buffer_.size(); }
	data::cbuffer_t read_cbuffer( bool/* flush*/ ) override { return buffer_; }
	using data::rstream_t::read_cbuffer;

	data::result_t write( const data::cbuffer_t & buffer ) override;
	::std::error_condition write_error() const override { return {}; }
private:
	usize key_idx = 0;
	::std::vector<u8> key_;
	::std::vector<u8> buffer_;
};

} // namespace lib::codec::impl

#endif // CPPLIB__lib__impl__codec__xorkey__hpp
