

/*
 * TokenList.cpp
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
#include "TokenList.hpp"

#include <iostream>
#include <cassert>
#include "Token.hpp"

using namespace MJSON;

void TokenList::init(int size)
{
	m_tokens.reserve(size);

	for(auto idx = 0; idx < m_tokens.capacity(); idx ++)
	{
		m_tokens.push_back(std::make_unique<Token>());
	}
}
void TokenList::add_token(Enums::e_token type, std::string value)
{
	assert(m_tokens.capacity() > 0 && "Storage not initialised.  Must call TokenList::init(int size) before adding tokens\n");
	m_tokens[m_current_idx]->m_type = type;
	if(value != "")
		m_tokens[m_current_idx]->m_value = value;
	//std::cout << "TokenList::add_token: " << value << "\n";
	m_current_idx++;
}

void TokenList::clear_unused()
{
	if(m_current_idx < m_tokens.size() - 1)
	{
		m_tokens.resize(m_current_idx);
	}

}

void TokenList::print_tokens() const
{
	for(auto& token : m_tokens)
	{
		std::cout << token->to_string() << "\n";
	}
}
