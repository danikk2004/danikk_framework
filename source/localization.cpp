#include<danikk_framework/localization.h>
#include<danikk_framework/memory.h>
#include<danikk_framework/dword_string.h>

#define _(key, value) { key, value },

namespace danikk_framework
{
	const char* current_lang = "en";

	static String nullstring = "nullstring";

	Localization localization
	{
		_("debug_max_time","max time: ")
		_("debug_min_time","min time: ")
		_("debug_average_time","average time: ")
		_("error/call_argv_init", "Call danikk_framework::argv::init")
		_("error/cant_find_localization", "Cant find localization ")
	};

	Localization::Localization(const std::initializer_list<LocalizationPair>& data)
	{
		for(const LocalizationPair& pair : data)
		{
			m_data.insert(pair.key, pair.value);
		}
	}

	void Localization::loadFromText(const char* data)
	{

	}

	const String& Localization::operator()(const char* key)
	{
		const String& result = m_data.get(key);

		if(isNullRef(result))
		{
			cerr << localization("error/cant_find_localization") << endl;
			return nullstring;
		}
		else
		{
			return result;
		}

	}
}
