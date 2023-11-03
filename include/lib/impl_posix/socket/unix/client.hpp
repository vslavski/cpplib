/* File: /lib/impl_posix/socket/unix/client.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__impl_posix__socket__unix__client__hpp
#define CPPLIB__lib__impl_posix__socket__unix__client__hpp

/// @todo Unify with "tcp"?

#include "../../../../lib/cstring.hpp"
#include "../../../../lib/socket/client.hpp"

#include "./base.hpp"

namespace lib::socket::impl::unix {

// DECLARATION lib::socket::impl::unix::client

class client
	: public base
	, public ::lib::socket::client
{
	using Super = base;
public:
	client() = default;
	client( const cstring & filename );
	virtual ~client();

	cstring getName() const override;

	bool connect( const cstring & filename );

	bool update() override;
	bool shutdown() override;
	bool close() override;
private:
	data::result_t check_error( isize result ) override;
	bool close_lock = false;
};

} // namespace lib::socket::impl::unix

#endif // CPPLIB__lib__impl_posix__socket__unix__client__hpp
