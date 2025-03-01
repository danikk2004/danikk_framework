#pragma once
#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/log.h>
#include <danikk_framework/number.h>
#include <assert.h>
#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>

namespace danikk_framework
{
	using glm::uvec2;
	using glm::vec2;

	LOG_OPERATOR(glm::uvec2 data)
	{
		formatWrite(out, "%,%", data.x, data.y);
		return out;
	}

	LOG_OPERATOR(glm::vec2 data)
	{
		formatWrite(out, "%,%", data.x, data.y);
		return out;
	}

	LOG_OPERATOR(glm::vec3 data)
	{
		formatWrite(out, "%,%", data.x, data.y, data.z);
		return out;
	}

	LOG_OPERATOR(glm::ivec2 data)
	{
		formatWrite(out, "%,%", data.x, data.y);
		return out;
	}

	inline glm::ivec2 ivec2_from_string(const String& data)
	{
		String first;
		String second;
		const char* space_ptr = data.find(' ');
		if(space_ptr == NULL)
		{
			return glm::ivec2(0, 0);
		}
		data.split(space_ptr, first, second);
		return glm::ivec2(parseNumber<int>(first), parseNumber<int>(second));
	}

	template<class vec_type> inline bool betweenByAxis(vec_type value, vec_type p1, vec_type p2)
	{
		bool x_axis = value.x == p1.x && value.x == p2.x;
		bool y_axis = value.y == p1.y && value.y == p2.y;
		if(x_axis)
		{
			return (value.x > p1.x && value.x < p2.x) ||
				(value.x > p2.x && value.x < p1.x);
		}
		else if(y_axis)
		{
			return (value.y > p1.y && value.y < p2.y) ||
				(value.y > p2.y && value.y < p1.y);
		}
		else
		{
			return false;
		}
	}

	template<class vec_type> inline bool betweenByDiag(vec_type value, vec_type p1, vec_type p2)
	{
		typedef int32 tmp_type;
		vec_type abs_delta1 =
				vec_type(::abs((tmp_type)value.x - (tmp_type)p1.x),
						::abs((tmp_type)value.y - (tmp_type)p1.y));
		vec_type abs_delta2 =
				vec_type(::abs((tmp_type)value.x - (tmp_type)p2.x),
						::abs((tmp_type)value.y - (tmp_type)p2.y));
		if((abs_delta1.x != abs_delta1.y) || (abs_delta2.x != abs_delta2.y))
		{
			return false;
		}
		bool by_x = (value.x > p1.x && value.x < p2.x) ||
					(value.x > p2.x && value.x < p1.x);
		bool by_y = (value.y > p1.y && value.y < p2.y) ||
					(value.y > p2.y && value.y < p1.y);
		return by_x && by_y;
	}

	template<class vec_type> vec_type::value_type squareLen(vec_type v1, vec_type v2)
	{
		return abs(v1.x - v2.x) + abs(v1.y - v2.y);
	}

	template<class vec2, class val> bool inRange(const vec2& range, const val& value)
	{
		return (value > range.x) && (value < range.y);
	}

	template<class vec2> vec2 index2Pos(int index, size_t size)
	{
		return vec2(index % size, index / size);
	}
}
