
#include <danikk_framework/misc/main.h>
#include <danikk_framework/argv.h>
#include <danikk_framework/exception.h>
#include <danikk_framework/log.h>

#include <exception>

using namespace danikk_framework;

int main(int argc, char** argv)
{
	ARGV_INIT();
	try
	{
		int result = danikk_framework::main();
		return result;
	}
	catch (Exception& exception)
	{
		formatLogFatal("exception code:%", (uint64)exception.errorCode());
		return exception.errorCode();
	}
	catch (std::exception& exception)
	{
		formatLogFatal("exception:%", exception.what());
		return -1;
	}
	catch (String& exception)
	{
		formatLogFatal("exception:%", exception);
		return -1;
	}
	catch (...)
	{
		logFatal("fatal error!");
		return -1;
	}

    return EXIT_SUCCESS;
}
