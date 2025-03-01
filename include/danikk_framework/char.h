#pragma once

namespace danikk_framework
{
	inline bool charIsBetween(char chr, char start, char end)
	{
		return chr >= start && chr <= end;
	}

	inline bool isNumber(char chr)
	{
		return chr >= '0' && chr <= '9';
	}
}
