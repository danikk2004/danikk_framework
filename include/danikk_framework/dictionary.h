#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/dynamic_array.h>
#include <danikk_framework/string.h>
#include <danikk_framework/number.h>
#include <danikk_framework/format.h>
#include <danikk_framework/cstring_functions.h>
#include "array.h"

namespace danikk_framework
{
	template<class valueT> class StringDictionary
	{
	public:
		struct pair
		{
			String key;
			valueT value;
		};
	private:
		DynamicArray<pair> data;
	public:
		StringDictionary() = default;

		template<class key_t> void insert(const key_t& key, const valueT& value)
		{
			pair& new_pair = data.pushCtor();
			new_pair.key = key;
			new_pair.value = value;
		}

		template<class key_t> void insert(const key_t& key, valueT& value)
		{
			pair& new_pair = data.pushCtor();
			new_pair.key = key;
			new_pair.value = value;
		}

		valueT* get(const char* key)
		{
			for(pair& p : data)
			{
				if(strequal(p.key.c_string(), key))
				{
					return &p.value;
				}
			}
			return NULL;
		}

		valueT* get(const String& key)
		{
			for(pair& p : data)
			{
				if(p.key == key)
				{
					return &p.value;
				}
			}
			return NULL;
		}
	};
}
