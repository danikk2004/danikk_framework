#ifndef DANIKK_PLATFORM_LINE_GETTER_H
#define DANIKK_PLATFORM_LINE_GETTER_H

#include <danikk_framework/string.h>

namespace danikk_framework
{
	class LineGetter
	{
	private:
		char* memory;
		char* lastChar;

	public:
		LineGetter(char* memory);

		LineGetter(String& memory);

		const char* getLine();

		operator bool();
	};
}

#endif
