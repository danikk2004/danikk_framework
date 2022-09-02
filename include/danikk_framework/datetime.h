#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/string_buffer.h>

namespace danikk_framework
{
	typedef StringBuffer<24> DateTimeString;

	struct DateTime
	{
		uint16 year;
		uint8 month;
		uint8 day;
		uint8 hour;
		uint8 minute;
		uint8 second;

		DateTimeString getString(char date_separator = '-', char datetime_separator = ' ', char time_separator = ':');

		DateTimeString getDateString(char date_separator = '-');

		static DateTime local();
	} ALIGNED_STRUCT(8);
}
