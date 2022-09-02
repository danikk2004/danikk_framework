#pragma once

#if defined(__linux__) || defined(linux) || defined(unix)
	#if defined(LP64) || defined(_LP64) || defined(__LP64__) || defined(_M_IA64)
		#define IS_64BIT 1
	#else
		#define IS_32BIT 1
	#endif
	#define IS_LINUX 1
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(__MINGW32__)
	#define IS_WINDOWS 1
#else
	//Неизвестная ОС
	#error unknown OS
#endif
