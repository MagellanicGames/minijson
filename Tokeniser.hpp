/*
 * Tokeniser.hpp
 * Converts the raw JSON src/string/characters to meaningful tokens than can then be processed from
 * JSON into C++ objects.
 *
 *  Created on: 5 Dec 2022
 ****************************************************************************************************
 *LICENSE: zlib/libpng
 *
 *Copyright (c) 2022 Liam Charalambous (@magellanicgames)
 *
 *This software is provided "as-is", without any express or implied warranty. In no event
 *will the authors be held liable for any damages arising from the use of this software.
 *
 *Permission is granted to anyone to use this software for any purpose, including commercial
 *applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *	1. The origin of this software must not be misrepresented; you must not claim that you
 *	wrote the original software. If you use this software in a product, an acknowledgment
 *	in the product documentation would be appreciated but is not required.
 *
 *	2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *  as being the original software.
 *
 *  3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************************************
 */

#pragma once
#include "MJSONEnums.hpp"
#include "TokenList.hpp"
#include <string>
#include <unordered_map>

namespace MJSON
{
	using char_idx_t = std::string::size_type;

	class Tokeniser
	{

	public:

		struct StringPos
		{
			std::string m_string;
			char_idx_t m_pos;
		};

		struct Character
		{
			Enums::e_char m_type;
			char_idx_t m_pos;
		};

		const std::string c_LETTERS = "abcdefghijklmnopqrstuvwxyzABCEDFGHIJKLMNOPQRSTUVWXYZ";
		const std::string c_NUMBERS = "-0123456789";

		Tokeniser() = default;

		TokenList get_token_list(std::string & json_src);

		StringPos get_string(std::string & json, char_idx_t current_char_idx);
		StringPos get_substr(std::string & json, Enums::e_char delimiter, char_idx_t current_char_idx );
		StringPos get_number_string(std::string & json, char_idx_t current_char_idx);
		std::string get_string_end_character(std::string &);

		Character get_next_character(std::string & json, char_idx_t current_char_idx);
		std::string get_next_n_chars_as_string(std::string & json, int num_chars, char_idx_t current_char_idx);

	private:
		std::string _get_char_at_idx(std::string& json, char_idx_t);
	};
}
