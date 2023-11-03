/* File: /lib/utils/voynich.hpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */
#ifndef CPPLIB__lib__utils__voynich__hpp
#define CPPLIB__lib__utils__voynich__hpp

#include <cstdlib>

#include <array>
#include <string>

#include "../../lib/types.hpp"
#include "../../lib/cstring.hpp"

// DECLARATION lib::utils::voynich

namespace lib::utils {

inline constexpr cstring VOYNICH[] =
	{ "\u{FF400}", "\u{FF401}", "\u{FF402}", "\u{FF403}", "\u{FF404}", "\u{FF405}", "\u{FF406}", "\u{FF407}"
	, "\u{FF408}", "\u{FF409}", "\u{FF40A}", "\u{FF40B}", "\u{FF40C}", "\u{FF40D}", "\u{FF40E}", "\u{FF40F}"
	, "\u{FF410}", "\u{FF411}", "\u{FF412}", "\u{FF413}", "\u{FF414}", "\u{FF415}", "\u{FF416}", "\u{FF417}"
	, "\u{FF418}", "\u{FF419}", "\u{FF41A}", "\u{FF41B}", "\u{FF41C}", "\u{FF41D}", "\u{FF41E}", "\u{FF41F}"
	, "\u{FF420}", "\u{FF421}", "\u{FF422}", "\u{FF423}", "\u{FF424}", "\u{FF425}", "\u{FF426}", "\u{FF427}"
	, "\u{FF428}", "\u{FF429}", "\u{FF42A}", "\u{FF42B}", "\u{FF42C}", "\u{FF42D}", "\u{FF42E}", "\u{FF42F}"
	, "\u{FF430}", "\u{FF431}", "\u{FF432}", "\u{FF433}", "\u{FF434}", "\u{FF435}", "\u{FF436}", "\u{FF437}"
	, "\u{FF43C}", "\u{FF43D}", "\u{FF43E}", "\u{FF43F}"
	, "\u{FF440}", "\u{FF441}", "\u{FF442}", "\u{FF443}", "\u{FF444}", "\u{FF445}", "\u{FF446}", "\u{FF447}"
	, "\u{FF448}", "\u{FF449}", "\u{FF44A}", "\u{FF44B}", "\u{FF44C}", "\u{FF44D}", "\u{FF44E}", "\u{FF44F}"
	, "\u{FF450}", "\u{FF451}", "\u{FF452}", "\u{FF453}", "\u{FF454}", "\u{FF455}", "\u{FF456}", "\u{FF457}"
	, "\u{FF458}", "\u{FF459}", "\u{FF45A}", "\u{FF45B}", "\u{FF45C}", "\u{FF45D}", "\u{FF45E}", "\u{FF45F}"
	, "\u{FF460}", "\u{FF461}", "\u{FF462}", "\u{FF463}", "\u{FF464}", "\u{FF465}", "\u{FF466}", "\u{FF467}"
	, "\u{FF468}", "\u{FF469}", "\u{FF46A}", "\u{FF46B}", "\u{FF46C}", "\u{FF46D}", "\u{FF46E}", "\u{FF46F}"
	, "\u{FF470}", "\u{FF471}", "\u{FF472}", "\u{FF473}", "\u{FF474}", "\u{FF475}", "\u{FF476}", "\u{FF477}"
	, "\u{FF478}", "\u{FF479}", "\u{FF47A}", "\u{FF47B}", "\u{FF47C}", "\u{FF47D}", "\u{FF47E}", "\u{FF47F}"
	, "\u{FF480}", "\u{FF481}", "\u{FF482}", "\u{FF483}", "\u{FF484}", "\u{FF485}", "\u{FF486}", "\u{FF487}"
	, "\u{FF488}", "\u{FF489}", "\u{FF48A}", "\u{FF48B}", "\u{FF48C}", "\u{FF48D}", "\u{FF48E}", "\u{FF48F}"
	, "\u{FF490}", "\u{FF491}", "\u{FF492}", "\u{FF493}", "\u{FF494}", "\u{FF495}", "\u{FF496}", "\u{FF497}"
	, "\u{FF498}", "\u{FF499}", "\u{FF49A}", "\u{FF49B}", "\u{FF49C}", "\u{FF49D}", "\u{FF49E}", "\u{FF49F}"
	, "\u{FF4A0}", "\u{FF4A1}", "\u{FF4A2}", "\u{FF4A3}", "\u{FF4A4}", "\u{FF4A5}", "\u{FF4A6}", "\u{FF4A7}"
	, "\u{FF4A8}", "\u{FF4A9}", "\u{FF4AA}", "\u{FF4AB}", "\u{FF4AC}", "\u{FF4AD}", "\u{FF4AE}", "\u{FF4AF}"
	, "\u{FF4B0}", "\u{FF4B1}", "\u{FF4B2}", "\u{FF4B3}", "\u{FF4B4}", "\u{FF4B5}", "\u{FF4B6}", "\u{FF4B7}"
	, "\u{FF4B8}", "\u{FF4B9}", "\u{FF4BA}", "\u{FF4BB}", "\u{FF4BC}", "\u{FF4BD}", "\u{FF4BE}", "\u{FF4BF}"
	, "\u{FF4C0}", "\u{FF4C1}", "\u{FF4C2}", "\u{FF4C3}", "\u{FF4C4}", "\u{FF4C5}", "\u{FF4C6}", "\u{FF4C7}"
	, "\u{FF4C8}", "\u{FF4C9}", "\u{FF4CA}", "\u{FF4CB}", "\u{FF4CC}", "\u{FF4CD}", "\u{FF4CE}", "\u{FF4CF}"
	, "\u{FF4D0}", "\u{FF4D1}", "\u{FF4D2}", "\u{FF4D3}", "\u{FF4D4}", "\u{FF4D5}", "\u{FF4D6}", "\u{FF4D7}"
	, "\u{FF4D8}", "\u{FF4D9}", "\u{FF4DA}", "\u{FF4DB}", "\u{FF4DC}", "\u{FF4DD}", "\u{FF4DE}", "\u{FF4DF}"
	, "\u{FF4E0}", "\u{FF4E1}", "\u{FF4E2}", "\u{FF4E3}", "\u{FF4E4}", "\u{FF4E5}", "\u{FF4E6}", "\u{FF4E7}"
	, "\u{FF4E8}", "\u{FF4E9}", "\u{FF4EA}", "\u{FF4EB}", "\u{FF4EC}", "\u{FF4ED}", "\u{FF4EE}", "\u{FF4EF}"
	, "\u{FF4F0}", "\u{FF4F1}", "\u{FF4F2}", "\u{FF4F3}", "\u{FF4F4}", "\u{FF4F5}", "\u{FF4F6}", "\u{FF4F7}"
	, "\u{FF4F8}", "\u{FF4F9}", "\u{FF4FA}", "\u{FF4FB}", "\u{FF4FC}", "\u{FF4FD}", "\u{FF4FE}", "\u{FF4FF}"
	, "\u{FF500}", "\u{FF501}", "\u{FF502}", "\u{FF503}", "\u{FF504}", "\u{FF505}", "\u{FF506}", "\u{FF507}"
	, "\u{FF508}", "\u{FF509}", "\u{FF50A}", "\u{FF50B}", "\u{FF50C}", "\u{FF50D}", "\u{FF50E}", "\u{FF50F}"
	, "\u{FF510}", "\u{FF511}", "\u{FF512}", "\u{FF513}", "\u{FF514}", "\u{FF515}", "\u{FF516}", "\u{FF517}"
	, "\u{FF518}", "\u{FF519}", "\u{FF51A}", "\u{FF51B}", "\u{FF51C}", "\u{FF51D}", "\u{FF51E}", "\u{FF51F}"
	};

inline /*constexpr */::std::string voynich( usize length ) noexcept {
	::std::string result;
	result.reserve( length * VOYNICH[0].size() );
	while ( length-- > 0 ) {
		const auto index = (usize) ::std::rand() % ::std::size( VOYNICH );
		result.append( VOYNICH[ index ].begin(), VOYNICH[ index ].end() );
	}
	return result;
}

} // namespace lib::utils

#endif // CPPLIB__lib__utils__voynich__hpp
