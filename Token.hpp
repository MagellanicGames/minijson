/*
 * Token.hpp
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
#include <string>

#include "MJSONEnums.hpp"

#pragma once

namespace MJSON
{
	struct Token
	{
		~Token(){};
		Enums::e_token m_type;
		std::string m_value;

		void operator=(const Token& t)
		{
			m_type = t.m_type;
			m_value = t.m_value;
		}

		std::string to_string()
		{
			return "Token{" + Enums::e_token_to_string(m_type) + "," + m_value + "}";
		}


	};
}

