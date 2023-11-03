/* File: /lib/bus/manager.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__bus__manager__hpp
#define CPPLIB__lib__bus__manager__hpp

#include <type_traits>
#include <utility>
#include <map>

#include <cpp/lib_algorithm>
#include <cpp/lib_concepts>
#include <cpp/lib_debug>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/delegate.hpp"
#include "../../lib/ptr.hpp"

#include "./tag.hpp"
#include "./message.hpp"
#include "./filter.hpp"

namespace lib::bus {

// DECLARATION lib::bus:Manager<>

template< ::cpp::IntOrEnum MessageIdType >
class Manager
	: tl::noncopymovable< Manager<MessageIdType> >
{
public:
	using message_id_type = MessageIdType;
	using message_type = Message<message_id_type>;
	/// @warning Current 'message_delegate_type' is applicable for notify_sync() only,
	/// because raw message pointer will be transferred: &MsgImpl -> void * -> MsgImpl *.
	using message_delegate_type = tl::delegate<const void *>;
	using message_filter_type = MessageFilter<message_id_type>;
	using message_filter_ptr = SPtr<message_filter_type>;
	using multi_delegate_type = tl::delegate<const message_type &>;
	using multi_filter_type = MultiFilter<message_id_type>;
	using multi_filter_ptr = SPtr<multi_filter_type>;

	template< class T, ::cpp::MemberFunction Fn >
	constexpr message_filter_ptr listen_sync( message_filter_ptr & filter, T & listener, Fn && fn ) noexcept {
		using Message = ::std::remove_cvref_t< decltype(get_message_type( fn )) >;
		auto & filters = sync_message_filters[ Message::ID ];
		return add_to_filters( filters, filter, listener, fn );
	}

	template< class T, ::cpp::MemberFunction Fn, ::cpp::MemberFunction...Others >
	constexpr void listen_sync( T & listener, Fn && fn, Others&&...others ) noexcept {
		CPP_UNUSED( listen_sync( default_message_filter, listener, fn ) );
		if constexpr ( sizeof...(others) > 0 )
			listen_sync( listener, /** @todo forward? */others... );
	}

	template< class T, ::cpp::MemberFunction Fn >
	constexpr multi_filter_ptr listen_sync( multi_filter_ptr & filter, T & listener, Fn && fn ) noexcept {
		return add_to_filters( sync_multi_filters, filter, listener, fn );
	}

	template< class T, ::cpp::MemberFunction Fn >
	constexpr bool unlisten_sync( message_filter_ptr & filter, T & listener, Fn && fn ) noexcept {
		using Message = ::std::remove_cvref_t< decltype(get_message_type( fn )) >;
		auto & filters = sync_message_filters[ Message::ID ];
		return remove_from_filters( filters, filter, listener, fn );
	}

	template< class T, ::cpp::MemberFunction Fn, ::cpp::MemberFunction...Others >
	constexpr void unlisten_sync( T & listener, Fn && fn, Others&&...others ) noexcept {
		/// @warning Copy default filter, otherwise it became unreferenced.
		auto filter = default_message_filter;
		CPP_UNUSED( unlisten_sync( filter, listener, fn ) );
		if constexpr ( sizeof...(others) > 0 )
			unlisten_sync( listener, /** @todo forward? */others... );
	}

	template< class T, ::cpp::MemberFunction Fn >
	constexpr bool unlisten_sync( multi_filter_ptr & filter, T & listener, Fn && fn ) noexcept {
		return remove_from_filters( sync_multi_filters, filter, listener, fn );
	}

	template< class Message, class T, class...Args >
	constexpr void notify_sync( const tag_bus<T> &/* sender*/, Args&&...args ) noexcept {
		const Message message{ /*sender, */::std::forward<Args>( args )... };
		notify_with_filters( sync_message_filters[ Message::ID ], message );
		notify_with_filters( sync_multi_filters, message );
	}
private:
	/// @meta
	template< class T, class Message >
	static constexpr Message & get_message_type( void(T::*)(const Message *) ) noexcept;

	constexpr auto add_to_filters( auto & filters, const auto & filter_, auto & listener, auto && fn ) noexcept {
		auto [ it, is_new_filter ] = filters.try_emplace( filter_ );
		auto & [ filter, delegate ] = *it;
		delegate += { listener, /** @todo forward? */fn };
		return filter;
	}

	constexpr auto remove_from_filters( auto & filters, auto & filter_, auto & listener, auto && fn ) noexcept {
		auto it = filters.find( filter_ );
		if ( it == filters.end() )
			return false;
		filter_.reset();
		auto & [ filter, delegate ] = *it;
		delegate -= { listener, /** @todo forward? */fn };
		if ( filter.use_count() == 1 )
			filters.erase( it );
		return true;
	}

	constexpr void notify_with_filters( auto & filters, const auto & message ) noexcept {
		auto it = filters.begin();
		while ( it != filters.end() ) {
			auto & [ filter, delegate ] = *it;
			if ( filter.use_count() == 1 ) {
				it = filters.erase( it );
				continue;
			}
			if ( (*filter)( message ) ) {
				if constexpr ( ::std::is_same_v<decltype(delegate), message_delegate_type> )
					delegate( &message );
				else
					delegate( message );
			}
			if ( not filter->is_once() )
				++it;
			else
				it = filters.erase( it );
		}
	}

	message_filter_ptr default_message_filter{ MkSPtr<message_filter_type>() };

	using message_filters_list
		= ::std::map< message_filter_ptr, message_delegate_type, ::cpp::ptr_less<message_filter_ptr> >;
	using multi_filters_list
		= ::std::map< multi_filter_ptr, multi_delegate_type, ::cpp::ptr_less<multi_filter_ptr> >;
	::std::map< message_id_type, message_filters_list > sync_message_filters;
	multi_filters_list sync_multi_filters;
};

} // namespace lib::bus

#endif // CPPLIB__lib__bus__manager__hpp
