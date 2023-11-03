/* File: /test/lib/impl/hash/sha1.cpp
 *
 * This file is a part of cpplib project which is distributed under MIT License.
 * See file LICENSE for full license details.
 *
 * Copyright (c) 2020-present Nikita Zuev (V.Slavski!) <nikita.zuev@gmx.com>
 */

#include <string>

#include <cpp/lib_debug>

#include <lib/types.hpp>
#include <lib/cstring.hpp>

#include <lib/utils/string.hpp>

#include <lib/impl/hash/sha1.hpp>

#include "./sha1.hpp"

namespace test::lib::hash::impl {

void Sha1::test_execute() noexcept/* override*/ {
	static constexpr ::lib::usize COUNT = 70;
	static constexpr ::lib::cstring A_TIMES_HASH[COUNT] =
		{ "da39a3ee5e6b4b0d3255bfef95601890afd80709" // ''
		, "6dcd4ce23d88e2ee9568ba546c007c63d9131c1b" // 'A'
		, "801c34269f74ed383fc97de33604b8a905adb635" // 'AA'
		, "606ec6e9bd8a8ff2ad14e5fade3f264471e82251" // 'AAA'
		, "e2512172abf8cc9f67fdd49eb6cacf2df71bbad3" // ...
		, "c1fe3a7b487f66a6ac8c7e4794bc55c31b0ef403"
		, "2d2929e0f1bca99d9652924ce73b7969d33ff429"
		, "f9b2869de6cc9226b990d83f805ec83915cc9c85"
		, "c08598945e566e4e53cf3654c922fa98003bf2f9"
		, "1cbbd7d768f77d4d3f24de43238979aa9fa1cd2f"
		, "c71613a7386fd67995708464bf0223c0d78225c4"
		, "004537f3b1fd67347489185a1c4b55da58f6edca"
		, "2b52d47ab698ccce79ab6d0552e98f87f8a3aebc"
		, "91dd8a106d38bd458250b80314a3b4837acfa85b"
		, "9108c1fc03ff53527f9d9de94d9c151e697e154d"
		, "343ad63c4d45b81d945360c080b065c98c7a8351"
		, "19b1928d58a2030d08023f3d7054516dbc186f20"
		, "9ee276acbf8a1257a58a5bad22bef8907e49cbf2"
		, "3a8262b7c3b43877389d300986b0c0b1eedfdfbf"
		, "1a6372d15d776f9879d300e51ec145363cd63667"
		, "ebd3d4adf97066c84b8ed17d6bd1e270818763e0"
		, "29ad0c6384182c5c2d4c953e200eed245467e503"
		, "d088f3b187a0957d72b5d5645939bfc4302dffb8"
		, "293efde746444af8e7aff0ad1a57c874cdc50966"
		, "4f130f23896bd6d0e95f2a42b2cb83d17ac8f1a2"
		, "a92b995e293d295c4bbab7043cccb030bef47488"
		, "ed641f05795d5ee712d1e6ddc2d5146079db9dee"
		, "82e757683db0b0417976c1661f7b020ae5225b80"
		, "7b92fac2f01809101168d085e9f1ef059b131be4"
		, "41be845b8e19da10e18a6bd3105793484d22bd53"
		, "2a22d32e957a9de69c50e8f52872e2dbf1d0745c"
		, "ca75e66a01a2b5b24f825f569d5ddeead3e50e4d"
		, "43d83b2e816a89cac876f16530b0b625585c8160"
		, "e04976c6e1ce44aa1840b07b57021c158a11eafc"
		, "609b3f4ee88fd429c53d51dca7ace87711e7d48f"
		, "4c911f83e9b42c92b8ea62135fa1bc0e727ce367"
		, "3c8a34351337e8f5376092d3f329767c8035344d"
		, "0b314daa55be9ff60f4337a25fef266036aed20c"
		, "35309ec13ef8d90aaae172e4cf437eb16ddbf6d5"
		, "6784f01a2b317aeef2ac03660dafa3270f4d420e"
		, "5cdbb64242d8551a7cf583903fd7d5b72b277537"
		, "0e477417eecfe482fd137e4a038fb5cf6dc7be76"
		, "880b405e8e5059e3aa1797f662ff4a0cfcbce20b"
		, "885dd07854409bf8cf5443652fd6835c23423338"
		, "a7da128970268478e46f9585d0fb6297349b9675"
		, "06bf9b84f2cffdb4b343ef9b3ddd1847f9b6ce3c"
		, "4683b63a087f88e7ada2f6e3eceb4a0e9f7195a1"
		, "b459efc276e7c1e39f997ed6c9b4f692dafd30b5"
		, "8b2177f39b224cab2fb4df5ee4827fbe7115ce44"
		, "d52bcfb557dd3ed70968f8835ccff3c924885631"
		, "080316afb4e11d98120b29d1070ce749f1f0a32c"
		, "4456f6c537924b7d47e430050d92bf6949a1fba8"
		, "defc08198e86f88a007ca10f10d8af0d402ffdc3"
		, "55066b480654e5846549494b863e3cd34bae76eb"
		, "18b837ae2f9a204a7fea6d6a2ae5174365137861"
		, "5021b3d42aa093bffc34eedd7a1455f3624bc552"
		, "6b45e3cf1eb3324b9fd4df3b83d89c4c2c4ca896"
		, "e8d6ea5c627fc8676fa662677b028640844dc35c"
		, "e0ed6b6f61dae4219379cf9fe19565150c8e6046"
		, "ba83959b9f4a8b3ca082d501e7b75ce73992e35f"
		, "c9c4571630054c5466d19b5ea28069dc71c72b68"
		, "fc202c022fdc439b99892020e04fc93b4ee8448a"
		, "0dc94299f2d293a48173f9c78a882f8a9bffe3b0"
		, "0ec86b3f3ac34ad860fa8da56bcca03a54018049"
		, "30b86e44e6001403827a62c58b08893e77cf121f"
		, "826b7e7a7af8a529ae1c7443c23bf185c0ad440c"
		, "eddee92010936db2c45d2c9f5fdd2726fcd28789"
		, "d0c9def032806d32bc485ea5493e34217d5091c9"
		, "01ae707f5f6574b061a4643f59c98277da6544a3"
		, "4b4e4f859b006e5b0afe2dc2806bae2ab3cb55b0"
		};

	::lib::hash::impl::sha1 sha1;
	::std::string plain;
	for ( ::lib::usize i = 0; i < COUNT; ++i, plain += 'A' ) {
		(void) sha1.update( plain );
		CPPLIB__TEST__LOOP_NEXT();
		CPPLIB__TEST__EQ( ::lib::hex( sha1.hash() ), A_TIMES_HASH[i] );
	}
}

} // namespace test::lib::hash::impl
