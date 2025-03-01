#pragma once

#include <danikk_framework/string.h>

namespace danikk_framework
{
	struct Line
	{
		const char* data = NULL;
		size_t size = 0;
	};

	bool canGetLine(const char* str);

	Line getLine(const char*& str);
}
