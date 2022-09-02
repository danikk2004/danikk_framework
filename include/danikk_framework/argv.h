#ifndef DANIKK_PLATFORM_ARGV_H
#define DANIKK_PLATFORM_ARGV_H

#include<danikk_framework/danikk_framework.h>
#include<danikk_framework/path_buffer.h>

namespace danikk_framework
{
	namespace argv
	{
		void init(int argc,char** argv);

		char* next();

		size_t remaining();
	}
}

#endif
