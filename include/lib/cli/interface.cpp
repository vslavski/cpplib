/* File: /lib/cli/interface.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <cpp/lib_debug>

#include "../../lib/types.hpp"
#include "../../lib/literals.hpp"
#include "../../lib/utils/string.hpp"

#include "./driver.hpp"
#include "./command.hpp"

#include "./interface.hpp"

namespace lib::cli {

// IMPLEMENTATION lib::cli::Interface

Interface::Interface( IDriver * driver )
	: tmp_lexeme{ &root }
{
	if ( driver != nullptr )
		CPP_UNUSED( link( *driver ) );
}

void Interface::out( const cstring & line ) const {
	auto * driver = get_link<IDriver>();
	if ( driver != nullptr ) driver->out( line );
}

void Interface::out_commands() const {
	for ( const auto & command : root.list_commands() )
		out( command );
}

bool Interface::autocomplete( ::std::string & commandline ) const {
	const auto * lexeme = &root;
	const auto & names = split( commandline, split_separators.c_str(), split_group.c_str() );
	if ( names.empty() ) {
		out_commands();
		return false;
	}
	for ( auto i = 0_sz; i < names.size(); ++i ) {
		const auto * next_lexeme = lexeme->next( names[i] );
		if ( next_lexeme == nullptr ) {
			if ( i == names.size() - 1 )
				break;
			else
				return false;
		}
		lexeme = next_lexeme;
	}
	const bool suggest_next =
		commandline.find_last_of( split_separators.c_str() ) == commandline.size() - 1;
	if ( not suggest_next and names.back() == lexeme->get_name() ) {
		commandline += " ";
		return true;
	}
	const auto & incomplete_name = suggest_next ? cstring() : names.back();
	const auto & suggested_names = lexeme->list_suggested( incomplete_name );
	if ( suggested_names.empty() )
		return false;
	if ( suggested_names.size() > 1 ) {
		out( "" );
		if ( suggest_next )
			for ( const auto & command : lexeme->list_commands() )
				out( commandline + command.substr( names.back().size() + 1/*space*/ ) );
		else
			for ( const auto & name : suggested_names )
				out( commandline + str( name.substr( incomplete_name.size() ) ) );
	}
	bool commandline_updated = false;
	const auto & first_suggested = suggested_names.front();
	for ( usize i = incomplete_name.size(); i < first_suggested.size(); ++i ) {
		const auto append_char = first_suggested[i];
		bool can_append = true;
		for ( auto j = 1_sz; j < suggested_names.size(); ++j ) {
			if ( append_char != suggested_names[j][i] ) {
				can_append = false;
				break;
			}
		}
		if ( not can_append )
			break;
		commandline += append_char;
		commandline_updated = true;
	}
	if ( not suggest_next and suggested_names.size() == 1 ) {
		commandline += " ";
		return true;
	}
	return commandline_updated;
}

void Interface::configure_split( const cstring & separators, const cstring & group ) {
	split_separators = str( separators );
	split_group = str( group );
}

Interface & Interface::operator << ( const cstring & command ) {
	for ( const auto & name : split( command, split_separators.c_str(), split_group.c_str() ) )
		tmp_lexeme = tmp_lexeme->append_named( name );
	return *this;
}

Interface & Interface::operator << ( const Callback & callback ) {
	tmp_lexeme->set_callback( callback );
	tmp_lexeme = &root;
	return *this;
}

bool Interface::operator () ( const cstring & commandline ) {
	const auto & lexemes = split( commandline, split_separators.c_str(), split_group.c_str() );
	if ( lexemes.empty() )
		return true;
	const auto * lexeme = &root;
	Command command( *this );
	update_history( lexemes );
	for ( const auto & name : lexemes ) {
		lexeme = lexeme->next( name, &command );
		if ( lexeme == nullptr )
			return false;
	}
	return lexeme->invoke( command );
}

void Interface::update_history( const ::std::vector<cstring> & lexemes ) {
	::std::string entry;
	for ( const auto & name : lexemes ) {
		if ( not entry.empty() )
			entry += " ";
		entry += name;
	}
	const auto it = ::std::find( history_.begin(), history_.end(), entry );
	if ( it == history_.end() )
		history_.emplace_front( entry );
	else if ( it != history_.begin() )
		history_.splice( history_.begin(), history_, it, ::std::next( it ) );
}

