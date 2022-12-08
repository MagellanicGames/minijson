/*
 * MJSONEnums.hpp
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
#include <unordered_map>
#include <vector>
#include <string>

namespace MJSON
{
	class Enums
	{
	public:
		enum class e_char {
			BRACE_OPEN,//{
			BRACE_CLOSE,//}
			COLON, //:
			COMMA, //,
			QUOTE, //"
			BRACKET_OPEN, //[
			BRACKET_CLOSE,//]
			LETTER,
			NUMBER,
			PERIOD,//.
			NEWLINE,
			WHITESPACE
		};

		enum class e_token
		{
			NOTHING,
			OBJECT_START, //{
			OBJECT_END, //}
			COMMA, //,
			KEY, // string type
			COLON, // :
			NUMBER, // int, float or double
			BOOL, //true or false
			STRING, // string value without quotes
			ARRAY_START, //[
			ARRAY_END, //]
			NULL_VALUE // null
		};

		static bool m_initialised;

		static void init_conversions();

		static std::string e_char_to_string(e_char);
		static e_char string_to_e_char(std::string&);
		static bool e_char_exists(std::string&);

		static std::string e_token_to_string(e_token);

	private:

		static std::unordered_map<e_char, std::string> m_e_character_to_string;
		static std::unordered_map<std::string, e_char> m_string_to_character_e;

		static std::unordered_map<e_token, std::string> m_e_token_to_string;
	};
}

