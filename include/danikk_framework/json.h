#pragma once

#include <json-c/json.h>

#include <exception>
#include <danikk_framework/string.h>
#include <json-c/json.h>

namespace danikk_framework
{
	class JSON
	{
	private:
		json_object* ptr;
	public:

		JSON(const char* str);

		JSON(const String& str);

		JSON(json_object* ptr);

		~JSON();

		class Exception : std::exception
		{
		private:
			String error_buffer;
		public:
			Exception(String&& error_buffer);

			virtual const char* what() const noexcept;
		};

		enum
		{
			  type_null = json_type_null,
			  type_boolean = json_type_boolean,
			  type_double = json_type_double,
			  type_int = json_type_int,
			  type_object = json_type_object,
			  type_array = json_type_array,
			  type_string = json_type_string
		};

		JSON operator[](const char* key);

		void checkType(size_t requested_type);

		template<size_t char_count> JSON operator[](const char* key[char_count])
		{
			return (*this)[(const char*)key];
		}

		const char* getString();

		double getDouble();

		int64 getInteger();

		void printKeyValues()
		{
			json_object_object_foreach(ptr, key, val)
			{
				cout << "type: ";
			    switch (json_object_get_type(val))
			    {
			      case json_type_boolean:
			    	  cout << "boolean";
			    	  break;
			      case json_type_double:
			    	  cout << "double";
			    	  break;
			      case json_type_int:
			    	  cout << "int";
			    	  break;
			      case json_type_string:
			    	  cout << "string";
			    	  break;
			      default:
			    	  break;
			    }

				cout << " key: " << key;
				cout << " value: ";

			    switch (json_object_get_type(val))
			    {
			      case json_type_boolean:
			    	  cout << json_object_get_boolean(val);
			      case json_type_double:
			    	  cout << json_object_get_double(val);
			      case json_type_int:
			    	  cout << json_object_get_int(val);
			      case json_type_string:
			    	  cout << json_object_get_string(val);
			      break;
			      default:
			    	  break;
			    }

			    cout << endl;
			}
		}

		class Iterator
		{
		private:
			json_object* ptr;
			size_t index;
		public:
			Iterator(json_object* ptr, size_t index);

			void operator++();

			bool operator==(Iterator& other);

			JSON operator*();
		};

		Iterator begin();

		Iterator end();

	};
}
