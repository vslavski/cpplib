/* File: /lib/tl/carray.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__carray__hpp
#define CPPLIB__lib__tl__carray__hpp

#include <type_traits>
#include <initializer_list>
#include <tuple>
#include <new>
#include <utility>

#include "../../lib/tl/noncopyable.hpp"

namespace lib::tl {

/// @todo Add boundaries check for debug?

template<typename CountT, ::std::size_t Capacity, class...DataT >
class carray final
	/// @todo Remove noncopyable/nonmovable?
//	: noncopymovable< carray<CountT,Capacity,DataT...> >
{
public:
	using value_type		= ::std::tuple<DataT...>;
	using reference			= ::std::tuple<DataT&...>;
	using const_reference	= ::std::tuple<const DataT&...>;
	using size_type			= ::std::size_t;
	using pointer			= ::std::tuple<DataT*...>;
	using const_pointer		= ::std::tuple<const DataT*...>;

	constexpr carray( CountT & count_, DataT(&...data_)[Capacity] ) noexcept
		: count_{ count_ }, data_{ data_... } {}

	/// @name Capacity
	/// @{
	constexpr size_type size() const noexcept { return count_; }
	constexpr static size_type max_size() noexcept { return capacity(); }
	constexpr static size_type capacity() noexcept { return Capacity; }
	constexpr bool empty() const noexcept { return count_ == 0; }
	/// @}

	/// @name Element access
	/// @{
	constexpr reference operator[]( size_type n ) noexcept { return at( n ); }
	constexpr const_reference operator[]( size_type n ) const noexcept { return at( n ); }
	constexpr const_reference at( size_type n ) const noexcept { return at_impl( n ); };
	constexpr reference at( size_type n ) noexcept { return at_impl( n ); }
	/// @}

	/// @name Assignment
	/// @{
	/// @todo Is constexpr required here?
	constexpr carray & operator = ( ::std::initializer_list<value_type> list ) noexcept {
		/// @todo If C++ allows to us in future:
		//static_assert( list.size() <= max_size() );
		if ( not empty() )
			clear();
		for ( auto && value : list )
			emplace_back( ::std::move( value ) );
		return *this;
	}
	/// @}

	/// @todo return reference
	constexpr reference emplace_back( auto && value ) noexcept
		{ return at_impl( count_++ ) = value; }
	/// different logic (ctor/creation) :(
	constexpr reference emplace_back( DataT&&...values ) noexcept
		{ return at_impl( count_++ ) = ::std::forward_as_tuple( values... ); }

	constexpr void clear() noexcept { clear_impl(); count_ = 0; }


private:
	using Data = ::std::tuple< DataT(&)[Capacity]... >;

	template< ::std::size_t TupleElementIdx = 0 >
	constexpr void emplace_back_impl( auto && tupleOfData ) noexcept {
		auto & dataArray = ::std::get<TupleElementIdx>( data_ );
		auto && dataValue = ::std::get<TupleElementIdx>( tupleOfData );
		using DataType = ::std::remove_reference_t<decltype(*dataArray)>;
		new ( &dataArray[count_] ) DataType ( ::std::move( dataValue ) );
		if constexpr ( TupleElementIdx + 1 < ::std::tuple_size_v<Data> )
			emplace_back_impl< TupleElementIdx + 1 >( tupleOfData );
	}

	template< ::std::size_t TupleElementIdx = 0 >
	constexpr void clear_impl() noexcept {
		auto & dataArray = ::std::get<TupleElementIdx>( data_ );
		using DataType = ::std::remove_reference_t<decltype(*dataArray)>;
		for ( ::std::size_t i = 0; i < count_; ++i )
			dataArray[count_].~DataType();
		if constexpr ( TupleElementIdx + 1 < ::std::tuple_size_v<Data> )
			clear_impl< TupleElementIdx + 1 >();
	}

	template< ::std::size_t TupleElementIdx = 0 >
	constexpr auto at_impl( size_type n ) noexcept {
		auto & dataArray = ::std::get<TupleElementIdx>( data_ );
		auto element = ::std::tie( dataArray[n] );
		if constexpr ( TupleElementIdx + 1 == ::std::tuple_size_v<Data> )
			return element;
		else
			return ::std::tuple_cat( element, at_impl< TupleElementIdx + 1 >( n ) );
	}

	CountT & count_;
	Data data_;
};

} // namespace lib::tl

#endif // CPPLIB__lib__tl__carray__hpp
