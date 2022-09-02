#include <danikk_framework/math.h>

namespace danikk_framework
{
	size_t log2(size_t x)
	{
		size_t result = 0;
		while(x != 0 )
		{
			result++;
			x >>= 1;
		}
		return result;
	}

	size_t powOf2(size_t x)
	{
		return 1ULL << x;
	}

	size_t alignToBigger2Pow(size_t x)
	{
		return 1ULL << (log2(x) + 1);
	}

	size_t alignTo(size_t x, size_t y)
	{
		return x - x % y + y;
	}

	int divUp(int x, int y)
	{
		return x / y + 1;
	}
}
