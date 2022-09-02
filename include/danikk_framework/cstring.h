#pragma once

#include <cstring>
#include <type_traits>
#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	typedef size_t unicode_char;

	template<class T> struct is_cstring
	{
		template<class arr_t> struct ischrarr
		{
			typedef std::remove_pointer<arr_t>::type unpointed;
			typedef std::remove_reference<unpointed>::type element_type;
			static constexpr bool value = std::is_array<T>::value && std::is_same<element_type, char>::value;
		};

		static constexpr bool value =
			std::is_convertible<T, const char*>::value		||
			std::is_convertible<T, const char* const>::value||
			std::is_same<T, char*>::value 					||
			std::is_same<T, const char*>::value 			||
			std::is_same<T, char* const>::value 			||
			std::is_same<T, const char* const>::value 		||
			ischrarr<T>::value;
	};

	//Те функции, которые являются шаблонными должны были
	//возвращать char* и const char*
	//И что-бы не дублировать код, можно просто
	//сделать тип си строки шаблоном.

#define CSTRING_TEMPLATE template<class cstringT> inline cstringT

	template<class stream_t,class cstring_t> std::enable_if<is_cstring<cstring_t>::value, stream_t&>::type operator << (stream_t& stream, cstring_t cstring)
	{
		const char* ptr = cstring;

		char curchr;
		while((curchr = *ptr) != '\0')
		{
			stream << curchr;
			ptr++;
		}

		return stream;
	}

	template<size_t n> inline const char* tocstring(const char value[n])
	{
		return value;
	}

	template<class cstringT> inline const char* tocstring(const cstringT& value)
	{
		if constexpr(is_cstring<cstringT>::value)
		{
			return value;
		}
		else if constexpr (std::is_same<cstringT, std::string>::value)
		{
			return value.c_str();
		}
		else
		{
			return value.c_string();
		}
	}

	bool strcontains(const char* str, char chr1);

	bool strcontains(const char* str, char chr1, char chr2);

	//Проверка строк на одинаковость.
	bool strequal(const char* str1, const char* str2);

	//Проверка строк на различие.
	bool strnotequal(const char* str1, const char* str2);

	//str содетжит символ chr перед первым символом end.
	bool strcontainsbefore(const char* str, char chr, char end);

	//Если str1 начинается с str2, то возвращает true.
	bool strstartswith(const char* str1, const char* str2);

	//Если str1 начинается с str2, то возвращает true.
	template<class str2T> bool strstartswith(const char* str1, const str2T& str2)
	{
		const char* str2Ptr = str2.data();
		const char* endPtr = str2.end();

		while(true)
		{
			if(str2Ptr == endPtr)
			{
				return true;
			}
			char chr1 = *str1++;
			if(chr1 == '\0')
			{
				return false;
			}
			if(chr1 != *str2Ptr++)
			{
				return false;
			}
		}
	}

	//Строка состоит только из заданных символов
	bool strcontainsonly(const char* str, char chr1, char chr2, char chr3);

	//Получает последний символ из строки.
	unicode_char strlastchar(const char* str);

	//Вырезает заданный символ из строки.
	//Возвращает длинну новой строки.
	size_t strcut(char* string, char character);

	//string safe copy
	//В 3-м аргументе указывается размер буффера назначения.
	//Возвращает количество копированных символов.
	//Если оно меньше dest_size, то буффер dest слишком мал.
	size_t strscpy(char* dest, const char* source, size_t dest_size);

	//Пропускает текст, пока текущий символ совпадает с одним из заданных.
	CSTRING_TEMPLATE strskipchars(cstringT str, const char chr1, const char chr2)
	{
		while(true)
		{
			char curchr = *str;
			if(curchr != chr1 && curchr != chr2)
			{
				return str;
			}
			str++;
		}
	}

	CSTRING_TEMPLATE strskipchars(cstringT str, char chr1, char chr2, char chr3)
	{
		for(char currentChar; (currentChar = *str); str++)
		{
			if(currentChar != chr1 && currentChar != chr2 && currentChar != chr3)
			{
				return str;
			}
		}
		return str;
	}

	//Пропускает до заданного символа.
	CSTRING_TEMPLATE strskiptochar(cstringT str,  char chr)
	{
		while(*str != chr)
		{
			++str;
		}
		return str;
	}

	//Пропускает текст, пока не текущий символ не будет равен одному из заданных.
	CSTRING_TEMPLATE strskiptochar(cstringT str, const char chr1, const char chr2)
	{
		for(char currentChar; (currentChar = *str) && currentChar != chr1 && currentChar != chr2; str++){}
		return str;
	}

	//Находит последний символ, которые совпадает с указанным.
	CSTRING_TEMPLATE strfindlast(cstringT str, char character)
	{
		cstringT lastCharPtr = NULL;
		for(char currentChar; (currentChar = *str); str++)
		{
			if(currentChar == character)
			{
				lastCharPtr = str;
			}
		}
		return lastCharPtr;
	}

	//Это просто более удобное название для strstr.
	CSTRING_TEMPLATE strfindsub(cstringT* str1, const char* str2)
	{
		return strstr(str1, str2);
	}

	//это strstr, но указатель сдвигается на размер str2.
	//То есть пропускает найденную подстроку,
	//путём возвращения указателя на место после неё.
	CSTRING_TEMPLATE strskipsub(cstringT str1, const char* str2)
	{
		cstringT result = strstr(str1, str2);
		while(*str2 != '\0')
		{
			str2 ++;
		}
		return result;
	}

#undef CSTRING_TEMPLATE
}
