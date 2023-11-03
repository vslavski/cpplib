/* File: /lib/tl/listener.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__tl__listener__hpp
#define CPPLIB__lib__tl__listener__hpp

#include <bit>
#include <functional>
#include <memory>
#include <type_traits>
#include <variant>

#include <cpp/lib_debug>
#include <cpp/lib_type_traits>

#include "../../lib/ptr.hpp"
#include "../../lib/ref.hpp"

// DECLARATION lib::tag_tl_listener<>

namespace lib {

namespace detail { struct tag_tl_listener_ {}; }

template<class T> struct tag_tl_listener : detail::tag_tl_listener_ {};

} // namespace lib

namespace lib::tl {

// DECLARATION lib::tl::listener<>

template< class Result, class...Args >
class listener final {
public:
	using Listener = ::lib::detail::tag_tl_listener_;
	using Method = Result(Listener::*)(Args...);
	using Instance = ::std::variant< ::std::monostate, Ref<Listener>, WPtr<Listener> >;

	constexpr listener() noexcept = default;
	constexpr ~listener() noexcept = default;
	constexpr listener( const listener & ) noexcept = default;
	constexpr listener( listener && ) noexcept = default;
	constexpr listener & operator = ( const listener & ) noexcept = default;
	constexpr listener & operator = ( listener && ) noexcept = default;

	constexpr listener( auto & instance_, auto method_ ) noexcept;
	template< class T >
	constexpr listener( SPtr<T> instance_, auto method_ ) noexcept;

	constexpr bool operator == ( const listener & other ) const noexcept;

	constexpr operator bool () const noexcept;

	constexpr Result operator () ( Args...args ) const noexcept;

	constexpr void reset() noexcept;

	template< class T, class U, class R, class...A >
	constexpr void reset( T & instance_, R(U::* method_)(A...) ) noexcept;
	template< class T, class U, class R, class...A >
	constexpr void reset( SPtr<T> instance_, R(U::* method_)(A...) ) noexcept;
private:
	template< class From, class To >
	static constexpr bool is_instance_ok
	= ::std::is_base_of_v< tag_tl_listener<To>, To >
	and (
		::std::is_same_v<From, To>
		or ::std::is_base_of_v<To, From>
	);

	template< class From, class To >
	static constexpr bool is_result_and_args_ok
	= ::std::is_same_v< From, To >
	or (
		not ::std::is_void_v<To>
		and ::std::is_void_v< ::std::remove_pointer_t<To> >
		and ::std::is_pointer_v<From>
		and ::cpp::has_same_cv_v<From, To>
		/// @todo Use ::cpp::is_pointer_interconvertible_base_of<> ?
	);

	template< class Type, class U, class R, class...A >
	constexpr void reset_method( R(U::* method_)(A...) ) noexcept;
	constexpr void reset_clear() const noexcept;

	mutable Instance instance;
	Method method;
};

// INLINES lib::tl::listener<>

template< class Result, class...Args >
inline constexpr listener<Result,Args...>::listener( auto & instance_, auto method_ ) noexcept
	{ reset( instance_, method_ ); }

template< class Result, class...Args >
template< class T >
inline constexpr listener<Result,Args...>::listener( SPtr<T> instance_, auto method_ ) noexcept
	{ reset( instance_, method_ ); }

template< class Result, class...Args >
inline constexpr bool listener<Result,Args...>::operator == ( const listener & other ) const noexcept {
	if ( method != other.method ) return false;
	if ( not *this ) return not other;
	if ( not other ) return not *this;
	if ( instance.index() != other.instance.index() ) return false;
	if ( const auto * listener_ref = ::std::get_if< Ref<Listener> >( &instance ) )
		return &listener_ref->get()
			== &::std::get_if< Ref<Listener> >( &other.instance )->get();
	if ( const auto * listener_wptr = ::std::get_if< WPtr<Listener> >( &instance ) )
		return listener_wptr->lock().get()
			== ::std::get_if< WPtr<Listener> >( &other.instance )->lock().get();
	CPP_ASSERT( false/*Variant holds unknown alternative*/ );
	return false;
}

