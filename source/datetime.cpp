#include <danikk_framework/datetime.h>
#include <danikk_framework/format.h>
#include <danikk_framework/string.h>
#include <danikk_framework/number.h>
#include <danikk_framework/cstring_functions.h>
#include <time.h>

#if IS_LINUX
    #include <sys/time.h>
#elif IS_WINDOWS
    #include <sys/timeb.h>
#else
	#error not implemented
#endif

namespace danikk_framework
{
	void DateTime::parse(const String& data)
	{
		readFrom(data.c_string());
	}

	void DateTime::readFrom(const char* data)
	{
		for(index_t datenum_index = 0;;datenum_index++)
		{
			uint num = readNumber<uint>(data);
			switch(datenum_index)
			{
				case 0:	year 	= num; 	break;
				case 1: month 	= num;	break;
				case 2: day 	= num;	break;
				case 3: hour 	= num;	break;
				case 4: minute 	= num;	break;
				case 5: second 	= num;	break;
				case 6: 				return;
			}
			if(strempty(data))
			{
				if(datenum_index < 5)
				{
					error = true;
				}
				return;
			}
			else if(datenum_index == 5)
			{
				return;
			}
		}
	}

	DateTimeString DateTime::getString(char date_separator, char datetime_separator, char time_separator) const
	{
		DateTimeString result;
		result
		<< (uint64)year << date_separator
		<< zero_aligned(month, 2) << date_separator
		<< zero_aligned(day, 2) << datetime_separator
		<< zero_aligned(hour, 2) << time_separator
		<< zero_aligned(minute, 2) << time_separator
		<< zero_aligned(second, 2);
		return result;
	}

	DateTimeString DateTime::getDateString(char date_separator) const
	{
		DateTimeString result;
		result
		<< (uint64)year << date_separator
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
		result.minute	= current.tm_min;
		result.second	= current.tm_sec;

		if(result.second == 60)
		{
			result.second = 0;
		}
		return result;
	}

	#define CMP(field) \
		if(first.field CMP_OPER1 second.field) return true; \
		if(first.field CMP_OPER2 second.field) return false;\

	#define CMP_CODE\
		CMP(year)\
		CMP(month)\
		CMP(day)\
		CMP(hour)\
		CMP(minute)\
		CMP(second)

	bool operator <(const DateTime& first, const DateTime& second)
	{
		#define CMP_OPER1 <
		#define CMP_OPER2 >
			CMP_CODE
		#undef CMP_OPER1
		#undef CMP_OPER2
		return false;
	}

	bool operator >(const DateTime& first, const DateTime& second)
	{
		#define CMP_OPER1 >
		#define CMP_OPER2 <
			CMP_CODE
		#undef CMP_OPER1
		#undef CMP_OPER2
		return false;
	}

	bool operator >=(const DateTime& first, const DateTime& second)
	{
		#define CMP_OPER1 >
		#define CMP_OPER2 <
			CMP_CODE
		#undef CMP_OPER1
		#undef CMP_OPER2
		return true;
	}

	bool operator <=(const DateTime& first, const DateTime& second)
	{
		#define CMP_OPER1 <
		#define CMP_OPER2 >
			CMP_CODE
		#undef CMP_OPER1
		#undef CMP_OPER2
		return true;
	}

	#undef CMP
	#undef CMP_CODE

	uint64 getLocalSecondsTimestamp()
	{
		return time(NULL);
	}

	uint64 getLocalMinutesTimestamp()
	{
		return time(NULL) / 60;
	}

#if IS_LINUX
    uint64 getSystemTimeInMilliseconds()
    {
    	timeval t;
		gettimeofday(&t, NULL);
        return t.tv_sec * 1000LL + t.tv_usec / 1000;
    }
#elif IS_WINDOWS
    uint64 getSystemTimeInMilliseconds()
    {
    	_timeb t;
    	_ftime(&t);
        return t.time * 1000LL + t.millitm;
    }
#else
	#error not implemented
#endif
}
