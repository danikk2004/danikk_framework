#ifndef DANIKK_PLATFORM_COUT_FEATURES_H
#define DANIKK_PLATFORM_COUT_FEATURES_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/cstring.h>

namespace danikk_framework
{
	typedef decltype(std::endl<char, std::char_traits<char>>) endl_t;

    template<class stream_t> stream_t& operator << (stream_t& stream, endl_t value)
    {
    	(void)value;
    	stream << '\n';
    	return stream;
    }

    using std::ostream;

	struct indent
	{
		size_t size;
	};

	template<class stream_t> stream_t& operator << (stream_t& cout, indent indent)
	{
		while(indent.size-- > 0)
		{
			cout << ' ';
		}
		return cout;
	}

	template<class string_t, char left_char, char right_char = left_char> struct side_chars
	{
		const char* str;

		side_chars(string_t& str)
		{
			this->str = tocstring(str);
		}
	};

	template<class stringstream_t, class string_t, char left_char, char right_char> stringstream_t& operator
		<<(stringstream_t& cout, side_chars<string_t, left_char, right_char> string)
	{
		cout << left_char << string.str << right_char;
		return cout;
	}

	template<class string_t> struct single_quotes : side_chars<string_t, '\''>
	{
		single_quotes(string_t& str) : side_chars<string_t, '\''>(str){}
	};

	template<class string_t> struct brackets : side_chars<string_t, '(', ')'>
	{

	};

	template<class string_t> struct square_brackets : side_chars<string_t, '[', ']'>
	{

	};

	template<class number_t> struct zero_aligned
	{
		number_t value;
		size_t number_count;
	};

	template<class stream_t, class number_t> stream_t& operator << (stream_t& out, const zero_aligned<number_t>& data)
	{
		size_t number_count = 1;

		for(number_t i = data.value; (i /= 10) > 1; number_count++);//Вычисление количества цифр в числе.

		if(number_count <= data.number_count)
		{
			size_t zero_count = data.number_count - number_count;

			while(zero_count-- > 0)
			{
				out << '0';
			}
		}
		out << data.value;

		return out;
	}
}

#endif
