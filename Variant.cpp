/*
 * Variant.cpp
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

#include "Variant.hpp"

using namespace MJSON;


void VectorVariant::add_variant(Variant::type var_type, std::string& value_str, std::string key)
{
	switch(var_type)
	{
	case Variant::type::null_t:
		m_container.push_back(std::make_unique<Null>());
		break;
	case Variant::type::string_t:
		m_container.push_back(std::make_unique<StringV>(value_str));
		break;
	case Variant::type::bool_t:
		m_container.push_back(std::make_unique<Bool>(value_str));
		break;
	case Variant::type::float_t:
		m_container.push_back(std::make_unique<Float>(value_str));
		break;
	case Variant::type::int_t:
		m_container.push_back(std::make_unique<Int>(value_str));
		break;
	case Variant::type::vector_t:
		m_container.push_back(std::make_unique<VectorVariant>());
		break;
	case Variant::type::map_t:
		m_container.push_back(std::make_unique<MapVariant>());
		break;
	}
}

Variant* VectorVariant::get_last_added_variant()
{
	Variant* result = nullptr;

	if(m_container.size() > 0)
	{
		result = m_container[m_container.size() - 1].get();
	}

	return result;
}

void MapVariant::add_variant(Variant::type var_type, std::string& value_str, std::string key)
{
	switch(var_type)
	{
	case Variant::type::null_t:
		m_container[key] = std::make_unique<Null>();
		break;
	case Variant::type::string_t:
		m_container[key] = std::make_unique<StringV>(value_str);
		break;
	case Variant::type::bool_t:
		m_container[key] = std::make_unique<Bool>(value_str);
		break;
	case Variant::type::float_t:
		m_container[key] = std::make_unique<Float>(value_str);
		break;
	case Variant::type::int_t:
		m_container[key] = std::make_unique<Int>(value_str);
		break;
	case Variant::type::vector_t:
		m_container[key] = std::make_unique<VectorVariant>();
		break;
	case Variant::type::map_t:
		m_container[key] = std::make_unique<MapVariant>();
		break;
	}
	m_last_added_key = key;
}

Variant* MapVariant::get_last_added_variant()
{
	Variant* result = nullptr;
	if(m_container.size() > 0)
	{
		result = m_container[m_last_added_key].get();
	}
	return result;
}

void MapVariant::print_keys() const
{
	std::cout << size() << " keys present in map.\n";
	int i = 0;
	for(auto& pair : m_container)
	{
		std::cout << i << ")" << pair.first << "\n";
		i++;
	}
}
