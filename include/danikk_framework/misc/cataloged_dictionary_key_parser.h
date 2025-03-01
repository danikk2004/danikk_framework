#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/memory.h>

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
	template<char splitter> class CatalogedDictionaryKeyParser
	{
	public:
		typedef uint64 chunkT;
	private:
		const char* currentPtr;
		size_t remainingSize;

		const chunkT* popKey(size_t size)
		{
			const char* lastPtr = currentPtr;
			currentPtr += size;
			remainingSize -= size;
			return (chunkT*)lastPtr;
		}

		chunkT popUnallignedChunk(size_t size)
		{
			chunkT value = 0;
			memcpy(&value, popKey(size), size);
			return value;
		}
	public:
		CatalogedDictionaryKeyParser(const char* currentPtr, size_t remainingSize)
		: currentPtr(currentPtr), remainingSize(remainingSize){}

		//Если следующий кусок ключа будет являться новым словом, то возвращает true (первое слово-исключение),
		//если кусок будет принадлежать слову предыдущего куска, то вернёт false.
		//Это функцию нужно ВСЕГДА вызывать перед next.
		bool isNewCatalog()
		{
			bool isNewWord = currentPtr[0] == splitter;

			if(isNewWord)
			{
				++currentPtr;
				--remainingSize;
			}

			return isNewWord;
		}

		//Берёт кусок ключа и возвращает его.
		chunkT next()
		{
			for(size_t index = 0;;index++)
			{
				if(currentPtr[index] == splitter)
				{
					return popUnallignedChunk(index);
				}

				size_t currentSizeOfChunk = index + 1;
				if(currentSizeOfChunk == sizeof(chunkT))
				{
					return *popKey(sizeof(chunkT));
				}
				else if(currentSizeOfChunk == remainingSize)
				{
					return popUnallignedChunk(currentSizeOfChunk);
				}
			}
			return 0;
		}

		bool end()
		{
			return remainingSize == 0;
		}
	};
}
