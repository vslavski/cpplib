/* File: /test/lib/cli.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <lib/debug/platform.hpp>

#ifdef CPPLIB_PLATFORM_POSIX
	#include <lib/impl_posix/cli/termios_driver.hpp>
#else
	#include <lib/impl/cli/cstdio_driver.hpp>
#endif // CPPLIB_PLATFORM_POSIX

#include <lib/literals.hpp>

#include <lib/cli.hpp>

#include "./cli.hpp"

namespace test::lib {

void Cli::test_execute() noexcept/* override*/ {
	using namespace ::lib;

#if defined( CPPLIB__lib__impl_posix__cli__termios_driver__hpp )
	cli::impl::TermiosDriver cli_driver;
#elif defined( CPPLIB__lib__impl__cli__cstdio_driver__hpp )
	cli::impl::CstdioDriver cli_driver;
#endif // lib::cli::impl...Driver
	cli_driver.prompt( "> " );
	cli::Interface cmd( &cli_driver );

	cmd.configure_split( "\t ", "`" );

	cmd
		<< "test"
	<< [&cmd]() {
		cmd.out( "This is test command." );
	};

	i32 foo = 0;
	cmd
		<< "set foo" << cli::param
	<< [&foo]( const cli::Command & command ) {
		auto & cmd = command.interface;
		if ( command.params.size() != 1 )
			cmd.out( "Set foo: Bad numbers of arguments" );
		else
			foo = command.params[0];
	}
		<< "get foo"
	<< [&]() {
		cmd.out( str( foo ) );
	};

	f32 bar = 313.37f;
	cmd
		<< "bar =" << cli::param
	<< [&bar]( const cli::Command & command ) {
		bar = command.params[0];
	}
		<< "bar"
	<< [&]() {
		cmd.out( str( bar ) );
	}

		<< "print params" << cli::params_list
	<< []( const cli::Command & command ) {
		auto & cmd = command.interface;
		cmd.out( "Print params: (count " + str( command.params.size() ) + "):" );
		auto index = 0_sz;
		for ( const auto & param : command.params )
			cmd.out( str( ++index ) + ": " + param.s() );
	};

	cmd
		<< "set" << cli::param
	<< []( const cli::Command & command ) {
		auto & cmd = command.interface;
		cmd.out( "\"set <param>\" called with param \"" + command.params[0].s() + "\"" );
	}
		<< "set" << cli::param << "to" << cli::param
	<< []( const cli::Command & command ) {
		auto & cmd = command.interface;
		cmd.out( "\"set <param> to <param>\" called with params \""
			+ command.params[0].s() + "\" and \""
			+ command.params[1].s() + "\"" );
	};

	bool exit = false;
	cmd.out( "\nType help or exit:" );
	cmd
		<< "exit"
	<< [&exit]() {
		exit = true;
	}
		<< "help"
	<< [&cmd]() {
		cmd.out_commands();
	};
	while ( not exit and cli_driver.update() );
}

} // namespace test::lib
