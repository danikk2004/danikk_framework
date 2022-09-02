#ifndef DANIKK_PLATFORM_CMD_H
#define DANIKK_PLATFORM_CMD_H

#include <danikk_framework/string_buffer.h>
#include <danikk_framework/string.h>
#include <danikk_framework/filesystem.h>
#include <danikk_framework/file_stream.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/number.h>
#include <thread>

namespace danikk_framework
{
	namespace internal
	{
		#if IS_LINUX
			inline const char* command_null_output = " 2>/dev/null";
		#else
			#error not implemented
		#endif

        extern thread_local String commandBuffer;

		template<class valueT> static void writeCommandArgToBuffer(valueT argument)
		{
			if constexpr(is_number<valueT>::value)
			{
                commandBuffer << (char)' ' << argument;
			}
			else
			{
				commandBuffer << " \'" << argument << "\'";
			}
		}

		inline static void writeArgs()
		{
			(void)commandBuffer;
		}

		template<class firstT,class... Types> inline static void writeArgs(firstT arg,Types... args)
		{
			writeCommandArgToBuffer(arg);
			writeArgs(args...);
		}
	}

	template<class firstT,class... Types> void executeCommandWithoutResults(firstT command,Types... args)
	{
        using namespace internal;
		commandBuffer << command;
		writeArgs(args...);
		system(commandBuffer.c_string());
		commandBuffer.clear();
	}

	template<class firstT,class... Types> void executeCommand(String& result, firstT command, Types... args)
	{
        using namespace internal;

		PathBuffer tempFilePathBuffer;
		tempFilePathBuffer.setPath(getTempDirectory());
		StringBuffer<128> tempFileName;
		tempFileName
		<< "temp_cmd_"
		<< (size_t)&commandBuffer//указатель для commandBuffer у разных потоков свой.
		<< ".dat";
		tempFilePathBuffer.setFileName(tempFileName);

		commandBuffer << command;
		writeArgs(args...);
		commandBuffer << " >" << tempFilePathBuffer.c_string();
		commandBuffer << command_null_output;

        system(commandBuffer.c_string());

		internal::commandBuffer.clear();

		readText(tempFilePathBuffer, result);
		remove(tempFilePathBuffer);
	}
}

#endif
