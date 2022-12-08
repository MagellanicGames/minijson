/*
 * Parser.hpp
 * Converts the tokenised JSON into either an unordered map or vector of Variant type objects.
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

#include "TokenList.hpp"
#include "Variant.hpp"
#include <array>
#include <stack>

namespace MJSON
{
	class ContainerVariant;
	class Variant;
	class Parser
	{
	public:

		std::shared_ptr<ContainerVariant> parse_tokens(TokenList&);

	private:

		void _add_key_value_pair_to_container(ContainerVariant* container, std::string key, Token* value);
		void _array_start_nested(std::stack<ContainerVariant*>& stack, std::string map_key, Token* current_token);
		void _object_start_nested(std::stack<ContainerVariant*>& stack, std::string map_key, Token* current_token);
		void _array_end(std::stack<ContainerVariant*>& stack);
		void _object_end(std::stack<ContainerVariant*>& stack);

		Variant::type _e_token_to_variant_type(Enums::e_token, std::string value_str = "");

		static constexpr int c_TOKEN_EXCEPTION_LIST_SIZE_MAX = 10;
		using TokenExceptionList_t = std::array<Enums::e_token, c_TOKEN_EXCEPTION_LIST_SIZE_MAX>;
		using TokenFilter_list_t = TokenExceptionList_t;

		Token* _get_next_token(int current_idx, TokenList&, TokenExceptionList_t token_exception_list, int exception_list_size);
		bool _is_token_valid(Enums::e_token token_type, TokenFilter_list_t valid_tokens, int filter_list_size);
		bool _is_token_invalid(Enums::e_token token_type, TokenFilter_list_t invalid_tokens, int filter_list_size);



		Enums::e_token m_last_token_type = Enums::e_token::NOTHING;
		static bool m_initialised;
		static void init_conversions();
		static std::unordered_map<Enums::e_token, Variant::type> m_e_token_to_variant_type;
	};
}
