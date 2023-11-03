/* File: /lib/impl/stream/stdio.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__stdio__hpp
#define CPPLIB__lib__impl__stream__stdio__hpp

#include "../../../lib/debug/features.hpp"

#include <system_error>

#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::stdio

namespace lib::stream::impl {

class stdio final
	: public data::unistream_t
{
public:
	stdio() = default;
	virtual ~stdio() = default;

	data::result_t putc( char symbol );
	data::result_t puts( const char * string );
	data::result_t printf( CPPLIB_PRINTF_ANNOTATION const char* format, ... )
		CPPLIB_PRINTF_ATTRIBUTE( format(printf,/*implicit this,*/2,3) );

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	data::result_t write( const data::cbuffer_t & buffer ) override;
	bool read_flush() override;
	bool write_flush() override;
	data::result_t read_size() override;
	data::result_t write_size() override;

	// IMPLEMENTATION lib::data::unistream_t, lib::data::rwstream_t

	bool flush() override { return write_flush(); }
	data::result_t size() override { return make_error_not_implemented(); }
	::std::error_condition error() const override { return error_; }
protected:
	data::result_t check_error( isize result );
	::std::error_condition error_;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__stdio__hpp
