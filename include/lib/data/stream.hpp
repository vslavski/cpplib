/* File: /lib/data/stream.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__data__stream__hpp
#define CPPLIB__lib__data__stream__hpp

#include <system_error>

#include "../../lib/tl/result.hpp"
#include "../../lib/types.hpp"
#include "../../lib/system/error.hpp"

#include "./buffer.hpp"

/// @todo noexcept?

namespace lib::data {

// DECLARATION lib::data::result_t

using result_t = tl::result_numeric_t<usize>;

// DECLARATION lib::data::rstream_t

class rstream_t {
public:
	virtual ~rstream_t() = default;

	virtual result_t read( const buffer_t & buffer ) = 0;
	virtual result_t peek( const buffer_t &/* buffer*/ ) { return make_error_not_implemented(); }
	virtual bool read_flush() { return false; }

	virtual result_t read_size() = 0;
	virtual result_t read_pos( usize/* position*/ ) { return make_error_not_implemented(); }
	virtual result_t read_pos() { return make_error_not_implemented(); }
	virtual buffer_t read_buffer( bool/* flush*/ ) { return {}; }
	virtual cbuffer_t read_cbuffer( bool/* flush*/ ) { return {}; }
	buffer_t read_buffer() { return read_buffer( false ); }
	cbuffer_t read_cbuffer() { return read_cbuffer( false ); }

	virtual ::std::error_condition read_error() const = 0;
};

// DECLARATION lib::data::wstream_t

class wstream_t {
public:
	virtual ~wstream_t() = default;

	virtual result_t write( const cbuffer_t & buffer ) = 0;
	/// @todo Add rewrite(pos, buffer) ?
	virtual bool write_flush() { return false; }

	virtual result_t write_size() = 0;
	virtual result_t write_pos( usize/* position*/ ) { return make_error_not_implemented(); }
	virtual result_t write_pos() { return make_error_not_implemented(); }
	virtual buffer_t write_buffer( bool/* flush*/ ) { return {}; }
	virtual cbuffer_t write_cbuffer( bool/* flush*/ ) { return {}; }
	buffer_t write_buffer() { return write_buffer( false ); }
	cbuffer_t write_cbuffer() { return write_cbuffer( false ); }

	virtual ::std::error_condition write_error() const = 0;
};

// DECLARATION lib::data::rwstream_t

class rwstream_t
	: public rstream_t
	, public wstream_t
{
public:
	virtual ~rwstream_t() = default;

	virtual bool flush() { return false; }

	virtual result_t size() { return make_error_not_implemented(); }
	virtual result_t pos( usize/* position*/ ) { return make_error_not_implemented(); }
	virtual result_t pos() { return make_error_not_implemented(); }

	virtual ::std::error_condition error() const = 0;
};

// DECLARATION lib::data::unistream_t

class unistream_t
	: public rwstream_t
{
public:
	virtual ~unistream_t() = default;

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override = 0;
	result_t size() override = 0;
	using rwstream_t::pos;
	using rwstream_t::error;

	// IMPLEMENTATION lib::data::rstream_t

	bool read_flush() override { return flush(); }
	result_t read_size() override { return size(); }
	result_t read_pos( usize position ) override { return pos( position ); }
	result_t read_pos() override { return pos(); }
	::std::error_condition read_error() const override { return error(); }

	// IMPLEMENTATION lib::data::wstream_t

	bool write_flush() override { return flush(); }
	result_t write_size() override { return size(); }
	result_t write_pos( usize position ) override { return pos( position ); }
	result_t write_pos() override { return pos(); }
	::std::error_condition write_error() const override { return error(); }
};

} // namespace lib::data

#endif // CPPLIB__lib__data__stream__hpp
