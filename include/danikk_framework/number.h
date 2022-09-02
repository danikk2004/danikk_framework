#ifndef DANIKK_PLATFORM_NUMBER_H
#define DANIKK_PLATFORM_NUMBER_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <danikk_framework/type_traits.h>
#include <cstdlib>
#include <concepts>
#include <type_traits>

namespace danikk_framework
{
	template<class numberT> size_t writeNumberToStringBuffer(numberT number, char* buffer, size_t bufferSize)
	{
		size_t len;

#define printToBuffer(format, type) snprintf(buffer, bufferSize, format, (type)number)
		if constexpr(std::is_signed<numberT>::value && std::is_integral<numberT>::value)
		{
			len = printToBuffer("%lli", long long int);
		}
		else if constexpr(std::is_unsigned<numberT>::value && std::is_integral<numberT>::value)
		{
			len = printToBuffer("%llu", long long unsigned int);
		}
		else if constexpr(std::is_same<numberT, float>::value)
		{
			len = printToBuffer("%f", float);
		}
		else if constexpr(std::is_same<numberT, double>::value)
		{
			len = printToBuffer("%lf", double);
		}
		else
		{
			static_assert(true, "this is not nubmer");//Это не число.
		}
#undef snprintf
		return len;
	}

	template<class numberT> String numberToString(numberT number)
	{
		char buffer[64];
		size_t len = writeNumberToStringBuffer(number, buffer, sizeof(buffer));
		return String((const char*)buffer, len);
	}

	template<class numberT> numberT parseNumber(const char* data)
	{
		if constexpr ((std::is_integral<numberT>::value && sizeof(numberT) < 4) || std::same_as<numberT, int>)
		{
			return (numberT)std::atoi(data);
		}
		else if constexpr(std::is_integral<numberT>::value)
		{
			char* a;
			return (numberT)std::strtoll(data,&a,10);
		}
		else if constexpr (std::is_floating_point<numberT>::value)
		{
			return (numberT)atof(data);
		}
		else
		{
			static_assert(true, "this is not nubmer");
		}
	}

	//float_t может являться double
	template<class float_t> struct truncatedFloat
	{
		float_t value;

		/*truncatedFloat(float_t value)
		{
			this->value = value;
		}*/
	};

	template<class stream_t, class float_t> stream_t operator<<(stream_t& out, truncatedFloat<float_t> tf)
	{
		char buffer[64];
		writeNumberToStringBuffer(tf.value, buffer, 64);
		char* current_char = buffer;

		while(*current_char != '.')
		{
			out << *current_char;
			current_char++;
		}

		if(*(current_char + 1) == '0')//Для игнорирования точки, а то раньше вместо "15" могло написать "15."
		{
			return out;
		}

		while(*current_char != '\0' && *current_char != '0')
		{
			out << (char)*current_char;
			current_char++;
		}

		return out;
	}

	template<class T> struct is_number
	{
		typedef remove_const<T>::type unconst_t;

		static constexpr bool value = std::is_floating_point<T>::value ||
		  std::is_integral<T>::value ||
		  std::is_integral<unconst_t>::value;
	};

	template<class T, class Return> struct enable_if_number
			: public std::enable_if<is_number<T>::value,
			  Return>
	{};

	template<class T, class Return> struct enable_if_not_number
			: public std::enable_if<is_number<T>::value,
			  Return>
	{};

	template <class streamT, class num_t> std::enable_if<is_number<num_t>::value,streamT&>::type operator << (streamT& cout, num_t num)
	{
		if constexpr (std::is_same<num_t, char>::value)
		{
			cout << (char)num;
		}
		else
		{
			char buffer[64];
			writeNumberToStringBuffer(num, buffer, 64);
			cout << (const char*)buffer;
			return cout;
		}
		return cout;
	}
}

#endif
