#pragma once

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	typedef uint64 DwordString;

	//Упаковывает строку, размером меньше 9 байт в uint64.
	DwordString packDwordString(const char* string);
}
