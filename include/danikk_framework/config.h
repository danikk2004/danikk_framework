#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/cataloged_dictionary.h>
#include <danikk_framework/dictionary.h>
#include <danikk_framework/cstring_functions.h>
#include <danikk_framework/number.h>
#include <danikk_framework/localization.h>
#include <danikk_framework/log.h>

namespace danikk_framework
{
	class Config
	{
	private:
		StringDictionary<String> m_data;
		uint m_opened = 0;
	public:
		Config() = default;

		void openFile(const char* filename);

		void openData(const String& data);

		void openData(const char* data, size_t size);

		template<class key_t, class value_t> void set(const key_t& key, const value_t& value)
		{
			m_data.insert(key, value);
		}

		template<class key_t> bool exits(const key_t& key)
		{
			return m_data.get(key) != NULL;
		}

		template<class ret_t> ret_t get(const String& key)
		{
			const String* res = m_data.get(key);
			if(res == NULL)
			{
				res = m_data.get(key);
				if(res != NULL)
				{
					goto res_is_find;
				}
				formatLogWarning("Config::get key:% res == NULL", key);
				abort();
				if constexpr(is_number<ret_t>::value)
				{
					return 0;
				}
				else
				{
					return (const char*)NULL;
				}
			}
			res_is_find:
			if constexpr(is_number<ret_t>::value)
			{
				return parseNumber<ret_t>(*res);
			}
			else
			{
				return *res;
			}
		}
	};
}
