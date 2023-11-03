/* File: /lib/impl/stream/file.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__stream__file__hpp
#define CPPLIB__lib__impl__stream__file__hpp

#include <cstdio>

#include <system_error>

#include "../../../lib/data/stream.hpp"

// DECLARATION lib::stream::impl::file

namespace lib::stream::impl {

class file final
	: public data::unistream_t
{
public:
	/// @todo Add open mode constants/enum?
	/// @todo Check `$ man fopen`
	void dbg_test();

	file() = default;
	file( const char * name, const char * mode = "r" );
	file( FILE * file_handle );
	virtual ~file();

	file & operator = ( FILE * file_handle );

	bool open( const char * name, const char * mode = "r" );
	bool close();
	bool is_open() const { return file_ != nullptr; }
	void reset();

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	data::result_t peek( const data::buffer_t & buffer ) override;
	data::result_t write( const data::cbuffer_t & buffer ) override;

	// IMPLEMENTATION lib::data::unistream_t

	bool flush() override;
	data::result_t size() override;

	// IMPLEMENTATION lib::data::rwstream_t

	data::result_t pos( usize position ) override;
	data::result_t pos() override;
	::std::error_condition error() const override { return error_; }
private:
	const ::std::error_condition & set_error_from_errno();

	FILE * file_ = nullptr;
	::std::error_condition error_;
};

} // namespace lib::stream::impl

#endif // CPPLIB__lib__impl__stream__file__hpp
