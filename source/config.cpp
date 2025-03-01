#include <danikk_framework/config.h>
#include <danikk_framework/cataloged_dictionary.h>
#include <danikk_framework/filesystem.h>
#include <danikk_framework/misc/line_getter.h>
#include <danikk_framework/cstring_functions.h>
#include <danikk_framework/localization.h>
#include <danikk_framework/log.h>

namespace danikk_framework
{
	void openData(const char* data, size_t size);

	void Config::openData(const String& data)
	{
		openData(data.data(), data.size());
	}

	void Config::openData(const char* data, size_t size)
	{
		const char* curstr = data;
		const char* end = data + size;
		String key;
		String value;
		bool is_key_parsing = true;
		while(true)
		{
			while(*curstr == '#' || (is_key_parsing && *curstr == '\n'))
			{
				curstr = strskiptochar(curstr, '\n');
				curstr++;
			}
			if(is_key_parsing)
			{
				if(*curstr == '=')
				{
					is_key_parsing = false;
				}
				else
				{
					key << *curstr;
				}
			}
			else
			{
				if(*curstr == '\n' || curstr >= end)
				{
					is_key_parsing = true;
					m_data.insert(key, value);
					key.clear();
					value.clear();
					if(curstr >= end)
					{
						break;
					}
				}
				else
				{
					value << *curstr;
				}
			}
			curstr++;
		}
		m_opened = true;
	}

	void Config::openFile(const char* filename)
	{
		String data;
		FileReader reader;
		reader.open(filename);
		if(!reader.isOpen())
		{
			formatLogInfo("!reader.isOpen() : %", filename);
		}
		data.resize(reader.size());
		reader.read(data.data(), data.size());
		data.cutChars('\r');
		openData(data);

	}
}
