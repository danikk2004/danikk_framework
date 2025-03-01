#pragma once

#include <danikk_framework/filestream.h>

namespace danikk_framework
{
	//класс для работы с файлами баз данных, в которых все данные расположены последовательно в чанках одинакового размера
	class ChunkedDB
	{
	private:
		FileStream file;
		uint32 chunk_size;//размер блока данных
		uint32 chunk_count;//количество блоков данных
		uint32 chunk_used_size;

		void open();
	public:
		ChunkedDB() = default;

		DEF_OPEN_F
		{
			file.open(path, "a+");
			open();
		}

		void setChunkSize(uint32);

		uint32 chunkCount();

		size_t read(char* data, index_t chunk_index, size_t count = 1);

		void write(char* data, index_t chunk_index);

		void add(char* data);

		void remove(index_t chunk_index, char* buffer);

		void flush();
	};
}
