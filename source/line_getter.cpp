#include <danikk_framework/misc/line_getter.h>
#include <danikk_framework/assert.h>

namespace danikk_framework
{
	Line getLine(const char*& str)
	{
		assert(*str != '\0');
		Line result;
		result.data = str;
		while(true)
		{
			if(*str == '\0')
			{
				return result;
			}
			if(*str == '\n')
			{
				str++;
				return result;
			}
			result.size++;
			str++;
		}
	}

	bool canGetLine(const char* str)
	{
		return *str != '\0';
	}
}
