#pragma once

#include <cstddef>
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;
using std::ostream;

#define IS_DEBUG not NDEBUG

#define IS_RELEASE NDEBUG

#define CUR_LINE __FILE__ << ":" << __LINE__ << ' '

#define ALIGNED_STRUCT(BYTE_COUNT) __attribute__ ((aligned(BYTE_COUNT)))

namespace danikk_framework
{
	typedef int8_t int8;
	typedef uint8_t uint8;
	typedef uint8_t byte;
	typedef int16_t int16;
	typedef uint16_t uint16;
	typedef int32_t int32;
	typedef uint32_t uint32;
	typedef int64_t int64;
	typedef uint64_t uint64;
	typedef size_t error_code;

	class MemoryStream;
}

using error_code = danikk_framework::error_code;
