/*
 * MJSONEnums.cpp
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

#include "MJSONEnums.hpp"

using namespace MJSON;

bool Enums::m_initialised = false;
std::unordered_map<Enums::e_char, std::string> Enums::m_e_character_to_string;
std::unordered_map<std::string, Enums::e_char> Enums::m_string_to_character_e;
std::unordered_map<Enums::e_token, std::string> Enums::m_e_token_to_string;

void Enums::init_conversions()
{
	if(m_initialised)
		return;
	m_initialised = true;
	m_e_character_to_string[e_char::BRACE_OPEN] = "{";
	m_e_character_to_string[e_char::BRACE_CLOSE] = "}";
	m_e_character_to_string[e_char::COLON] = ":";
	m_e_character_to_string[e_char::COMMA] = ",";
	m_e_character_to_string[e_char::QUOTE] = "\"";
	m_e_character_to_string[e_char::BRACKET_OPEN] = "[";
	m_e_character_to_string[e_char::BRACKET_CLOSE] = "]";
	m_e_character_to_string[e_char::PERIOD] = ".";
	m_e_character_to_string[e_char::NEWLINE] = "\n";
	m_e_character_to_string[e_char::WHITESPACE] = " ";


	for(auto& pair : m_e_character_to_string)
	{
		m_string_to_character_e[pair.second] = pair.first;
	}

	m_e_token_to_string[e_token::NOTHING] = "NOTHING";
	m_e_token_to_string[e_token::OBJECT_START] = "OBJECT_START";
	m_e_token_to_string[e_token::OBJECT_END] = "OBJECT_END";
	m_e_token_to_string[e_token::COMMA] = "COMMA";
	m_e_token_to_string[e_token::KEY] = "KEY";
	m_e_token_to_string[e_token::COLON] = "COLON";
	m_e_token_to_string[e_token::NUMBER] = "NUMBER";
	m_e_token_to_string[e_token::BOOL] = "BOOL";
	m_e_token_to_string[e_token::STRING] = "STRING";
	m_e_token_to_string[e_token::ARRAY_START] = "ARRAY_START";
	m_e_token_to_string[e_token::ARRAY_END] = "ARRAY_END";
	m_e_token_to_string[e_token::NULL_VALUE] = "NULL_VALUE";

}

std::string Enums::e_char_to_string(e_char c)
{
	return m_e_character_to_string[c];
}

Enums::e_char Enums::string_to_e_char(std::string& s)
{
	return m_string_to_character_e[s];
}

bool Enums::e_char_exists(std::string& char_string)
{
	return m_string_to_character_e.find(char_string) != m_string_to_character_e.end();
}

std::string Enums::e_token_to_string(e_token token)
{
	return m_e_token_to_string[token];
}
