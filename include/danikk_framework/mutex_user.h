#pragma once

#include <danikk_framework/string.h>
#include <mutex>

namespace danikk_framework
{
	class MutexUser
	{
	private:

		std::mutex* ptr;

	public:
		MutexUser(std::mutex& ref)
		{
			ptr = &ref;
			ptr->lock();
		}

		~MutexUser()
		{
			ptr->unlock();
		}
	};
}
