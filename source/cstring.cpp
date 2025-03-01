#include <danikk_framework/cstring_functions.h>
#include <stddef.h>

namespace danikk_framework
{
	bool strempty(const char* str)
	{
		return *str == '\0';
	}

	bool strcontains(const char* str, char chr1)
	{
		for(char currentChar; (currentChar = *str); str++)
		{
			if (currentChar == chr1)
			{
				return true;
			}
		}
		return false;
	}
	bool strcontains(const char* str, char chr1, char chr2)
	{
		for(char currentChar; (currentChar = *str); str++)
		{
			if (currentChar == chr1 || currentChar == chr2)
			{
				return true;
			}
		}
		return false;
	}

	bool strequal(const char* str1, const char* str2)
	{
		while(true)
		{
			char chr1 = *str1++;
			char chr2 = *str2++;
			if(chr1 != chr2)
			{
				return false;
			}
			if(chr1 == '\0')
			{
				return true;
			}
		}
	}

	bool strnotequal(const char* str1, const char* str2)
	{
		while(true)
		{
			char chr1 = *str1++;
			char chr2 = *str2++;
			if(chr1 != chr2)
			{
				return true;
			}
			if(chr1 == '\0')
			{
				return false;
			}
		}
	}

	bool strcontainsbefore(const char* str, char chr, char end)
	{
		for(char currentChar; (currentChar = *str); str++)
		{
			if (currentChar == chr)
			{
				return true;
			}
			if (currentChar == end)
			{
				return false;
			}
		}
		return false;
	}

	bool strstartswith(const char* str1, const char* str2)
	{
		while(true)
		{
			char chr1 = *str1++;
			char chr2 = *str2++;
			if(chr2 == '\0')
			{
				return true;
			}
			if(chr1 != chr2)
			{
				return false;
			}
		}
	}

	bool strmifsw(const char*& str1, const char* str2)
	{
		const char* str1_copy = str1;
		while(true)
		{
			char chr1 = *str1_copy;
			char chr2 = *str2;
			if(chr2 == '\0')
			{
				str1 = str1_copy;
				return true;
			}
			if(chr1 != chr2)
			{
				return false;
			}
			str1_copy++;
			str2++;
		}
	}

	bool strcontainsonly(const char* str, char chr1, char chr2, char chr3)
	{
		for(char currentChar; (currentChar = *str); str++)
		{
			if (currentChar != chr1 && currentChar != chr2 && currentChar != chr3)
			{
				return false;
			}
		}
		return true;
	}

	unicode_char strlastchar(const char* str)
	{
		while(*str != '\0')
		{
			str++;
		}
		return *(str - 1);
	}

	size_t strcut(char* str, char chr)
	{
		char* readptr = str;
		char* writeptr = str;
		char curchr;

		while((curchr = *readptr++) != '\0')
		{
			if(curchr != chr)
			{
				*writeptr++ = curchr;
			}
		}

		return writeptr - str - 1;
	}

	size_t strscpy(char* dest, const char* source, size_t dest_size)
	{
		char* end = dest + dest_size;
		char* current = dest;

		while(current < end)
		{
			char current_chr = *source;
			source++;
			if(current_chr == '\0')
			{
				return current - dest;
			}
			*current = current_chr;
		}
		return dest_size;
	}
}
