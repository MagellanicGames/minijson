/*
 * MJSON.hpp
 * Provides the main interface for converting JSON files.  Can load from disk or a hard coded string.
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


#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

#include "Variant.hpp"
#include "Token.hpp"
#include "TokenList.hpp"

//typedef std::string::size_type char_idx_t;


namespace MJSON
{
	class JSON
	{
	public:
		JSON()
		{
			Enums::init_conversions();
		}

		void load_src(std::string path); //loads json src file into m_json_src member
		void load_src_from_string(std::string json_src); //sets json_src member to a string of json src

		std::shared_ptr<ContainerVariant> get_parsed_json() {return m_parsed_json;}

	private:
		TokenList _convert_src_to_tokens();
		void _parse_tokens(TokenList&);

		std::string m_json_src;
		std::shared_ptr<ContainerVariant> m_parsed_json = nullptr;
	};
}

