#pragma once

#include <chrono>

#define FUNCTION_LOG_TIMER danikk_framework::internal::FunctionLogTimer __FLT__(__PRETTY_FUNCTION__);

namespace danikk_framework
{
	namespace internal
	{
		using namespace std::chrono;
		class FunctionLogTimer
		{
			const char* func_name;

			FunctionLogTimer()
			{
				const char* func_name;
				std::chrono::system_clock::now();
			}
		};
	}

	inline void functionTimerFunc(const char* func_name)
	{

	}
}
