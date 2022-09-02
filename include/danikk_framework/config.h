#ifndef DANIKK_PLATFORM_CONFIG_H
#define DANIKK_PLATFORM_CONFIG_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/named_resource_map.h>


namespace danikk_framework
{
	class BinaryConfig
	{
	private:
		NamedResourceMap<char*> m_data;
	public:
		BinaryConfig(){}

		BinaryConfig(const char* filename)
		{

		}

        void readFromBuffer(MemoryStream& input)
        {
        	FixedArray<char, 64> keyBuffer;
        	FixedArray<char, 64> valueBuffer;
        	while(input.)
        	{

        	}
        	while(true)
        	{

        	}
        }

        void writeToBuffer(MemoryStream& output)
        {

        }
	};
}
