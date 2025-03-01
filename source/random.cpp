#include <danikk_framework/random.h>
#include <danikk_framework/thread.h>

#undef random

namespace danikk_framework
{
	void* Random::random_source;
	Random default_random;
	Mutex raw_block_mutex;

	void Random::init()
	{
		if(random_source == NULL)
		{
			#if IS_LINUX
				random_source = (void*)fopen("/dev/random", "r");
			#elif
				#error not implemented
			#endif
		}

	}

	void Random::rawBlock(char* data, size_t size)
	{
	#if IS_LINUX
		fread(data, size, 1, (FILE*)random_source);
	#elif
		#error not implemented
	#endif
	}

	void Random::read(char* destination, size_t size)
	{
		assert(size <= buffer_capacity);
		if(buffer_capacity < size + buffer_used)
		{
			uint64 random_key;
			USE_MUTEX(raw_block_mutex)
			{
				rawBlock((char*)&random_key, sizeof(uint64));
			}
			for(buffer_used = 0; buffer_used < buffer_capacity; buffer_used += sizeof(uint64))
			{
				char* buffer_ptr = (char*)&buffer;
				buffer_ptr += buffer_used;
				*(uint64*)buffer_ptr = random_key;
				random_key = random_key * 34526662624031 + 693857152369187;
			}
			buffer_used = 0;
		}
		memcpy(destination, (char*)&buffer + buffer_used, size);
		buffer_used += size;
	}

	bool random_booler::next()
	{
		if(bool_count == 0)
		{
			bool_count = 63;
			random.read((char*)&saved_num, sizeof(uint64));
		}
		bool res = saved_num & 1;
		saved_num >>= 1;
		bool_count--;
		return res;
	}
}
