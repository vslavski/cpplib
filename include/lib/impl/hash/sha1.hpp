/* File: /lib/impl/hash/sha1.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl__hash__sha1__hpp
#define CPPLIB__lib__impl__hash__sha1__hpp

#include <array>

#include "../../../lib/types.hpp"

#include "../../../lib/data/hash.hpp"

namespace lib::hash::impl {

// DECLARATION lib::hash::impl::sha1

/** @brief SHA-1 hash
 *  @details _US Secure Hash Algorithm 1_ implementation (_Method 1_) based on [RFC 3174](https://www.rfc-editor.org/rfc/rfc3174.html).
 *  @see https://www.rfc-editor.org/rfc/rfc3174.html
 */
class sha1
	: public data::hash_t
{
	using Super = data::hash_t;
public:
	static constexpr usize WORD_BYTES = 4;
	static constexpr usize HASH_WORDS = 5;
	static constexpr usize HASH_BYTES = HASH_WORDS * WORD_BYTES;
	static constexpr usize BLOCK_BITS = 512;
	static constexpr usize BLOCK_BYTES = BLOCK_BITS / 8/*bits*/;
	static constexpr usize BLOCK_WORDS = BLOCK_BYTES / WORD_BYTES;
	static constexpr usize LENGTH_BITS = 64;
	static constexpr usize LENGTH_BYTES = LENGTH_BITS / 8/*bits*/;
	using Word = u32;
	using Length = u64;

	sha1();
	sha1( const data::cbuffer_t & buffer );

	// IMPLEMENTATION lib::data::hash_t

	void reset() override;

	// IMPLEMENTATION lib::data::rstream_t, lib::data::wstream_t

	data::result_t read( const data::buffer_t & buffer ) override;
	bool read_flush() override;
	data::result_t read_size() override { return HASH_BYTES; }
	data::cbuffer_t read_cbuffer( bool flush ) override;
	using rstream_t::read_cbuffer;

	data::result_t write( const data::cbuffer_t & buffer ) override;
	data::result_t write_size() override { return (usize) length; }
	data::buffer_t write_buffer( bool flush ) override;
	data::cbuffer_t write_cbuffer( bool flush ) override;
	using wstream_t::write_cbuffer;
	data::result_t write_pos() override { return (usize) (length % BLOCK_BYTES); }
	using Super::write_pos;
	::std::error_condition write_error() const override { return {}; }
private:
	static constexpr Word f( Word t, Word B, Word C, Word D ) noexcept;
	static constexpr Word K( Word t ) noexcept;

	void pack_hash();
	void unpack_hash();

	void process_block();

	usize flush_bit( usize offset );
	void flush_pad( usize offset );
	void flush_length();

	bool flushed;
	Length length;
	::std::array<u8, HASH_BYTES> hash_buff;
	::std::array<u8, BLOCK_BYTES> block_buff;

	::std::array<Word, HASH_WORDS> H;
};

} // namespace lib::hash::impl

#endif // CPPLIB__lib__impl__hash__sha1__hpp
