/*
 * Tokeniser.cpp
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

#include "Tokeniser.hpp"
#include <sstream>
#include <cassert>

using namespace MJSON;

using e_char = Enums::e_char;
using e_token = Enums::e_token;


TokenList Tokeniser::get_token_list(std::string & json_src)
{
	assert(json_src.size() > 0 && "Error, src length <  1");

	TokenList tokens;
	tokens.init(json_src.size() / 2);

	char_idx_t idx = 0;

	while(idx < json_src.size() - 1)
	{
		Character current_character = get_next_character(json_src, idx);
		e_char character_type = current_character.m_type;
		idx = current_character.m_pos;


		switch(character_type)
		{
		case e_char::BRACE_OPEN:
			tokens.add_token(e_token::OBJECT_START);
			break;
		case e_char::BRACE_CLOSE:
			tokens.add_token(e_token::OBJECT_END);
			break;
		case e_char::BRACKET_OPEN:
			tokens.add_token(e_token::ARRAY_START);
			break;
		case e_char::BRACKET_CLOSE:
			tokens.add_token(e_token::ARRAY_END); //TODO: Check if end of file (comma)
			break;
		case e_char::WHITESPACE:
		case e_char::NEWLINE:
		case e_char::COMMA:
			break;

		case e_char::QUOTE:
			{
				StringPos str = get_string(json_src, idx);
				idx = str.m_pos;
				Character next_character = get_next_character(json_src,idx);
				if(next_character.m_type == e_char::COLON) //must be a key if previous was a string
				{
					tokens.add_token(e_token::KEY, str.m_string);
					idx = next_character.m_pos;
				}
				else if(next_character.m_type == e_char::COMMA || next_character.m_type == e_char::BRACE_CLOSE || next_character.m_type == e_char::BRACKET_CLOSE) //must be a string value as ", would indicate value, not key
				{
					tokens.add_token(e_token::STRING, str.m_string);
				}
				break;
			}
		case e_char::LETTER:
			{
				idx --;
				std::string next_four_chars = get_next_n_chars_as_string(json_src, 4, idx);
				if(next_four_chars == "true")
				{
					tokens.add_token(e_token::BOOL, next_four_chars);
					idx += 4;
				}
				else if(next_four_chars == "fals")
				{
					tokens.add_token(e_token::BOOL, get_next_n_chars_as_string(json_src, 5, idx));
					idx += 5;
				}
				else if(next_four_chars == "null")
				{
					tokens.add_token(e_token::NULL_VALUE, next_four_chars);
					idx += 4;
				}
				else
				{
					assert(false && "Character found at invalid position.  Not key, string, bool or null\n");
				}
				break;
			}
		case e_char::NUMBER:
			{
				//std::cout << "Number value\n";
				idx --;
				StringPos num_as_str = get_number_string(json_src, idx);
				assert(num_as_str.m_string.substr(0, 1) != " " && "Leading whitespace, should have already been removed/ignored\n");

				std::string clean_num_str = num_as_str.m_string;

				if(clean_num_str.find(Enums::e_char_to_string(e_char::WHITESPACE)) != std::string::npos) //remove following whitespace
				{
					clean_num_str = get_substr(clean_num_str, e_char::WHITESPACE, 0).m_string;
				}
				if(clean_num_str.find(Enums::e_char_to_string(e_char::NEWLINE)) != std::string::npos)
				{
					clean_num_str = get_substr(clean_num_str, e_char::NEWLINE, 0).m_string;
				}


				std::string end_char = get_string_end_character(clean_num_str);

				if(end_char == Enums::e_char_to_string(e_char::BRACKET_CLOSE) || end_char == Enums::e_char_to_string(e_char::BRACE_CLOSE))
				{
					clean_num_str = get_substr(clean_num_str, e_char::WHITESPACE, 0).m_string;
					tokens.add_token(e_token::NUMBER, clean_num_str);
					idx += clean_num_str.length();
				}
				else
				{
					tokens.add_token(e_token::NUMBER, clean_num_str);
					idx += clean_num_str.length();
				}
			}
		}

	}
	tokens.clear_unused();
	return tokens;
}


//Return char idx will be of the character following the aquired string
Tokeniser::StringPos Tokeniser::get_string(std::string & json, char_idx_t current_char_idx)
{
	//std::cout << "get_string\n";
	return get_substr(json, e_char::QUOTE,	current_char_idx);

}

Tokeniser::StringPos Tokeniser::get_number_string(std::string & json, char_idx_t current_char_idx)
{
	//std::cout << "get_number_string\n";
	return get_substr(json, e_char::COMMA, current_char_idx);
}

Tokeniser::StringPos Tokeniser::get_substr(std::string& json, e_char delimiter, char_idx_t current_char_idx)
{
	//std::cout << "get_subtr\n";
	std::string current_char = _get_char_at_idx(json, current_char_idx);

	int str_len = 0;
	do {
		str_len += 1;
		current_char = json.substr(current_char_idx + str_len, 1);

	}
	while( current_char_idx + str_len < json.size() - 1 && current_char != Enums::e_char_to_string(delimiter) );


	std::string string = json.substr(current_char_idx, str_len);
	current_char_idx += str_len + 1;

	return StringPos { string, current_char_idx };
}


std::string Tokeniser::_get_char_at_idx(std::string& json, char_idx_t idx)
{
	return json.substr(idx, 1);
}

std::string Tokeniser::get_next_n_chars_as_string(std::string & json, int num_chars, char_idx_t idx)
{
	return json.substr(idx, num_chars);
}


Tokeniser::Character Tokeniser::get_next_character(std::string & json, char_idx_t current_char_idx)
{
	//std::string current_char = _get_char_at_idx(json, current_char_idx);
	std::string current_char;
	do {
			current_char = _get_char_at_idx(json, current_char_idx);
			current_char_idx ++;
	}
	while(current_char == Enums::e_char_to_string(e_char::WHITESPACE) || current_char == Enums::e_char_to_string(e_char::NEWLINE));

	if(c_LETTERS.find(current_char) != std::string::npos)
	{
		return Character { e_char::LETTER, current_char_idx};
	}
	else if(c_NUMBERS.find(current_char) != std::string::npos)
	{
		return Character{ e_char::NUMBER, current_char_idx};
	}
	else
	{
		assert(Enums::e_char_exists(current_char) && "Invalid character found\n");
		return Character { Enums::string_to_e_char(current_char), current_char_idx};
	}

}

std::string Tokeniser::get_string_end_character(std::string & str)
{
	if(str.length() < 1)
		return "";
	if (str.find("\n") != std::string::npos)
	{
		return str.substr(str.length() -2, 1);
	}
	else
	{
		return str.substr(str.length() - 1,	1);
	}
}
