/* File: /lib/impl/stream/debug.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__debug__hpp
#define CPPLIB__lib__impl__stream__debug__hpp

#include <limits>

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::debug

namespace lib::stream::impl {

class debug final
	: public data::rwstream_t
{
public:
	debug();
	debug( data::rwstream_t * stream, usize min_rw, usize max_rw );
	virtual ~debug() = default;

	void reset();
	void reset( data::rwstream_t * stream, usize min_rw, usize max_rw );
	void shuffle();

	// IMPLEMENTATION lib::data::rstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	data::result_t peek( const data::buffer_t & buffer ) override;
	bool read_flush() override;
	data::result_t read_size() override;
	::std::error_condition read_error() const override;

	// IMPLEMENTATION lib::data::wstream_t

	data::result_t write( const data::cbuffer_t & buffer ) override;
	bool write_flush() override;
	data::result_t write_size() override;
	::std::error_condition write_error() const override;

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override;
	data::result_t size() override;
	::std::error_condition error() const override;
private:
	data::rwstream_t * stream;
	usize min_rw;
	usize max_rw;
	usize next_read;
	usize next_write;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__debug__hpp
