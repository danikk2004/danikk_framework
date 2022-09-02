#include <danikk_framework/line_getter.h>

namespace danikk_framework
{
	LineGetter::LineGetter(char* memory) : memory(memory), lastChar(memory){}

	LineGetter::LineGetter(String& memory) : memory(memory.data()), lastChar(memory.data()){}

	const char* LineGetter::getLine()
	{
		const char* result = lastChar;
		while(true)
		{
			char currentChar = *lastChar;
			if(currentChar == '\n' || currentChar == '\0')
			{
				*lastChar = '\0';
				lastChar++;
				return result;
			}
			lastChar++;
		}
	}

	LineGetter::operator bool()
	{
		return *lastChar != '\0';
	}
}
