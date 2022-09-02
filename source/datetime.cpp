#include <danikk_framework/datetime.h>
#include <danikk_framework/format.h>
#include <time.h>

namespace danikk_framework
{
	DateTimeString DateTime::getString(char date_separator, char datetime_separator, char time_separator)
	{
		DateTimeString result;
		result
		<< year << date_separator
		<< zero_aligned(month, 2) << date_separator
		<< zero_aligned(day, 2) << datetime_separator
		<< zero_aligned(hour, 2) << time_separator
		<< zero_aligned(minute, 2) << time_separator
		<< zero_aligned(second, 2);
		return result;
	}

	DateTimeString DateTime::getDateString(char date_separator)
	{
		DateTimeString result;
		result
		<< year << date_separator
		<< zero_aligned(month, 2) << date_separator
		<< zero_aligned(day, 2);
		return result;
	}

	DateTime DateTime::local()
	{
		time_t raw_time;
		time(&raw_time);
		tm current = *localtime(&raw_time);

		DateTime result;

		result.year 	= current.tm_year + 1900;
		result.month 	= current.tm_mon + 1;
		result.day 		= current.tm_mday;
		result.hour 	= current.tm_hour;
		result.minute	= current.tm_mon;
		result.second	= current.tm_sec;

		if(result.second == 60)
		{
			result.second = 0;
		}
		return result;
	}
}
