/* File: /lib/file/ini.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__file__ini__hpp
#define CPPLIB__lib__file__ini__hpp

#include <functional>
#include <tuple>
#include <string>
#include <vector>
#include <map>

#include "../../lib/tl/noncopyable.hpp"
#include "../../lib/cstring.hpp"
#include "../../lib/data/stream.hpp"

namespace lib::file {

// DECLARATION lib::file::Ini

class Ini final
	: tl::noncopymovable< Ini >
{
public:
	using ForeachFn = ::std::function<
		bool( const cstring &/*section*/, const cstring &/*key*/, const cstring &/*value*/ )
		>;
	using KeyValue = ::std::pair< cstring, cstring >;
	using Section = ::std::vector< KeyValue >;
	struct SectionOpLess {
		constexpr bool operator()( const cstring & lhv, const cstring & rhv ) const noexcept
			{ return lhv.data() < rhv.data(); }
	};
	using Sections = ::std::map< cstring, Section, SectionOpLess >;

	Ini() = default;
	Ini( data::rstream_t & stream );

	bool read( data::rstream_t & stream );
	void reset();
	bool foreach( ForeachFn foreach_fn ) const;

	const Sections & sections() const { return sections_; }

private:

	void parseContent();

	::std::string content;
	Sections sections_;
};

} // namespace lib::file

#endif // CPPLIB__lib__file__ini__hpp
