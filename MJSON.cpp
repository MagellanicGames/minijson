/*
 * MJSON.cpp
 *
 *  Created on: 3 Dec 2022
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
#include "MJSON.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#include "Tokeniser.hpp"
#include "Parser.hpp"


using namespace MJSON;

using e_char = Enums::e_char;
using e_token = Enums::e_token;

void JSON::load_src(std::string load_path)
{
	using namespace std;
	ifstream f(load_path);
	ostringstream ss;
	if(f)
	{
		ss << f.rdbuf();
	}
	m_json_src = ss.str();
	TokenList token_list = _convert_src_to_tokens();
	_parse_tokens(token_list);
}

void JSON::load_src_from_string(std::string json_src)
{
	m_json_src = json_src;
	TokenList token_list = _convert_src_to_tokens();
	_parse_tokens(token_list);
}

TokenList JSON::_convert_src_to_tokens()
{
	Tokeniser t;
	return t.get_token_list(m_json_src);
}

void JSON::_parse_tokens(TokenList& token_list)
{
	Parser p;
	m_parsed_json = p.parse_tokens(token_list);
}
