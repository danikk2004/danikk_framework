#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/localization.h>
#include <danikk_framework/trace_sigsegv.h>
#include <execinfo.h>
#include <csignal>

namespace danikk_framework
{
	static void tracer(int)
	{
		void* callstack[256];
		size_t frame_count = backtrace(callstack, 256);
		char** stack_trace_array = backtrace_symbols(callstack, frame_count);
		for(size_t i = 0; i < frame_count; i++)
		{
			cout << stack_trace_array[i];
		}
	}

	void trace_sigsegv()
	{
		std::signal(SIGSEGV, tracer);
	}
}
