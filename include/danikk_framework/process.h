#pragma once

#include <iostream>
#include <cstdio>

#include <danikk_framework/filestream.h>

namespace danikk_framework
{
	class Process
	{
		FILE* pipe;

		Process(const char* command)
		{
			pipe = popen(command, "rw");
		}

		void send(const char* command)
		{
			fwrite(command, strlen(command), sizeof(char), pipe);
		}
	};
}
