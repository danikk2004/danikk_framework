#include <danikk_framework/dword_string.h>
#include <cstring>

namespace danikk_framework
{
	DwordString packDwordString(const char* string)
	{
		const char* currentString = string;
		size_t len = 0;
		while(*currentString != '\0')
		{
			currentString++;
			len++;
		}
		if(len > 8)
		{
			return packDwordString("overflow");
		}
		DwordString result = 0;
		currentString = string;
		size_t startLen = len;
		while(len > 0)
		{
			result += (DwordString)(*currentString) << ((startLen - len) * 8);
			currentString++;
			len--;
		}
		return result;
	}
}