template< class Result, class...Args >
inline constexpr listener<Result,Args...>::operator bool () const noexcept
	{ return not ::std::holds_alternative<::std::monostate>( instance ); }

template< class Result, class...Args >
inline constexpr Result listener<Result,Args...>::operator () ( Args...args ) const noexcept {
	if ( ::std::holds_alternative<::std::monostate>( instance ) )
		return Result();
	if ( const auto * listener_ref = ::std::get_if< Ref<Listener> >( &instance ) )
		return ::std::invoke( method, *listener_ref, ::std::forward<Args>( args )... );
	if ( const auto * listener_wptr = ::std::get_if< WPtr<Listener> >( &instance ) ) {
		if ( auto * listener = listener_wptr->lock().get() )
			return ::std::invoke( method, listener, ::std::forward<Args>( args )... );
		else
			reset_clear();
	}
	return Result();
}

template< class Result, class...Args >
inline constexpr void listener<Result,Args...>::reset() noexcept
	{ reset_clear(); }

template< class Result, class...Args >
template< class T, class U, class R, class...A >
inline constexpr void listener<Result,Args...>::reset( T & instance_, R(U::* method_)(A...) ) noexcept {
	static_assert( is_instance_ok<T, U>
		, "U is not a base class of T, or U is not inherit lib::tag_tl_listener<U>" );
	using Type = ::std::conditional_t< ::std::is_base_of_v<U, T>, U, T >;
	if constexpr ( ::std::is_base_of_v<::std::enable_shared_from_this<T>, T> )
		instance = ::std::static_pointer_cast< tag_tl_listener<Type> >
			( instance_.::std::enable_shared_from_this<T>::shared_from_this() );
	else
		instance = static_cast< tag_tl_listener<Type>& >( instance_ );

	reset_method<Type>( method_ );
}

template< class Result, class...Args >
template< class T, class U, class R, class...A >
inline constexpr void listener<Result,Args...>::reset( SPtr<T> instance_, R(U::* method_)(A...) ) noexcept {
	static_assert( is_instance_ok<T, U>
		, "U is not a base class of T, or U is not inherit lib::tag_tl_listener<U>" );
	using Type = ::std::conditional_t< ::std::is_base_of_v<U, T>, U, T >;
	CPP_ASSERT( instance_ );
	instance = ::std::static_pointer_cast< tag_tl_listener<Type> >( instance_ );

	reset_method<Type>( method_ );
}

template< class Result, class...Args >
template< class Type, class U, class R, class...A >
inline constexpr void listener<Result,Args...>::reset_method( R(U::* method_)(A...) ) noexcept {
	static_assert( is_result_and_args_ok<R, Result>
		, "Bad result type conversion: R to Result" );
	if constexpr ( sizeof...(Args) > 0 )
		static_assert( ::cpp::conjunction_v< is_result_and_args_ok<A, Args>... >
			, "Bad argument type conversion: A... to Args..." );
	static_assert( sizeof(Result(Type::*)(Args...)) == sizeof(decltype(method_))
		, "std::bit_cast (arguments cast) looses method's metainfo" );
	static_assert( sizeof(Result(tag_tl_listener<Type>::*)(Args...)) == sizeof(Result(Type::*)(Args...))
		, "static_cast to 'tag_tl_listener<>' looses method's metainfo" );
	static_assert( sizeof(Method) == sizeof(Result(tag_tl_listener<Type>::*)(Args...))
		, "static_cast to 'Method' looses method's metainfo" );
	const auto & tmp_method = ::std::bit_cast< Result(Type::*)(Args...) >( method_ );
	method = static_cast< Method >(
		static_cast< Result(tag_tl_listener<Type>::*)(Args...) >(
		tmp_method ) );
}

template< class Result, class...Args >
inline constexpr void listener<Result,Args...>::reset_clear() const noexcept
	{ instance = {}; }

} // namespace lib::tl

#endif // CPPLIB__lib__tl__listener__hpp
