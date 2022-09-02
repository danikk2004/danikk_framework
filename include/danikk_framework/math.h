#ifndef DANIKK_PLATFORM_MATH_H
#define DANIKK_PLATFORM_MATH_H
#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	//Логарифм по основанию двойки.
	size_t log2(size_t x);

	//Степень двойки.
	size_t powOf2(size_t x);

	//Выравнивает число до степени двойки.
	size_t alignToBigger2Pow(size_t x);

	//Делает число кратным, по отношении к другому числу.
	size_t alignTo(size_t x, size_t y);

	//Делит число на другое, но округляет в большую сторону.
	size_t divUp(size_t x, size_t y);
}

#endif
