/*
 * Parser.cpp
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

#include "Parser.hpp"
#include "Variant.hpp"

#include <iostream>
#include <cassert>

using namespace MJSON;

using e_token = Enums::e_token;

bool Parser::m_initialised = false;
std::unordered_map<Enums::e_token, Variant::type> Parser::m_e_token_to_variant_type;

void Parser::init_conversions()
{
	if(m_initialised)
		return;
	m_initialised = true;

	//enum class type {null_t, int_t, float_t,bool_t, string_t, container_t, vector_t, map_t, end};
	m_e_token_to_variant_type[e_token::BOOL] = Variant::type::bool_t;
	m_e_token_to_variant_type[e_token::STRING] = Variant::type::string_t;
	m_e_token_to_variant_type[e_token::ARRAY_START] = Variant::type::vector_t;
	m_e_token_to_variant_type[e_token::OBJECT_START] = Variant::type::map_t;
}

std::shared_ptr<ContainerVariant> Parser::parse_tokens(TokenList& token_list)
{
	std::stack<ContainerVariant*> stack;
	ContainerVariant* root_container = nullptr;
	m_last_token_type = e_token::NOTHING;

	for(int current_idx = 0; current_idx < token_list.size(); current_idx++)
	{
		Token* current_token = token_list[current_idx];

		//std::cout << "Parser::parse_tokens: " << current_token->to_string() << "\n";
		switch(current_token->m_type)
		{
			case e_token::OBJECT_START:
			{
				assert(m_last_token_type != e_token::OBJECT_START &&
						"Parser::parse_tokens: Invalid token sequence, object start can only follow a colon or array start token if it is not root.\n");

				if(m_last_token_type == e_token::NOTHING)
				{
					assert(root_container == nullptr && "Root container already set, something's gone wrong\n");
					stack.push(new MapVariant);
					root_container = stack.top();
					m_last_token_type = current_token->m_type;
				}
				else if(m_last_token_type == e_token::ARRAY_START || m_last_token_type == e_token::OBJECT_END)
				{
					_object_start_nested(stack, "none", current_token);
				}

				break;
			}
			case e_token::OBJECT_END:
			{
				_object_end(stack);
				break;
			}
			case e_token::ARRAY_START:
				{
					if(m_last_token_type == e_token::NOTHING)
					{
						assert(root_container == nullptr && "Root container already set, something's gone wrong\n");
						stack.push(new VectorVariant);
						root_container = stack.top();
						m_last_token_type = current_token->m_type;
					}
					else
					{
						_array_start_nested(stack, "none", current_token);
					}

					break;
				}
			case e_token::ARRAY_END:
				{
					_array_end(stack);
					break;
				}
			case e_token::KEY:
			{
				assert(m_last_token_type != e_token::KEY &&
						"Parser::parse_tokens: Invalid token sequence, object key can't follow another key, object end or array end\n");

				std::string& key_string = current_token->m_value;

				TokenExceptionList_t token_exceptions = { e_token::KEY , e_token::OBJECT_END, e_token::ARRAY_END };
				Token* next_token = _get_next_token(current_idx, token_list, token_exceptions, 3);
				//std::cout << "Parser::parse_tokens:Key: " << current_token->to_string() << " value: " << next_token->to_string() << "\n";

				if(next_token->m_type == e_token::ARRAY_START)
				{
					_array_start_nested(stack, key_string, next_token);

				}
				else if(next_token->m_type == e_token::OBJECT_START)
				{
					_object_start_nested(stack, key_string, next_token);
				}
				else
				{
					_add_key_value_pair_to_container(stack.top(), key_string, next_token);
				}

				current_idx ++; //additional increment as processing 2 tokens (the key and it's paired value)

				break;
			}
			case e_token::NUMBER:
			case e_token::NULL_VALUE:
			case e_token::BOOL:
			case e_token::STRING:
				{
					assert(root_container != nullptr && "Root object container not set. Object or Array must be root.\n");
					assert(stack.size() > 0 && "Type can only be added here if within an array/vector, stack is empty.\n");
					assert(!_is_token_invalid(m_last_token_type, {e_token::OBJECT_START, e_token::KEY}, 2) && "Invalid token sequence.\n");
					assert(stack.top()->m_type == Variant::type::vector_t && "Type can only be added here if within an array/vector\n");

					_add_key_value_pair_to_container(stack.top(), "no_key", current_token);
					break;
				}
			case e_token::COMMA:
			case e_token::COLON:
			case e_token::NOTHING:
				{
					assert(false && "Invalid token found, should have been removed by tokeniser.\n");
				}
		}
	}
	assert(stack.size() == 0 && "Stack should be empty, a container must not have ended (OBJECT_END or ARRAY_END)\n");
	return std::shared_ptr<ContainerVariant>(root_container);
}

void Parser::_add_key_value_pair_to_container(ContainerVariant* container, std::string key, Token* value)
{
	Variant::type var_t = Variant::type::null_t;
	var_t = _e_token_to_variant_type(value->m_type, value->m_value);
	container->add_variant(var_t, value->m_value, key);
	m_last_token_type = value->m_type;
}

void Parser::_array_start_nested(std::stack<ContainerVariant*>& stack,std::string map_key, Token* current_token)
{
	ContainerVariant* current_container = stack.top();
	_add_key_value_pair_to_container(current_container, map_key, current_token);
	Variant* vector_variant = current_container->get_last_added_variant();
	assert(vector_variant->m_type == Variant::type::vector_t);
	stack.push(dynamic_cast<ContainerVariant*>(vector_variant));
	m_last_token_type = current_token->m_type;
}

void Parser::_object_start_nested(std::stack<ContainerVariant*>& stack,std::string map_key, Token* current_token)
{
	ContainerVariant* current_container = stack.top();
	_add_key_value_pair_to_container(current_container, map_key, current_token); //create and add new VariantMap
	Variant* map_variant = current_container->get_last_added_variant(); //retrieve new VariantMap to put on stack
	assert(map_variant->m_type == Variant::type::map_t && "Invalid type, should be a vector_t\n");
	stack.push(dynamic_cast<ContainerVariant*>(map_variant));
	m_last_token_type = current_token->m_type;
}

void Parser::_array_end(std::stack<ContainerVariant*>& stack)
{
	if(_is_token_invalid(m_last_token_type, {e_token::OBJECT_START, e_token::KEY}, 2))
		assert(false && "Invalid token sequence.\n");
	ContainerVariant* current_array = stack.top();
	assert(current_array->m_type == Variant::type::vector_t && "Invalid token sequence, expected array contents before end of array.\n");
	stack.pop();
	m_last_token_type = e_token::ARRAY_END;
}

void Parser::_object_end(std::stack<ContainerVariant*>& stack)
{
	if(_is_token_invalid(m_last_token_type, {e_token::OBJECT_START, e_token::KEY, e_token::ARRAY_START}, 3))
						assert(false && "Invalid token sequence.\n");
	ContainerVariant* current_map = stack.top();
	assert(current_map->m_type == Variant::type::map_t && "Invalid token sequence, expected array contents before end of object.\n");
	stack.pop();
	m_last_token_type = e_token::OBJECT_END;
}
Variant::type Parser::_e_token_to_variant_type(e_token token, std::string value_str)
{
	Parser::init_conversions();
	if(Parser::m_e_token_to_variant_type.find(token) != Parser::m_e_token_to_variant_type.end())
	{
		return Parser::m_e_token_to_variant_type[token];
	}
	else if(token == e_token::NUMBER)
	{
		assert(value_str != "" && "To convert token enum to variant enum, requires the numbers value to deduce float or int\n");
		if(value_str.find(".") != std::string::npos)
			return Variant::type::float_t;
		else
			return Variant::type::int_t;
	}
	else
	{
		return Variant::type::null_t;
	}
}

Token* Parser::_get_next_token(int current_idx, TokenList& token_list, TokenExceptionList_t token_exception_list, int exception_list_size)
{
	Token* next_token = token_list[current_idx + 1];
	bool error_type = false;

	for(int idx = 0; idx < exception_list_size; idx ++)
	{
		if(next_token->m_type == token_exception_list[idx])
		{
			error_type = true;
			break;
		}
	}
	assert(!error_type && "Token not expected, something's gone wrong\n");
	return next_token;
}

bool Parser::_is_token_valid(Enums::e_token token_type, TokenFilter_list_t valid_tokens, int filter_list_size)
{
	bool result = false;
	for(int idx =0; idx < filter_list_size; idx++)
	{
		if(token_type == valid_tokens[idx])
		{
			result = true;
			break;
		}

	}
	return result;
}

bool Parser::_is_token_invalid(Enums::e_token token_type, TokenFilter_list_t invalid_tokens, int filter_list_size)
{
	bool result = false;
	for(int idx =0; idx < filter_list_size; idx++)
	{
		if(token_type == invalid_tokens[idx])
		{
			result = true;
			break;
		}

	}
	return result;
}



