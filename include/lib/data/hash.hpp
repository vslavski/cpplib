/* File: /lib/data/hash.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__data__hash__hpp
#define CPPLIB__lib__data__hash__hpp

#include "../../lib/system/error.hpp"

#include "./stream.hpp"

// DECLARATION lib::data::hash_t

namespace lib::data {

class hash_t
	: public rwstream_t
{
public:
	virtual ~hash_t() = default;

	result_t update( const cbuffer_t & data ) { return write( data ); }
	cbuffer_t hash( bool flush = true ) { return read_cbuffer( flush ); }

	virtual void reset() = 0;

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	result_t read( const buffer_t & buffer ) override = 0;
	result_t peek( const buffer_t & buffer ) override { return read( buffer ); }
	bool read_flush() override = 0;
	result_t read_size() override = 0;
	cbuffer_t read_cbuffer( bool flush ) override = 0;
	::std::error_condition read_error() const override { return write_error(); }

	result_t write( const cbuffer_t & buffer ) override = 0;
	bool write_flush() override { return read_flush(); }
	result_t write_size() override { return make_error_not_implemented(); }
	::std::error_condition write_error() const override = 0;

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override { return read_flush();  }
	result_t size() override { return read_size(); }
	::std::error_condition error() const override { return write_error(); }
};

} // namespace lib

#endif // CPPLIB__lib__data__hash__hpp
