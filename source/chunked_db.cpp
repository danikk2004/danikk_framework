#include <danikk_framework/misc/chunked_db.h>
#include <danikk_framework/filesystem.h>

namespace danikk_framework
{
	void ChunkedDB::open()
	{
		if(file.isOpen())
		{
			size_t file_size = file.size();
			assert(file_size % chunk_size == 0);
			chunk_count = file_size / chunk_size;
			if(chunk_used_size == 0)
			{
				chunk_used_size = chunk_size;
			}
		}
	}

	void ChunkedDB::setChunkSize(uint32 value)
	{
		chunk_size = value;
	}

	uint32 ChunkedDB::chunkCount()
	{
		return chunk_count;
	}

	size_t ChunkedDB::read(char* data, index_t chunk_index, size_t count)
	{
		assert(chunk_index < chunk_count);
		file.set(chunk_index * chunk_size);
		size_t read_size = file.read(data, chunk_size * count);
		return read_size / chunk_size;
	}

	void ChunkedDB::write(char* data, index_t chunk_index)
	{
		assert(chunk_index < chunk_count);
		file.set(chunk_index * chunk_size);
		file.write(data, chunk_used_size);
	}

	void ChunkedDB::add(char* data)
	{
		chunk_count++;
		file.end();
		file.write(data, chunk_size);
		file.writeNull(chunk_size - chunk_used_size);
	}

	void ChunkedDB::remove(index_t chunk_index, char* buffer)
	{
		if(chunk_index < chunkCount() - 1)					//если нужно удалить блок в конце файла, то просто изменяем размер файла
		{
			assert(read(buffer, chunkCount() - 1, 1) == 1);//иначе перемещаем последний блок поверх удаляемого, а потом изменяем размер файла
			write(buffer, chunk_index);
		}
		file.resize(file.size() - chunk_size);
		chunk_count--;
	}

	void ChunkedDB::flush()
	{
		file.flush();
	}
}
