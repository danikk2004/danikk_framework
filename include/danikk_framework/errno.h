#pragma once
#include<errno.h>

namespace danikk_framework
{
	typedef size_t errno_t;

	#define RETURN_ERRNO\
		errno_t errno_res = errno;\
		errno = 0;\
		return errno_res;
}

using danikk_framework::errno_t;
