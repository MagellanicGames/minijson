/*
 * Variant.hpp
 *
 * These are types to convert from the JSON's dynamic types, to C++ static types.
 * There is a Variant for each JSON type to convert to a C++ type.
 * These are not intended to be used within projects other than as intermediary containers for data from
 * JSON to your project's classes/types.
 *
 *  Created on: 2 Dec 2022
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
#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

typedef  std::string string_data;

namespace MJSON
{
	class Variant
	{
	public:
		enum class type {null_t, int_t, float_t,bool_t, string_t, container_t, vector_t, map_t, end};

		Variant(type var_type):
			m_type(var_type)
		{}

		virtual ~Variant()
		{
			//std::cout << "MSJON::Variant::~Variant()\n";
		};



		type m_type;

		union {
			int m_signed_int;
			//unsigned int m_unsigned_int;
			bool m_bool;
			float m_float;
		};
		string_data m_string;

	};

	using VariantVec_t = std::vector<std::unique_ptr<Variant>> ;
	using VariantMap_t = std::unordered_map<std::string, std::unique_ptr<Variant>> ;

	class Null: public Variant
	{
	public:
		Null():Variant(type::null_t)
		{}
		virtual ~Null(){};

		int to_type()
		{
			return 0;
		}

	};

	class Int : public Variant
	{
	public:

		Int():Variant(type::int_t)
		{
			m_signed_int = 0;
		}

		Int(const Int & i):Int()
		{
			m_signed_int = i.m_signed_int;
		}

		Int(int i):Int()
		{
			m_signed_int = i;
		}

		Int(string_data& s):Int()
		{
			_from_string(s);
		}

		virtual ~Int(){};

		int to_type()
		{
			return m_signed_int;
		}

		void operator=(const int& i)
		{
			m_signed_int = i;
		}

		void operator=(const Int & i)
		{
			m_signed_int = i.m_signed_int;
		}

		void operator=(const unsigned int & ui)
		{
			m_signed_int = (int)ui;
		}

		void operator=(const bool & b)
		{
			m_signed_int = (int)b;
		}

		void operator=(const float & f)
		{
			m_signed_int = (int) f;
		}

		void operator=(const string_data & s)
		{
			_from_string(s);
		}

		operator int() const { return m_signed_int;}

	private:

		void _from_string(const string_data & s)
		{
			int num = 0;
			try
			{
				num = std::stoi(s);
			}
			catch (const std::invalid_argument& exception)
			{
				std::cerr << "Invalid args, number must be present at the beginning of the string.\n";
			}
			m_signed_int = (int)num;
		}
	};



	class Float : public Variant
	{
	public:
		Float():Variant(type::float_t)
		{
			m_float = 0.0f;
		}

		virtual ~Float(){};

		Float(const Float& f): Float()
		{
			m_float = f.m_float;
		}

		Float(float f) : Float()
		{
			m_float = f;
		}

		Float(int i) : Float()
		{
			m_float = (float)i;
		}

		Float(const string_data & s) : Float()
		{
			_from_string(s);
		}

		float to_type()
		{
			return m_float;
		}

		void operator=(const float& f)
		{
			m_float = f;
		}

		void operator=(const Float& f)
		{
			m_float = f.m_float;
		}

		void operator=(const int& i)
		{
			m_float = (float) i;
		}

		void operator=(const Int& i)
		{
			m_float = (float)i.m_signed_int;
		}

		void operator=(const string_data & s)
		{
			_from_string(s);
		}

		operator float() const {return m_float;}

	private:

		void _from_string(const string_data & s)
		{
			float num = 0;
			try
			{
				num = std::stof(s);
			}
			catch (const std::invalid_argument& exception)
			{
				std::cerr << "Invalid args, number must be present at the beginning of the string.\n";
			}
			m_float = (float)num;
		}

	};


	class Bool : public Variant
	{
	public:
		Bool():Variant(type::bool_t)
		{
			m_bool = false;
		}
		virtual ~Bool(){};

		Bool(const Bool& b ):Bool()
		{
			m_bool = b.m_bool;
		}

		Bool(const bool& b):Bool()
		{
			m_bool = b;
		}

		Bool(const std::string&s):Bool()
		{
			if(s == "true" || s == "True" || s == "TRUE")
				m_bool = true;
			else
				m_bool = false;
		}

		bool to_type()
		{
			return m_bool;
		}

		operator bool() const {return m_bool;}
	};

	class StringV : public Variant
	{
	public:
		StringV():Variant(type::string_t)
		{
			m_string = "";
		};
		virtual ~StringV() {};

		StringV(const StringV&s) : StringV()
		{
			m_string = s.m_string;
		}

		StringV(const string_data& s):StringV()
		{
			m_string = s;
		}

		StringV(const char* s): StringV()
		{
			m_string = string_data(s);
		}

		void operator=(const StringV& s)
		{
			m_string = s.m_string;
		}

		void operator=(const string_data& s)
		{
			m_string = s;
		}

		void operator=(const char* s)
		{
			m_string = string_data(s);
		}

		bool operator==(const char* s)
		{
			std::string other(s);
			return m_string == other;
		}

		bool operator==(const std::string& other)
		{
			return m_string == other;
		}

		bool operator==(const StringV& other)
		{
			return m_string == other.m_string;
		}

		operator std::string() const {return m_string;}
		operator const char*() const {return m_string.c_str();}
	};

	class ContainerVariant : public Variant
	{
	public:

		ContainerVariant(type container_type): Variant(container_type)		{}
		virtual ~ContainerVariant(){};

		virtual void add_variant(Variant::type var_type, std::string& value_str, std::string key = "") = 0;
		virtual Variant* get_last_added_variant() = 0;
	};

	class MapVariant;

	class VectorVariant : public ContainerVariant
	{
	public:
		VectorVariant():ContainerVariant(type::vector_t)
		{

		}
		virtual ~VectorVariant(){};

		void add_variant(Variant::type var_type, std::string& value_str, std::string key = "") override;
		Variant* get_last_added_variant() override;

		template<typename T>
		T* get_value(const int& idx)
		{
			return dynamic_cast<T*>(m_container[idx].get());
		}

		template<typename T>
		T& get_ref_to_value(const int& idx)
		{
			return *dynamic_cast<T*>(m_container[idx].get());
		}

		int size() const
		{
			return m_container.size();
		}

		Variant* operator[](const int& idx)
		{
			return m_container[idx].get();
		}

		Variant* operator[](const Int& idx)
		{
			return m_container[idx.m_signed_int].get();
		}

		VariantVec_t m_container;
	};


	class MapVariant : public ContainerVariant
	{
	public:
		MapVariant():ContainerVariant(type::map_t)
		{}
		virtual ~MapVariant(){};

		void add_variant(Variant::type var_type, std::string& value_str, std::string key = "") override;
		Variant* get_last_added_variant() override;

		template<typename T>
		T* get_value(const char* key)
		{
			return dynamic_cast<T*>(m_container[key].get());
		}

		template<typename T>
		T* get_value(const std::string& key)
		{
			return get_value<T>(key.c_str());
		}

		template<typename T>
		T& get_ref_to_value(const std::string& key)
		{
			return *get_value<T>(key.c_str());
		}

		template<typename T>
		T& get_ref_to_value(const char* key)
		{
			return *get_value<T>(key);
		}

		int size() const
		{
			return m_container.size();
		}

		bool has(const char* key) const
		{
			return m_container.find(key) != m_container.end();
		}

		bool has(const std::string& key) const
		{
			return has(key.c_str());
		}

		void print_keys() const;

		Variant* operator[](const std::string& key)
		{
			return m_container[key].get();
		}

		Variant* operator[](const char* key)
		{
			return m_container[key].get();
		}

		Variant* operator[](const StringV& key)
		{
			return m_container[key.m_string].get();
		}

		VariantMap_t m_container;
		std::string m_last_added_key = "";
	};

}
