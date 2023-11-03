/* File: /lib/cli/interface.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__cli__interface__hpp
#define CPPLIB__lib__cli__interface__hpp

#include <functional>
#include <variant>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/tl/linkable.hpp"
#include "../../lib/cstring.hpp"

#include "./param.hpp"

namespace lib::cli {

// PREDECLARATIONS

struct Command;
class IDriver;

// DECLARATION lib::cli::detail::callback_short, lib::cli::detail::callback_full

namespace detail {
using callback_short	= ::std::function<void()>;
using callback_full		= ::std::function<void( const Command & )>;
} // namespace detail

// DECLARATION lib::cli::Interface

class Interface final
	: tl::noncopymovable<Interface>
	, public tl::linkable<Interface, IDriver>
{
public:
	using Callback = ::std::variant
		< ::std::monostate, detail::callback_short, detail::callback_full >;

	Interface( IDriver * driver );

	void out( const cstring & line ) const;
	void out_commands() const;
	bool autocomplete( ::std::string & commandline ) const;
	void configure_split( const cstring & separators, const cstring & group );
	const ::std::list<::std::string> & history() const { return history_; }

	Interface & operator << ( const cstring & command );
	Interface & operator << ( const char * command );
	inline Interface & operator << ( const decltype(param) & );
	inline Interface & operator << ( const decltype(params_list) & );
	Interface & operator << ( const Callback & callback );

	bool operator () ( const cstring & commandline );
private:
	struct Lexeme final
		: tl::noncopymovable<Lexeme>
	{
		Lexeme * append_named( const cstring & name_ );
		Lexeme * append_param();
		Lexeme * append_params_list();
		void set_callback( const Callback & callback_ );

		const Lexeme * next( const cstring & name_, Command * command = nullptr ) const;
		bool invoke( Command & command ) const;

		const ::std::string & get_name() const { return name; }
		::std::vector<::std::string> list_commands() const;
		::std::vector<cstring> list_suggested( const cstring & name_ ) const;
	private:
		static constexpr const char * PARAM_CHILD_KEY = "";

		bool has_callback() const;

		enum class Type {
			ROOT, NAMED, PARAM, PARAMS_LIST
		} type = Type::ROOT;

		const Lexeme * parent = nullptr;
		::std::map< ::std::string, Lexeme > childs;
		Callback callback;

		::std::string name;
	};

	void update_history( const ::std::vector<cstring> & lexemes );

	Lexeme root;
	Lexeme * tmp_lexeme;

	::std::string split_separators = "\t ";
	::std::string split_group = "";

	/// @todo Manage capacity.
	::std::list<::std::string> history_;
};

// INLINES lib::cli::Interface

inline Interface & Interface::operator << ( const char * command ) {
	return operator << ( cstring( command ) );
}

inline Interface & Interface::operator << ( const decltype(param) & ) {
	tmp_lexeme = tmp_lexeme->append_param();
	return *this;
}

inline Interface & Interface::operator << ( const decltype(params_list) & ) {
	tmp_lexeme = tmp_lexeme->append_params_list();
	return *this;
}

} // namespace lib::cli

#endif // CPPLIB__lib__cli__interface__hpp
