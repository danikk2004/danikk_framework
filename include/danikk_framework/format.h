#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/cout_features.h>

namespace danikk_framework
{
	template<class out_t> void formatAppend(out_t& out, const char* format_str)
	{
		while(*format_str)
		{
			char current_char = *format_str;
			switch(current_char)
			{
			case '\0':
				return;
			case '%':
				return;
			default:
				out << current_char;
				break;
			}
			format_str++;
		}
	}

	template<class out_t, class first_t> void formatAppend(out_t& out, const char* format_str, const first_t& first)
	{
		while(*format_str)
		{
			char current_char = *format_str;
			switch(current_char)
			{
			case '\0':
				return;
			case '-':
				if(*(format_str + 1) == '%')
				{
					format_str++;
					out << (char)'%';
				}
				else
				{
					out << (char)'-';
				}
				break;
			case '%':
				out << first;
				formatAppend(out, format_str + 1);
				return;
			default:
				out << current_char;
				break;
			}
			format_str++;
		}
	}

	template<class out_t, class first_t, class ...other_t> void formatAppend(out_t& out, const char* format_str, const first_t& first, const other_t&... args)
	{
		while(*format_str)
		{
			char current_char = *format_str;

			switch(current_char)
			{
			case '\0':
				return;
			case '-':
				if(*(format_str + 1) == '%')
				{
					format_str++;
					out << (char)'%';
				}
				else
				{
					out << (char)'-';
				}
				break;
			case '%':
				out << first;
				formatAppend(out, format_str+1, args...);
				return;
			default:
				out << current_char;
				break;
			}
			format_str++;
		}
	}

	template<class out_t, class ...args_t> void format(out_t& out, const char* format_str, const args_t&... args)
	{
		out.clear();
		formatAppend(out, format_str, args...);
	}

	template<class out_t, class ...args_t> void formatWrite(out_t& out, const char* format_str, const args_t&... args)
	{
		formatAppend(out, format_str, args...);
	}
}
