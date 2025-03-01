#include <danikk_framework/profiler.h>

namespace danikk_framework
{
	void RecursionWatcher::startWatch()
	{
		value++;
	}

	void RecursionWatcher::endWatch()
	{
		value--;
	}

	uint32& RecursionWatcher::get()
	{
		return value;
	}



	TimeWatcher::TimeWatcher(const char* function_name, bool no_log)
	{
		if(no_log)
		{
			this->function_name = NULL;
		}
		else
		{
			this->function_name = function_name;
		}
		startWatch();
	}

	TimeWatcher::~TimeWatcher()
	{
		if(function_name == NULL)
		{
			return;
		}
		if(watch_counter == 0)
		{
			endWatch();
		}
		if(watch_counter > 1)
		{
			uint avg = time_sum / watch_counter;
			formatLogDebug("% count:% stat-avg:%ms time:%ms", function_name, watch_counter, avg, time_sum);
		}
		else
		{
			formatLogDebug("% time:%ms", function_name, time_sum);
		}
	}

	void TimeWatcher::startWatch()
	{
		start_time = getSystemTimeInMilliseconds();
	}

	void TimeWatcher::endWatch()
	{
		uint32 current_time = getSystemTimeInMilliseconds();
		uint32 delta_time = current_time - start_time;
		time_sum += delta_time;
		watch_counter++;
	}

	uint64 TimeWatcher::elapsed()
	{
		return getSystemTimeInMilliseconds() - start_time;
	}
}
