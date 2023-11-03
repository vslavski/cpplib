/* File: /lib/bus/filter.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__bus__filter__hpp
#define CPPLIB__lib__bus__filter__hpp

#include <cstring>

#include <vector>

#include <cpp/lib_concepts>
#include <cpp/lib_algorithm>

#include "../../lib/tl/noncopyable.hpp"

#include "./message.hpp"

namespace lib::bus {

/// @todo MessageFilterSync?
// DECLARATION lib::bus::MessageFilter<>

template< ::cpp::IntOrEnum MessageIdType >
class MessageFilter
	: tl::noncopymovable< MessageFilter<MessageIdType> >
{
public:
	using message_id_type = MessageIdType;
	using message_type = Message<message_id_type>;
	using less_type = bool (MessageFilter::*)( const MessageFilter & ) const noexcept;

	constexpr MessageFilter( bool once = false, less_type less = nullptr ) noexcept
		: less{ less }
		, once{ once }
	{}
	virtual ~MessageFilter() noexcept = default;

	constexpr bool operator < ( const MessageFilter & other ) const noexcept {
		if ( once != other.once )
			return once;
		if ( less != nullptr and less == other.less )
			return (this->*less)( other );
		/// @hack It's possible 'memcmp' result will be undefined, maybe that's why
		/// C++ provide only 'operator ==' for pointer to member functions.
		return ::std::memcmp( &less, &other.less, sizeof(less) ) < 0;
	}
	virtual bool operator () ( const message_type &/* message*/ ) const noexcept {
		return true;
	}

	constexpr bool is_once() const noexcept { return once; }
private:
	/// @note 'less' can't be mutable, because filters are stored in map containers,
	/// thus the result of 'operator <' shouldn't be changed after instantiation.
	/// At the other end it maybe gives some "flexebility" on building complex filters.
	const less_type less;

	const bool once;
};

/// @todo MultiFilterSync?
// DECLARATION lib::bus::MultiFilter<>

template< ::cpp::IntOrEnum MessageIdType >
class MultiFilter
	: private MessageFilter< MessageIdType >
{
	using Super = MessageFilter<MessageIdType>;
public:
	using message_id_type = typename Super::message_id_type;
	using message_type = typename Super::message_type;
	using less_type = typename Super::less_type;

	constexpr MultiFilter( bool once, auto...message_ids ) noexcept
		: MultiFilter{ once, less_type{}, message_ids... }
	{}
	constexpr MultiFilter( bool once, less_type less, auto...message_ids ) noexcept
		: Super{ once, less }
		, message_ids{ message_ids... }
	{}
	virtual ~MultiFilter() noexcept = default;

	constexpr bool operator < ( const MultiFilter & other ) const noexcept {
		return message_ids < other.message_ids
			or Super::operator<( other );
	}
	bool operator () ( const message_type & message ) const noexcept override {
		return ::cpp::contains( message_ids, message.id );
	}

	using Super::is_once;
private:
	/// @see note for 'MessageFilter::less'
	const ::std::vector<message_id_type> message_ids;
};

} // namespace lib::bus

#endif // CPPLIB__lib__bus__filter__hpp
