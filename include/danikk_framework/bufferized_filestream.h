#pragma once
#include<danikk_framework/filestream.h>
#include<danikk_framework/memory_buffer.h>
#include<danikk_framework/log.h>

namespace danikk_framework
{
	class BufferizedFileStream : public FileStream
	{
	protected:
		MemoryBuffer buffer;
		size_t offset = 0;
		static constexpr size_t buffer_capacity = 4096;

		void flushBuffer();
	public:
		BufferizedFileStream();

        int eof()
        {
        	return buffer.size() < buffer_capacity && offset == buffer.size();
        }

        template<class value_t> void write(const value_t* source, size_t size = sizeof(value_t))
    	{
        	if(size > buffer.capacity() - buffer.size())
        	{
        		flushBuffer();
        		FileStream::write(source, size);
        	}
        	else
        	{
        		buffer.write(source, size);
        	}
    	}

        template<class value_t> void read(value_t* destination, size_t size = sizeof(value_t))
    	{
        	if(offset + size >= buffer.size())
        	{
            	if(offset < buffer.size())
            	{
            		memcpy(buffer.data(), buffer.data() + offset, buffer.size() - offset);		//перемещает "недочитанный" кусок в начало буффера
            		offset = buffer.size() - offset;											//устанавливает смещение на конец куска в начале буффера
            	}
        		size_t read_size = FileStream::read(buffer.data(), buffer.size() - offset);	//дочитывает значение из файла
        		buffer.resize(read_size + offset);											//изменяет размер буффера в зависимости от прочинанного значения
        		offset = 0;																	//устанавливает смещение на начало буффера
        	}
        	else if(buffer.size() == 0)
        	{
        		buffer.resize(FileStream::read(buffer.data(), buffer_capacity));
        	}
        	if(buffer.size() == 0)
        	{
        		logError("FileStream::read at buffer.size() == 0");
        	}
        	if constexpr(sizeof(value_t) == 1)
        	{
        		*destination = *(buffer.data() + offset);
        		offset++;
        	}
        	else
        	{
            	memcpy(destination, buffer.data() + offset, size);
            	offset += size;
        	}
    	}

        template <class valueT> BufferizedFileStream& operator<<(const valueT& input)
        {
        	BufferizedFileStream::write(&input, 1);
        	return *this;
        }

        template <class valueT> BufferizedFileStream& operator>>(valueT& output)
        {
        	BufferizedFileStream::read(&output, 1);
        	return *this;
        }

        void flush();
	};

	class BufferizedFileWriter : public BufferizedFileStream
	{
	public:
		BufferizedFileWriter() = default;

		DEF_OPEN
        {
        	FileStream::open(path, "w");
        }
	};

	class BufferizedFileReader : public BufferizedFileStream
	{
	public:
		BufferizedFileReader() = default;

		DEF_OPEN
        {
        	FileStream::open(path, "r");
        }
	};

	class LineFileWriter : public FileStream
	{
		String line_buffer;
		uint32 line_capacity = 1;
		uint32 line_counter = 0;
	public:
		LineFileWriter();

		void setLineCapacity(uint32);

		DEF_OPEN
        {
        	FileStream::open(path, "w");
        }

		bool hasBeenFlushed();

		LineFileWriter& operator<<(char chr);

		LineFileWriter& operator<<(const char* str);
	};

	class LineFileReader : public FileStream
	{
		MemoryBuffer buffer;
		index_t buffer_offset = 0;
	public:
		LineFileReader();

		DEF_OPEN
        {
        	FileStream::open(path, "r");
        }

        int eof();

		bool read(String& str);
	};
}
