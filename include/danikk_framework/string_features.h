#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <danikk_framework/dynamic_array.h>

namespace danikk_framework
{
	static inline DynamicArray<String> splitStringToArray(const String& str, char separator)
	{
		DynamicArray<String> result;
		String* current_str = &result.pushCtor();

		for(char chr : str)
		{
			if(chr == separator)
			{
				current_str = &result.pushCtor();
			}
			else
			{
				*current_str << chr;
			}
		}
		return move(result);
	}
}
