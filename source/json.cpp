#include <danikk_framework/json.h>
#include <danikk_framework/format.h>

namespace danikk_framework
{
	static const char* typeToString(size_t type)
	{
		switch(type)
		{
			case JSON::type_null: 		return "null";
			case JSON::type_boolean: 	return "boolean";
			case JSON::type_double: 	return "double";
			case JSON::type_int: 		return "int";
			case JSON::type_object: 	return "object";
			case JSON::type_array: 		return "array";
			case JSON::type_string: 	return "string";
			default: return "unknown";
		}
	}

	static void checkType(size_t current_type,size_t requested_type)
	{
		if(current_type != requested_type)
		{
			const char* current_type_str = typeToString(current_type);
			const char* requested_type_str = typeToString(requested_type);

			String error_buffer;

			formatAppend(error_buffer,
					"[danikk_framework::JSON::Exception] current_type: % requested_type: %",
					current_type_str, requested_type_str);

			throw JSON::Exception((String&&)error_buffer);
		}
	}

	using Exception = JSON::Exception;

	Exception::Exception(String&& error_buffer)
	{
		this->error_buffer = (String&&)error_buffer;
	}

	const char* Exception::what() const noexcept
	{
		return error_buffer.c_string();
	}

	JSON::JSON(const char* str):JSON(json_tokener_parse(str)){}

	JSON::JSON(const String& str):JSON(str.c_string()){}

	JSON::JSON(json_object* ptr)
	{
		this->ptr = ptr;
		//this->ptr = json_object_get(ptr);
	}

	JSON::~JSON()
	{
		//json_object_put(ptr);
	}

	JSON JSON::operator[](const char* key)
	{
		json_object_iter iter;
		json_object_object_foreachC(ptr, iter)
		{
			if(strequal(key, iter.key))
			{
				return iter.val;
			}
		}
		return (json_object*)NULL;
		//return json_object_object_get(ptr, key);
	}

	void JSON::checkType(size_t requested_type)
	{
		danikk_framework::checkType(json_object_get_type(ptr), requested_type);
	}

	const char* JSON::getString()
	{
		checkType(type_string);
		return json_object_get_string(ptr);
	}

	double JSON::getDouble()
	{
		checkType(type_double);
		return json_object_get_double(ptr);
	}

	int64 JSON::getInteger()
	{
		checkType(type_int);
		return json_object_get_int(ptr);
	}

	using Iterator = JSON::Iterator;

	Iterator::Iterator(json_object* ptr, size_t index)
	{
		this->ptr = ptr;
		this->index = index;
	}

	void Iterator::operator++()
	{
		index++;
	}

	bool Iterator::operator==(Iterator& other)
	{
		return ptr == other.ptr && index == other.index;
	}

	JSON Iterator::operator*()
	{
		return json_object_array_get_idx(ptr, index);
	}

	Iterator JSON::begin()
	{
		checkType(type_array);

		return Iterator(ptr, 0);
	}

	Iterator JSON::end()
	{
		checkType(type_array);

		return Iterator(ptr, json_object_array_length(ptr));
	}
	/*
	const char *key = NULL;
	struct json_object *val = NULL;
	struct lh_entry *entry;
	struct lh_entry *entry_next = NULL;
	for(entry = json_object_get_object(obj)->head;
		(entry ? (
			key = (char*)lh_entry_k(entry),
			val = (struct json_object*)lh_entry_v(entry),
			entry_next = entry->next,
			entry) : 0);
		entry = entry_next)

	json_object_object_foreach(jobj, key, val)*/
}
