/*
 * JSONTestParser.cpp
 *
 *  Created on: 7 Dec 2022
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
#include "JSONTestParser.hpp"
#include "MJSON.hpp"
#include <iostream>
#include <cassert>

using namespace MJSON;

//The following string is JSON used to develop, test and debug the tokeniser and parser.
//This will be extended further as it doesn't cover all cases, but covers enough for what
//is currently needed for my purposes.
//This parser only supports having an array or object as the root element.
//This test is essentially a unit test...without any unit tests. If the source code
//is modified then rerunning this will help identifying if anything was broken or
//any bugs present.
std::string test_json_src = R"(
{
    "hello" : "world" ,
    "t": true ,
    "f": false,
    "n": null,
    "i": 123 ,
    "negative" : -30,
    "negative_float" : -21.245,
    "pi": 3.1416,
    "a": [1, 2, 3, 4],
    "array0" : [ 1, [2,4], 4],
    "transform" : { "m_position" : [0,0] , "m_scale" : [1,1] , "another_string" : "my string"} ,
    "obj_n" : { "another_num" : 24235.2544},
    "object_array" : [{"name" : "object0"}, {"name" : "object1"}],
    "null_array" : [null, null, null, null],
    "mixed_array" : [true, 1, 3.4, false, -3, -6.5, "a string"]
}
)";

void JSONTestParser::test_parser()//Function to be used like a unit test
{
	JSON j;
	j.load_src_from_string(test_json_src);
	auto parsed_json = j.get_parsed_json();

	constexpr int expected_map_size = 15;
	std::cout << "\nJSONTestParser::test_parser: Validating JSON test file...\n";
	assert(parsed_json != nullptr && "Parsed JSON is null. Have you loaded and parsed the source?\n");
	assert(parsed_json->m_type == Variant::type::map_t && "Expected a map (JSON object) as root.\n");

	MapVariant& map_var = *dynamic_cast<MapVariant*>(parsed_json.get());

	map_var.print_keys();
	assert(map_var.size() == expected_map_size && "Root object does not have 14 keys/entries\n");

	{
		std::array<std::string, expected_map_size> expected_keys =
		{
				"hello", "t", "f", "n", "i", "negative", "negative_float", "pi", "a", "array0", "transform", "obj_n", "object_array", "null_array", "mixed_array"
		};

		for(auto& key : expected_keys)
		{
			assert(map_var.has(key) && "Could not find key.\n");
		}
	}


	std::cout << "All Expected keys found, testing data values are correct...\n";
	assert(map_var["hello"]->m_string == "world");
	assert(map_var["t"]->m_bool == true);
	assert(map_var["f"]->m_bool == false);
	assert(map_var["n"]->m_type == Variant::type::null_t );
	assert(map_var["i"]->m_signed_int == 123  );
	assert(map_var["negative"]->m_signed_int == -30);
	assert(map_var["negative_float"]->m_float < -21 );
	assert(map_var["pi"]->m_float > 3.0);

	{
		assert(map_var["a"]->m_type == Variant::type::vector_t );
		VectorVariant& a = map_var.get_ref_to_value<VectorVariant>("a");
		assert(a[0]->m_signed_int == 1);
		assert(a[1]->m_signed_int == 2);
		assert(a[2]->m_signed_int == 3);
		assert(a[3]->m_signed_int == 4);
	}


	{
		assert(map_var["array0"]->m_type == Variant::type::vector_t);
		VectorVariant& array0 = map_var.get_ref_to_value<VectorVariant>("array0");
		assert(array0.size() == 3);
		assert(array0[0]->m_signed_int == 1);

		assert(array0[1]->m_type == Variant::type::vector_t);
		VectorVariant& element1 = array0.get_ref_to_value<VectorVariant>(1);
		assert(element1[0]->m_signed_int == 2);
		assert(element1[1]->m_signed_int == 4);

		assert(array0[2]->m_signed_int == 4);
	}

	{
		assert(map_var["transform"]->m_type == Variant::type::map_t);
		MapVariant& transform = map_var.get_ref_to_value<MapVariant>("transform");

		assert(transform.size() == 3);
		std::array<std::string, 3> expected_keys = {"m_position", "m_scale", "another_string"};
		for(auto& key : expected_keys)
		{
			assert( transform.has(key) && "Could not find key.\n");
		}

		assert(transform["m_position"]->m_type == Variant::type::vector_t);
		VectorVariant& m_position = transform.get_ref_to_value<VectorVariant>("m_position");
		assert(m_position[0]->m_signed_int == 0);
		assert(m_position[1]->m_signed_int == 0);

		assert(transform["m_scale"]->m_type == Variant::type::vector_t);
		VectorVariant& m_scale = transform.get_ref_to_value<VectorVariant>("m_scale");
		assert(m_scale[0]->m_signed_int == 1);
		assert(m_scale[1]->m_signed_int == 1);

		assert(transform["another_string"]->m_type == Variant::type::string_t);
		assert(transform["another_string"]->m_string == "my string");

	}

	{
		assert(map_var["obj_n"]->m_type == Variant::type::map_t);
		MapVariant& obj_n = map_var.get_ref_to_value<MapVariant>("obj_n");
		assert(obj_n["another_num"]->m_type == Variant::type::float_t);
	}
	{
		assert(map_var["object_array"]->m_type == Variant::type::vector_t);
		VectorVariant& object_array = map_var.get_ref_to_value<VectorVariant>("object_array");
		assert(object_array.size() == 2);

		MapVariant& object0 = object_array.get_ref_to_value<MapVariant>(0);
		assert(object0.has("name"));
		assert(object0["name"]->m_type == Variant::type::string_t);
		assert(object0["name"]->m_string == "object0");

		MapVariant& object1 = object_array.get_ref_to_value<MapVariant>(1);
		assert(object1.has("name"));
		assert(object1["name"]->m_type == Variant::type::string_t);
		assert(object1["name"]->m_string == "object1");
	}

	{
		assert(map_var["null_array"]->m_type == Variant::type::vector_t);
		VectorVariant& null_array = map_var.get_ref_to_value<VectorVariant>("null_array");
		assert(null_array.size() == 4);
	}
	{
		assert(map_var["mixed_array"]->m_type == Variant::type::vector_t);
		VectorVariant& mixed_array = *map_var.get_value<VectorVariant>("mixed_array");
		assert(mixed_array.size() == 7);

		assert(mixed_array[0]->m_type == Variant::type::bool_t && mixed_array[0]->m_bool == true);
		assert(mixed_array[1]->m_type == Variant::type::int_t && mixed_array[1]->m_signed_int == 1);
		assert(mixed_array[2]->m_type == Variant::type::float_t && mixed_array[2]->m_float > 3.0);
		assert(mixed_array[3]->m_type == Variant::type::bool_t && mixed_array[3]->m_bool == false);
		assert(mixed_array[4]->m_type == Variant::type::int_t && mixed_array[4]->m_signed_int == -3);
		assert(mixed_array[5]->m_type == Variant::type::float_t && mixed_array[5]->m_float < -6.0);
		assert(mixed_array[6]->m_type == Variant::type::string_t && mixed_array[6]->m_string == "a string");
	}
	std::cout << "JSON parse test successful. Data types, value and structure as expected.\n\n";
}
