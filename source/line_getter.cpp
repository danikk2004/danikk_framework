#include <danikk_framework/misc/line_getter.h>
#include <danikk_framework/assert.h>

namespace danikk_framework
{
	char* getLine(char*& str)
	{
		assert(*str != '\0');
		char* result = str;
		while(true)
		{
			if(*str == '\0')
			{
				str++;
				return result;
			}
			if(*str == '\n')
			{
				*str = '\0';
				str++;
				return result;
			}
			str++;
		}
	}
}
