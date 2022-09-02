#ifndef DANIKK_PLATFORM_FILE_STREAM_H
#define DANIKK_PLATFORM_FILE_STREAM_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/stream.h>
#include <danikk_framework/string.h>
#include <danikk_framework/number.h>
#include <danikk_framework/path_buffer.h>
#include <danikk_framework/string_buffer.h>
#include <danikk_framework/errno.h>
#include <filesystem>

namespace danikk_framework
{
    //Общий класс для файловых потоков.
    class FileStream
    {
    protected:
        FILE* filePtr = NULL;
        errno_t error;
    public:
        //Собщает позицию в файле
        long tell();

        //Переместить позицию файла.
        void seek(long offset, int mode);

        //Переместить позицию файла на указанную позицию.
        void set(long offset);

        //Сдвинуть указатель относительно текущего положения потока.
        void move(long offset);

        int eof();

        bool isOpen();

        void close();

        FileStream() = default;

        ~FileStream();

        FileStream(const FileStream&) = delete;

        void operator=(const FileStream&) = delete;
    };

    //Поток чтения из файла.
    class FileReader : public FileStream, public InputStream
    {
    private:
        size_t m_size = 0;
    public:
        void open(const char* filePath);

        void open(const String& filePath);

        void open(PathBuffer& filePath);

        FileReader() = default;

        template<class pathT> FileReader(pathT& path)
		{
        	open(path);
		}

        size_t size();

        template <class valueT> void read(valueT& output)
        {
        	if constexpr(is_number<valueT>::value)
			{
        		fwrite(&output, 1, sizeof(valueT), filePtr);
			}
        	else
        	{
        		output->readFrom(*this);
        	}
        }

        template <class valueT> void read(valueT* destination, size_t size)
        {
        	if constexpr(is_number<valueT>::value)
			{
        		fread(destination, size, sizeof(valueT), filePtr);
			}
        	else
        	{
        		valueT* end = destination + size;
        		while(destination < end)
        		{
        			destination->readFrom(*this);
        			destination++;
        		}
        	}
        }
    };

    //Поток записи в файл.
    class FileWriter : public FileStream, public OutputStream
    {
    public:
        void open(const char* filePath);

        void open(const String& filePath);

        void open(PathBuffer& filePath);

        FileWriter() = default;

        template<class pathT> FileWriter(pathT& path)
		{
        	open(path);
		}

        template <class valueT> void write(const valueT* source, size_t size)
        {
        	if constexpr(is_number<valueT>::value)
			{
        		fwrite(source, size, sizeof(valueT), filePtr);
			}
        	else
        	{
        		const valueT* end = source + size;
        		while(source < end)
        		{
        			source->writeTo(*this);
        			source++;
        		}
        	}
        }

        template <class valueT> void write(const valueT& input)
        {
        	if constexpr(is_number<valueT>::value)
			{
				write((const char*)&input, sizeof(valueT));
			}
        	else
        	{
        		input.writeTo((FileWriter&)*this);
        	}
        }
    };

    //Поток записи и чтения из файла.
    class FileWorker : public FileStream, public IOStream
	{
    public:
        void open(const char* filePath, const char* mode);

        void open(const String& filePath, const char* mode);

        void open(PathBuffer& filePath, const char* mode);

        FileWorker() = default;

        template<class pathT> FileWorker(pathT& path):FileWorker(path, "r+"){}

        template<class pathT, class modeT> FileWorker(pathT& path, const char* mode)
		{
        	open(path, mode);
		}

        void write(const char* source, size_t size);

        void read(char* destination, size_t size);
    };
}

#endif
