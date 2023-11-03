/* File: /lib/impl/codec/base64.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__codec__base64__hpp
#define CPPLIB__lib__impl__codec__base64__hpp

#include <vector>

#include "../../../lib/types.hpp"

#include "../../../lib/data/codec.hpp"

// DECLARATION lib::codec::impl::base64

namespace lib::codec::impl {

/** @brief Base64 codec
 *  @details _Base64 Data Encoding_ implementation based on [RFC 4648](https://www.rfc-editor.org/rfc/rfc4648.html).
 *  @see https://www.rfc-editor.org/rfc/rfc4648.html
 */
class base64
	: public data::codec_t
{
public:
	static constexpr usize BASE = 64;
	static constexpr usize BIT_COUNT = 6;
	static constexpr usize WORD_SIZE = 4;
	static constexpr usize WORD_BITS = BIT_COUNT * WORD_SIZE;
	static constexpr usize WORD_BYTES = WORD_BITS / 8/*bits*/;

	static bool IsValid( const data::cbuffer_t & buffer );

	base64() = default;
	base64( const data::cbuffer_t & buffer, Mode mode_ = Mode::ENCODE );
	virtual ~base64() = default;

	// IMPLEMENTATION lib::data::codec_t

	Mode mode() const override { return mode_; }
	void reset( Mode mode = Mode::UNDEFINED ) override;

	// IMPLEMENTATION lib::data::encoder_t, lib::data::decoder_t

	data::result_t encode( const data::cbuffer_t & plain ) override;
	data::result_t decode( const data::cbuffer_t & coded ) override;

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	bool read_flush() override;
	data::result_t read_size() override { return buffer_.size(); }
	data::cbuffer_t read_cbuffer( bool flush ) override;

	::std::error_condition write_error() const override { return error_; }
private:
	struct Word {
		constexpr Word() noexcept = default;
		constexpr Word( u8 first, u8 second, u8 third, u8 fourth ) noexcept;
		constexpr u8 first() const noexcept;
		constexpr u8 second() const noexcept;
		constexpr u8 third() const noexcept;
		constexpr u8 fourth() const noexcept;
		constexpr u8 first( u8 value ) noexcept;
		constexpr u8 second( u8 value ) noexcept;
		constexpr u8 third( u8 value ) noexcept;
		constexpr u8 fourth( u8 value ) noexcept;
		u8 data[WORD_BYTES] { 0 };
	};

	::std::error_condition error_;
	Mode mode_ = Mode::UNDEFINED;
	usize word_idx = 0;
	Word word;
	::std::vector<u8> buffer_;
};

} // namespace lib::codec::impl

#endif // CPPLIB__lib__impl__codec__base64__hpp
