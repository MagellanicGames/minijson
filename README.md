# Mini JSON for C++

A mini JSON library made with C++ 14 that is easy to use. It is intended for use in small scope game dev. projects.

This library is being used in currently in an in-development game and so will probably still receive updates and bug fixes.

The only (known) limitation with MJSON is that the root object for any input JSON files must have an object or array as the root element.  The reason for this is just because I have no requirement currently to have JSON files that are not a collection of data. So even though it is correct to have a string, number etc. as as root element, this is not supported.


## Building

Simply copy the files to your project and include "MJSON.hpp" to use.  All code is scoped under the MJSON namespace, so it can be easily seperated from your project's code.  There are no exterior dependencies.

Compiled binaries to link to will be coming in the future.

Mini JSON does require C++ 14 as a minimum.  This library has only been tested with G++ 11.3.0, so outside of the I can't help.  I will be testing it with MSVC when I have chance, but the intention is that this library should be easily usable with any compiler supporting C++ 14.


## Use


#### Testing (Optional)

Once MJSON has been added to your project, you can test everything is working as intended by running the JSONTestParser (include JSONTestParser.hpp).  Of course this means that everything is working as intended.  If you make any modifications to the code, you can rerun this test to check if anything has been unintentionally broken.

```
#include "JSONTestParser.hpp"

int main(int argc, char *argv[])
{
	MJSON::JSONTestParser test;
	test.test_parser();
	return 0;
}
```

#### Reading a JSON file

```
#include "{path_to_mjson_src_folder}/MJSON.hpp"

int main(int argc, char *argv[])
{
	MJSON::JSON json;
    json.load_src("{path_to_json_file}");
	test.test_parser();

	auto parsed_json = json.get_parsed_json();


    //your code goes here to process the parsed json
	return 0;
}
```

#### Parsed JSON structure & Variant

Upon running the parser you will receive back a `shared_ptr<ContainerVariant>`.  This pointer will be either VectorVariant or MapVariant depending what the root element of your JSON was.

The Variant class has a type for each JSON type:

+ Null
+ Int
+ Bool
+ Float
+ StringV
+ VectorVariant (maps to JSON array)
+ MapVariant (maps to JSON object)

All variants can be queried for their type by checking their `Variant->m_type` member variable.

```
if(variant->m_type == Variant::type::float_t)
{
    //cast, assign, whatever
}
```

Int, Bool, and Float can all have their data retrieved from the base Variant class by accessing the their respective m_signed_int, m_bool and m_float members (which are stored as a union).

Strings can be accessed via the m_string member.

Each of the none container type variants (Int, Bool etc.) can be cast to a concrete class of the basic type.  These also possess overloads for simple assignment to basic C++ types:

```
using namespace MJSON;

Variant* int_variant;
Int* int_obj = dynamic_cast<Int*>(int_variant);
int my_int = *int_obj;
```

#### Variant Containers.

The `shared_ptr<ContainerVariant>` received from the `get_parsed_json()` function can be cast to either a VectorVariant or MapVariant.  You should know from your JSON src what type is expected but like the other variants this can be queried with the `m_type` member.

To retrieve their ContainerVariant's data you must cast it to it's concrete type.  This is because each use a STL container for the underlying data structure. ` std::vector<unique_ptr<Variant>>` for VectorVariant and `std::unordered_map<std::string,unique_ptr<Variant>` for MapVariant.  So VectorVariants need an integer index and MapVariants need string keys.

```
VectorVariant& vector_variant = *dynamic_cast<VectorVariant*>(variant_container_ptr);

if(vector_variant[24]->m_type == Variant::type::bool_t)
    bool my_bool = vector_variant[24]->m_bool;

MapVariant& map_variant = *dynamic_cast<MapVariant*>(variant_container_ptr);

if(map_variant["a_bool_value"]->m_type == Variant::type::bool_t)
    bool my_bool = map_variant["a_bool_value"]->m_bool;
```

#### Retrieving Data from VariantContainers

Both container types overload the [] operator, so they can be accessed like any other array or map.  Using the [] operator will return a `Variant*`

To simplify and make the retrivale code more readable (as there is a lot of casting being done) the container variants each implement `get_value<T>()` and `get_ref_to_value<T>()` function, where `T` is of type `Variant`.

These functions are best used for retrieving nested containers, rather than simple number or string values where it would be best to simply access the data from a `Variant*` to avoid any casting.

You can look through the JSONTestParser.cpp to see their usage, but of course, here is a simple example.

```
    JSON j;
	j.load_src_from_string(test_json_src);
	auto parsed_json = j.get_parsed_json();

	MapVariant& map_var = *dynamic_cast<MapVariant*>(parsed_json.get());

	if(map_var.has("array_of_ints"))
	{
        VectorVariant& array_of_ints = map_var.get_ref_to_value<VectorVariant>("array_of_ints");

        for(int i = 0; i < array_of_ints.size(); i++)
        {
            std::cout << array_of_ints[i]->m_signed_int << "\n"
        }
	}

```

The `get_ref_to_value<T>()` function allows an easier interface for retrieving data from nested arrays or objects.

That should be all that is required to use the library.  All of the `Variant` types intended purpose is  for transferring data from JSON to your projects concrete types/classes.  Especially when used for a game project, these types are woefully cumbersome and inefficient to use.

If there are any issues, let me know and I'll try and get them rectified.

## Future Development

The Mini JSON project will continue to be added to over time as it is used more and refined.  There are already a few things that need altering, but currently as the project stands it is useable and seemingly minimal on bugs.  Currently being the only user, I of course cannot state there are no bugs.  There just has to be as I can only test it so much.

The first things I intend to improve with the library is the interface for retrieving data from the Variant classes.  The `get_value<T>()` function name is a bit confusing as it retrieves and pointer.
There also need to be static asserts to ensure that the type T used with `get_ref_to_value<T>()` is derived from `Variant` only.
