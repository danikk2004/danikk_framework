#ifndef DANIKK_PLATFORM_KEY_PARSER_H
#define DANIKK_PLATFORM_KEY_PARSER_H

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	/*
	 * Это класс для разбивания ключа на элементы.
	 * Критерий для отсечения элемента следующие:
	 * 1. текущий символ равен нижнему подчёркиванию;
	 * 2. размер проверяемого куска равен размеру куска ключа (sizeof(chunkT), 8 байт):
	 * 3. размер проверяемого куска равен оставшейся длинне ключа.
	 *
	 * Например возьмём строку petya/aaaaaaaaa/123
	 *
	 * После правильного разбития будут следующие данные:
	 *  1. isNewCatalog = false	"petya"
	 *  2. isNewCatalog = true	"aaaaaaaa"
	 *  3. isNewCatalog = false	"a"
	 *  4. isNewCatalog = true	"123"
	 */
	class CatalogedMapKeyParser
	{
	public:
		typedef uint64 chunkT;
	private:
		const char* currentPtr;
		size_t remainingSize;

		const chunkT* popKey(size_t size);

		chunkT popUnallignedChunk(size_t size);
	public:
		CatalogedMapKeyParser(const char* currentPtr, size_t remainingSize);

		//Если следующий кусок ключа будет являться новым словом, то возвращает true (первое слово-исключение),
		//если кусок будет принадлежать слову предыдущего куска, то вернёт false.
		//Это функцию нужно ВСЕГДА вызывать перед next.
		bool isNewCatalog();

		//Берёт кусок ключа и возвращает его.
		chunkT next();

		bool end();
	};
}

#endif
