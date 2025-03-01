#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>

namespace danikk_framework
{
	bool inline parseToken(index_t& i, String& src, String& dest)
	{
		dest.clear();
		while(src[i] == ' ')
		{
			i++;
			if(i == src.size())
			{
				return false;
			}
		}
		while(src[i] != ' ')
		{
			if(i == src.size())
			{
				if(dest.size() == 0)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			dest << src[i];
			i++;
		}
		return true;
	}
}
