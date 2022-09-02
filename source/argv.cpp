#include <danikk_framework/argv.h>

namespace danikk_framework
{
	extern const char* executablePath;

	namespace argv
	{
		static size_t remainingArgv;
		static char** currentArgv;

		void init(int argc,char** argv)
		{
			executablePath = *argv;
			remainingArgv = argc - 1;
			currentArgv = argv + 1;
		}

		char* next()
		{
			char* result = *currentArgv;
			remainingArgv--;
			currentArgv++;
			return result;
		}

		size_t remaining()
		{
			return remainingArgv;
		}
	}
}
