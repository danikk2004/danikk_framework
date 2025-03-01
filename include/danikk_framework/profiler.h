#pragma once

#include <danikk_framework/datetime.h>
#include <danikk_framework/log.h>

namespace danikk_framework
{
	namespace internal
	{
		template<class watcher_t> class Watch
		{
		private:
			watcher_t* watcher_ptr;
		public:
			Watch(watcher_t& watcher)
			{
				watcher_ptr = &watcher;
				watcher_ptr->startWatch();
			}

			~Watch()
			{
				watcher_ptr->endWatch();
			}
		};
	}

	class Watcher
	{
	public:
		void startWatch();

		void endWatch();
	};

	class RecursionWatcher : Watcher
	{
	private:
		friend class internal::Watch<RecursionWatcher>;
		uint32 value = 0;

		void startWatch();

		void endWatch();
	public:
		typedef internal::Watch<RecursionWatcher> Watch;

		RecursionWatcher() = default;

		uint32& get();
	};

	class TimeWatcher : Watcher
	{
	private:
		friend class internal::Watch<TimeWatcher>;
		const char* function_name;
		uint64 time_sum = 0;
		uint64 start_time;
		uint32 watch_counter = 0;
	public:
		typedef internal::Watch<TimeWatcher> Watch;

		TimeWatcher(const char* function_name, bool no_log = false);

		~TimeWatcher();

		void startWatch();

		void endWatch();

		uint64 elapsed();
	};
}
