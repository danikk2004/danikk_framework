#pragma once

#include <danikk_framework/string.h>

namespace danikk_framework
{
	inline static const char* hex_source = "0123456789ABCDEF";

	//n- это размер input, а размер output это n * 2;
	inline void tohex(char* output, const byte* input, size_t n)
	{
		const byte* end = input + n;

	    while(input < end)
	    {
	    	uint8 chr = *input;
	    	input++;
	        *output = hex_source[(chr >> 4) & 0x0F];
	        output++;
	        *output = hex_source[chr 		& 0x0F];
	        output++;
	    }
	}
}