// INLINES lib::cli::Interface::Lexeme

Interface::Lexeme * Interface::Lexeme::append_named( const cstring & name_ ) {
	CPP_ASSERT( not name_.empty() );
	CPP_ASSERT( type != Type::PARAMS_LIST );
	auto & lexeme = childs[ str( name_ ) ];
	lexeme.type = Type::NAMED;
	lexeme.parent = this;
	lexeme.name = name_;
	return &lexeme;
}

Interface::Lexeme * Interface::Lexeme::append_param() {
	CPP_ASSERT( type != Type::PARAMS_LIST );
	auto & lexeme = childs[ PARAM_CHILD_KEY ];
	lexeme.type = Type::PARAM;
	lexeme.parent = this;
	return &lexeme;
}

Interface::Lexeme * Interface::Lexeme::append_params_list() {
	CPP_ASSERT( type != Type::PARAMS_LIST );
	auto & lexeme = childs[ PARAM_CHILD_KEY ];
	lexeme.type = Type::PARAMS_LIST;
	lexeme.parent = this;
	return &lexeme;
}

void Interface::Lexeme::set_callback( const Callback & callback_ ) {
	CPP_ASSERT( type != Type::ROOT );
	CPP_ASSERT( not ::std::holds_alternative<::std::monostate>( callback_ ) );
	callback = callback_;
}

const Interface::Lexeme * Interface::Lexeme::next
	( const cstring & name_, Command * command /*= nullptr*/ ) const
{
	const Lexeme * lexeme = this;
	if ( type != Type::PARAMS_LIST ) {
		auto it = childs.find( str( name_ ) );
		if ( it != childs.end() )
			return &it->second;
		it = childs.find( PARAM_CHILD_KEY );
		if ( it == childs.end() )
			return nullptr;
		lexeme = &it->second;
	}
	if ( command != nullptr ) {
		if ( is_integer( name_ ) )
			command->params.emplace_back( ::std::stoi( str( name_ ) ) );
		else if ( is_float( name_ ) )
			command->params.emplace_back( ::std::stof( str( name_ ) ) );
		else
			command->params.push_back( str( name_ ) );
	}
	return lexeme;
}

bool Interface::Lexeme::invoke( Command & command ) const {
	if ( not has_callback() ) {
		const auto it = childs.find( PARAM_CHILD_KEY );
		if ( it != childs.end() and it->second.type == Type::PARAMS_LIST )
			return it->second.invoke( command );
		return false;
	}
	if ( ::std::holds_alternative<detail::callback_full>( callback ) )
		::std::invoke( ::std::get<detail::callback_full>( callback ), command );
	else
		::std::invoke( ::std::get<detail::callback_short>( callback ) );
	return true;
}

::std::vector<::std::string> Interface::Lexeme::list_commands() const {
	::std::string lexeme_name;
	switch ( type ) {
	case Type::NAMED:		lexeme_name = name + " "; break;
	case Type::PARAM:		lexeme_name = "<param> "; break;
	case Type::PARAMS_LIST:	lexeme_name = "<params...> "; break;
	default: break;
	}
	::std::vector<::std::string> result;
	for ( const auto & child : childs ) {
		const auto & commands = child.second.list_commands();
		result.insert( result.end(), commands.begin(), commands.end() );
	}
	if ( type != Type::ROOT ) {
		for ( auto & commandline : result )
			commandline.insert( 0, lexeme_name );
		if ( has_callback() )
			result.insert( result.begin(), lexeme_name );
	}
	return result;
}

::std::vector<cstring> Interface::Lexeme::list_suggested( const cstring & name_ ) const {
	::std::vector<cstring> result;
	for ( const auto & child : childs )
		if ( child.first.find( name_ ) == 0 )
			result.emplace_back( child.first );
	return result;
}

bool Interface::Lexeme::has_callback() const {
	return not ::std::holds_alternative<::std::monostate>( callback );
}

} // namespace lib::cli
