#pragma once

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	//Выдаёт хэш код с гарантией защиты от коллизий, если
	//длинна строки меньше или равна 9 символам
	//и она закодирована в ascii.
	uint64 getSumHash64(const char* str)
	{
		uint64 result = 0;
		size_t currentChar = 'A';
		size_t charIndex = 0;

		while(currentChar != '\0')
		{
			currentChar = str[charIndex];
			result += currentChar << ((charIndex % 9) * 7);
			charIndex++;
		}
		return result;
	}
}
