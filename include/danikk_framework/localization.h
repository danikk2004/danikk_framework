#ifndef DANIKK_PLATFORM_LOCALIZATION_H
#define DANIKK_PLATFORM_LOCALIZATION_H

#include<danikk_framework/danikk_framework.h>
#include<danikk_framework/cataloged_map.h>
#include<initializer_list>

namespace danikk_framework
{
	class Localization
	{
	private:
		CatalogedMap<String> m_data;
	public:
		//Конструктора без аргументов нету, так как нужно явно указывать словарь со значениями по умолчанию на английском.

		struct LocalizationPair
		{
			const char* key;
			const char* value;
		};

		Localization(const std::initializer_list<LocalizationPair>& data);

		void loadFromText(const char* data);

		const String& operator()(const char* key);
	};

    //То есть Localization это класс локализации,
    //localization это объект со всеми строками фреймворка.
	//В каждом пространстве имён пусть будет свой объект
	//Localization с именем localization.
	//Что бы везде можно было бы просто ввести localization("ключ").
	extern Localization localization;

    //Строка текущей локализации.
    //По умолчанию ставится в английский.
	extern const char* current_lang;
}

#endif
