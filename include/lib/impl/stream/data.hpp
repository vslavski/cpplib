/* File: /lib/impl/stream/data.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__data__hpp
#define CPPLIB__lib__impl__stream__data__hpp

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::data

namespace lib::stream::impl {

/**
 *             ,-- read_flush() --, ,---  read_size() ---,
 * read_buff  [. . . . . . . . . . |....read_cbuffer()....]
 *      ^                          |                       \
 *      |                       read_pos()              read_buff.size()
 * same data/size
 *      |
 *      v      ,-- write_flush() --, ,--- write_size() ---,
 * write_buff [. . . . . . . . . . .|....write_buffer()....]
 *                                  |                       \
 *                              write_pos()              write_buff.size()
 */

/// @todo Rename to databuff.
class data final
	: public lib::data::rwstream_t
{
public:
	data( const lib::data::cbuffer_t & read_buff );
	data( const lib::data::buffer_t & write_buff );
	virtual ~data() = default;

	void reset( const lib::data::cbuffer_t & read_buff = {} );
	void reset( const lib::data::buffer_t & write_buff );
	bool empty() const { return read_buff.empty(); }
	bool readonly() const { return write_buff.empty(); }

	// IMPLEMENTATION lib::data::rstream_t

	lib::data::result_t read( const lib::data::buffer_t & buffer ) override;
	lib::data::result_t peek( const lib::data::buffer_t & buffer ) override;
	bool read_flush() override;
	lib::data::result_t read_size() override;
	lib::data::result_t read_pos( usize position ) override;
	lib::data::result_t read_pos() override;
	lib::data::cbuffer_t read_cbuffer( bool flush ) override;
	::std::error_condition read_error() const override { return error(); }

	// IMPLEMENTATION lib::data::wstream_t

	lib::data::result_t write( const lib::data::cbuffer_t & buffer ) override;
	bool write_flush() override;
	lib::data::result_t write_size() override;
	lib::data::result_t write_pos( usize position ) override;
	lib::data::result_t write_pos() override;
	lib::data::buffer_t write_buffer( bool flush ) override;
	::std::error_condition write_error() const override { return error(); }

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override;
	lib::data::result_t size() override;
	lib::data::result_t pos( usize position ) override;
	lib::data::result_t pos() override;
	::std::error_condition error() const override { return {}; }
private:
	lib::data::cbuffer_t read_buff;
	lib::data::buffer_t write_buff;

	usize read_pos_ = 0;
	usize write_pos_ = 0;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__data__hpp
