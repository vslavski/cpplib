/* File: /lib/impl/stream/buffer.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__buffer__hpp
#define CPPLIB__lib__impl__stream__buffer__hpp

#include <vector>

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::buffer

namespace lib::stream::impl {

/**
 *             ,-- read_flush() --, ,---  read_size() ---,
 * read_buff  [. . . . . . . . . . |....read_cbuffer()....]
 *     \                           |                       \
 *      \                       read_pos()              read_buff.size()
 * (same initial capacity)
 *      /
 *     /       ,--- write_flush() ---, ,-- write_size() --,
 * write_buff [.....write_buffer()....] . . . . . . . . . .]
 *                                   /                      \
 *                        write_buff.size()         write_buff.capacity()
 *                            write_pos()
 */

class buffer final
	: public data::rwstream_t
{
public:
	/// @todo buffer( rwstream_t, size )
	buffer( usize size );
	virtual ~buffer() = default;

	/// @todo reset( rwstream_t, size )
	void reset( data::rwstream_t * stream_ = nullptr );
	bool empty() const;

	// IMPLEMENTATION lib::data::rstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	data::result_t peek( const data::buffer_t & buffer ) override;
	bool read_flush() override;
	data::result_t read_size() override;
	data::result_t read_pos( usize position ) override;
	data::result_t read_pos() override;
	data::cbuffer_t read_cbuffer( bool flush ) override;
	::std::error_condition read_error() const override { return error(); }

	// IMPLEMENTATION lib::data::wstream_t

	data::result_t write( const data::cbuffer_t & buffer ) override;
	bool write_flush() override;
	data::result_t write_size() override;
	data::result_t write_pos( usize position ) override;
	data::result_t write_pos() override;
	data::buffer_t write_buffer( bool flush ) override;
	::std::error_condition write_error() const override { return error(); }

	// IMPLEMENTATION lib::data::rwstream_t

	bool flush() override { return read_flush() and write_flush(); }
	::std::error_condition error() const override;
private:
	/// @todo Implement ::lib::ringbuffer<> with interface like ::std::vector<>.

	data::rwstream_t * stream;

	usize read_pos_;
	::std::vector<u8> read_buff;
	::std::vector<u8> write_buff;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__buffer__hpp
