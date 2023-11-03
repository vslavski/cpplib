/* File: /lib/data/codec.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__data__codec__hpp
#define CPPLIB__lib__data__codec__hpp

#include "../../lib/system/error.hpp"

#include "./stream.hpp"

namespace lib::data {

// DECLARATION lib::data::encoder_t

class encoder_t
	: public virtual rwstream_t
{
public:
	virtual ~encoder_t() = default;

	virtual result_t encode( const cbuffer_t & plain ) = 0;
	virtual bool encode_flush() { return read_flush(); }

	virtual result_t encode_size() { return read_size(); }
	virtual cbuffer_t encode_cbuffer( bool flush ) { return read_cbuffer( flush ); }
	cbuffer_t encode_cbuffer() { return encode_cbuffer( false ); }

	virtual ::std::error_condition encode_error() const { return write_error(); }

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	result_t read( const buffer_t & buffer ) override = 0;
	bool read_flush() override = 0;
	result_t read_size() override = 0;
	cbuffer_t read_cbuffer( bool flush ) override = 0;
	::std::error_condition read_error() const override { return write_error(); }

	result_t write( const cbuffer_t & buffer ) override { return encode( buffer ); }
	bool write_flush() override { return read_flush(); }
	result_t write_size() override { return make_error_not_implemented(); }
	::std::error_condition write_error() const override = 0;
};

// DECLARATION lib::data::decoder_t

class decoder_t
	: public virtual rwstream_t
{
public:
	virtual ~decoder_t() = default;

	virtual result_t decode( const cbuffer_t & plain ) = 0;
	virtual bool decode_flush() { return read_flush(); }

	virtual result_t decode_size() { return read_size(); }
	virtual cbuffer_t decode_cbuffer( bool flush ) { return read_cbuffer( flush ); }
	cbuffer_t decode_cbuffer() { return decode_cbuffer( false ); }

	virtual ::std::error_condition decode_error() const { return write_error(); }

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	result_t read( const buffer_t & buffer ) override = 0;
	bool read_flush() override = 0;
	result_t read_size() override = 0;
	cbuffer_t read_cbuffer( bool flush ) override = 0;
	::std::error_condition read_error() const override { return write_error(); }

	result_t write( const cbuffer_t & buffer ) override { return decode( buffer ); }
	bool write_flush() override { return read_flush(); }
	result_t write_size() override { return make_error_not_implemented(); }
	::std::error_condition write_error() const override = 0;
};

// DECLARATION lib::data::codec_t

class codec_t
	: public virtual rwstream_t
	, public encoder_t
	, public decoder_t
{
public:
	enum class Mode { UNDEFINED, ENCODE, DECODE, IGNORED };

	virtual ~codec_t() = default;

	virtual Mode mode() const = 0;
	virtual void reset( Mode mode = Mode::UNDEFINED ) = 0;

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	result_t read( const buffer_t & buffer ) override = 0;
	result_t peek( const buffer_t & buffer ) override { return read( buffer ); }
	bool read_flush() override = 0;
	result_t read_size() override = 0;
	cbuffer_t read_cbuffer( bool flush ) override = 0;
	::std::error_condition read_error() const override { return write_error(); }

	result_t write( const cbuffer_t & buffer ) override;
	bool write_flush() override { return read_flush(); }
	result_t write_size() override { return make_error_not_implemented(); }
	::std::error_condition write_error() const override = 0;

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override { return read_flush();  }
	result_t size() override { return read_size(); }
	::std::error_condition error() const override { return write_error(); }
};

} // namespace lib::data

// SPECIALIZATION lib::str()

#include "../../lib/cstring.hpp"

namespace lib {
inline cstring str( data::codec_t & codec ) {
	const auto & buffer = codec.read_cbuffer( true );
	return {(const char*) buffer.data(), buffer.size()};
}
} // namespace lib

#endif // CPPLIB__lib__data__codec__hpp
