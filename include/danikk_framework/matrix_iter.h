#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/assert.h>
#include <glm/glm.hpp>

namespace danikk_framework
{
	using glm::uvec2;

	#define ITERATE_MATRIX(iterator, matrix) \
			for(uvec2 iterator = uvec2(0);\
			isMatrixIteration(iterator, matrix.size());\
			incrementMatrixIndex(iterator, matrix.size()))

	static inline void incrementMatrixIndex(uvec2& cur, const uvec2& size)
	{
		cur.x++;
		if(cur.x == size.x)
		{
			cur.y++;
			cur.x = 0;
		}
	}

	static inline bool isMatrixIteration(uvec2& cur, const uvec2& size)
	{
		return cur.x != size.x && cur.y != size.y;
	}
}
