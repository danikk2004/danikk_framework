#pragma once

#include <danikk_framework/current_system.h>

#if IS_WINDOWS
	#include <windows.h>
#endif

#include <iostream>
#include <cstdint>

using std::swap;

#if not NDEBUG or _DEBUG
	#define IS_DEBUG 1
#else
	#define IS_DEBUG 0
#endif

#define IS_RELEASE not IS_DEBUG
#define ERROR_INDEX __LONG_MAX__
#define ALIGNED_STRUCT

namespace danikk_framework
{
	template<class value_t> inline value_t&& move(value_t& value)
	{
		return (value_t&&)value;
	}

	template<class value_t> inline value_t&& move(value_t&& value)
	{
		return (value_t&&)value;
	}

	template<class value_t> inline void swap(value_t& value1, value_t& value2)
	{
		value_t tmp = move(value1);
		value1 = move(value2);
		value2 = move(tmp);
	}

	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef uint8_t byte;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;

	typedef int8_t bool8;
	typedef int16_t bool16;
	typedef int32_t bool32;
	typedef int64_t bool64;

	typedef size_t index_t;
	typedef size_t count_t;

	template<class T> using InitList = std::initializer_list<T>;

	class String;
	class MemoryBuffer;
}
