#pragma once
#include<errno.h>
#include<danikk_framework/exception.h>

namespace danikk_framework
{
	typedef int errno_t;

	/*class ErrnoException : public Exception
	{
	private:
		uint32 error_code;
	public:
		ErrnoException(const char* message , uint32 error_code) : Exception(message)
		{
			this->error_code = error_code;
		}

		uint32 code()
		{
			return error_code;
		}
	};*/

	#define ERRNO_CALL(CALL)\
		errno = 0;\
		CALL;\
		if(errno)\
		{\
			errno_t errno_res = errno;\
			errno = 0;\
			return errno_res;\
		}
}

using danikk_framework::errno_t;
