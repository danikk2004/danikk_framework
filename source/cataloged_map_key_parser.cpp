#include <danikk_framework/cataloged_map_key_parser.h>
#include <cstring>

namespace danikk_framework
{
	typedef CatalogedMapKeyParser::chunkT chunkT;

	const chunkT* CatalogedMapKeyParser::popKey(size_t size)
	{
		const char* lastPtr = currentPtr;
		currentPtr += size;
		remainingSize -= size;
		return (chunkT*)lastPtr;
	}

	chunkT CatalogedMapKeyParser::popUnallignedChunk(size_t size)
	{
		chunkT value = 0;
		memcpy(&value, popKey(size), size);
		return value;
	}

	CatalogedMapKeyParser::CatalogedMapKeyParser(const char* currentPtr, size_t remainingSize)
	: currentPtr(currentPtr), remainingSize(remainingSize){}

	bool CatalogedMapKeyParser::isNewCatalog()
	{
		bool isNewWord = currentPtr[0] == '/';

		if(isNewWord)
		{
			++currentPtr;
			--remainingSize;
		}

		return isNewWord;
	}

	//Берёт кусок ключа и возвращает его.
	chunkT CatalogedMapKeyParser::next()
	{
		for(size_t index = 0;;index++)
		{
			if(currentPtr[index] == '/')
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

	bool CatalogedMapKeyParser::end()
	{
		return remainingSize == 0;
	}
}
