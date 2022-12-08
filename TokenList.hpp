/*
 * TokenList.hpp
 * Holds the tokenised JSON, which can then be processed by the parser.
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
#include <vector>
#include <string>
#include <memory>

#include "MJSONEnums.hpp"
#include "Token.hpp"

namespace MJSON
{
	class TokenList
	{
	public:
		void init(int size);
		void add_token(Enums::e_token type, std::string value = "");
		void clear_unused();

		void print_tokens() const;
		int size()const {return m_tokens.size();}

		Token* operator[](const int & idx)
		{
			return m_tokens[idx].get();
		}
	private:
		std::vector<std::unique_ptr<Token>> m_tokens;
		int m_current_idx = 0;
	};
}

