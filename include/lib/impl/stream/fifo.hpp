/* File: /lib/impl/stream/fifo.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__fifo__hpp
#define CPPLIB__lib__impl__stream__fifo__hpp

#include <vector>

#include "../../../lib/types.hpp"
#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::fifo

namespace lib::stream::impl {

/**
 *             ,-- read_flush() --, ,---  read_size() ---,
 * read_buff  [. . . . . . . . . . |....read_cbuffer()....]
 *     \                           |                       \
 *      \                       read_pos()              read_buff.size()
 * (same object)
 *      /
 *     /       ,--- write_flush() ---, ,-- write_size() --...
 * write_buff [.....write_buffer()....] . . . . . . . . . ...
 *                                   /                       \
 *                        write_buff.size()              RAM limit
 *                            write_pos()
 */

class fifo final
	: public data::rwstream_t
{
public:
	fifo() = default;
	virtual ~fifo() = default;

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

	bool flush() override;
	data::result_t size() override;
	::std::error_condition error() const override { return {}; }
private:
	/// @todo Implement ::lib::ringfifo<> with interface like ::std::vector<>.

	::std::vector<u8> buffer_;
	usize read_pos_ = 0;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__fifo__hpp
