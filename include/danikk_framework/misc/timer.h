#pragma once
#include <danikk_framework/dynamic_array.h>

#include <chrono>
#include <thread>

namespace danikk_framework
{
	class Timer
	{
	protected:
		std::chrono::seconds delay;
		void(*action)() = NULL;
		std::unique_ptr<std::thread> thread_ptr;

		static void main(ptr<Timer> ptr)
		{
			while(true)
			{
				ptr->action();
				std::this_thread::sleep_for(ptr->delay);
			}
		}
	public:
		Timer() = default;

		Timer(void(*action)(), size_t delay)
		{
			this->action = action;
			this->delay = std::chrono::seconds(delay);
		}

		void run()
		{
			thread_ptr = std::make_unique<std::thread>(Timer::main, this);
		}
	};

	class MutexTimer : public Timer
	{
	private:
		std::unique_ptr<std::mutex> mutex_ptr;

		static void main(ptr<MutexTimer> ptr)
		{
			while(true)
			{
				ptr->lock();
				ptr->action();
				ptr->unlock();
				std::this_thread::sleep_for(ptr->delay);
			}
		}


	public:
		MutexTimer() = default;

		MutexTimer(void(*action)(), size_t delay)
			: Timer(action, delay)
		{
			mutex_ptr = std::make_unique<std::mutex>();
		}

		void run()
		{
			thread_ptr = std::make_unique<std::thread>(MutexTimer::main, this);
		}

		void lock()
		{
			mutex_ptr->lock();
		}

		void unlock()
		{
			mutex_ptr->unlock();
		}
	};
}
