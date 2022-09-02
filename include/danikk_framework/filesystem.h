#ifndef DANIKK_PLATFORM_FILE_FUNCTIONS_H
#define DANIKK_PLATFORM_FILE_FUNCTIONS_H

#include <danikk_framework/cstring.h>
#include <danikk_framework/current_system.h>
#include <danikk_framework/dynamic_array.h>
#include <danikk_framework/file_stream.h>
#include <danikk_framework/memory_stream.h>
#include <danikk_framework/number.h>
#include <danikk_framework/path_buffer.h>
#include <danikk_framework/string_buffer.h>
#include <danikk_framework/string.h>
#include <danikk_framework/current_system.h>
#include <danikk_framework/errno.h>

#if IS_LINUX
	#include <sys/stat.h>
	#include<unistd.h>
#else
	#error not implemented
#endif

namespace danikk_framework
{
	template<class pathT> void readText(pathT& path, String& output)
	{
        FileReader reader(path);
        if(!reader.isOpen())
		{
        	return;
		}
		size_t fileSize = reader.size();
		if(fileSize == 0)
		{
			output.clear();
		}
		else
		{
			output.resize(fileSize);
			reader.read(output.data(), fileSize);
		}
	}

	template<class pathT> void readLines(pathT& path, DynamicArray<String>& output)
	{
        FileReader reader(path);
        if(!reader.isOpen())
		{
        	return;
		}
		DynamicArray<char> data;
		size_t fileSize = reader.size();
		data.resize(fileSize);
		reader.read(data.data(), fileSize);
		data.cutElements('\r');
		char* currentCharPtr = data.data();
		char* endPtr = data.end();
		const char* lastStringStartPtr = currentCharPtr;

		while(currentCharPtr < endPtr)
		{
            char currentChar = *currentCharPtr;
            if(currentChar == '\n')
            {
                *currentCharPtr = '\0';
            	output.push_move(String(lastStringStartPtr, (size_t)currentCharPtr - (size_t)lastStringStartPtr));
            	lastStringStartPtr = (currentCharPtr) + 1;
            }
            ++currentCharPtr;
		}
	}

	template<class pathT> void readBytes(pathT& path, MemoryStream& output)
	{
		FileReader reader(path);
        if(!reader.isOpen())
		{
        	return;
		}
		size_t size = reader.size();
		output.reserve(size);
		reader.read(output.used(), size);
		output.skip(size);
	}

	template<class pathT> void saveText(pathT& path, String& data)
	{
		FileWriter writer(path);
        if(!writer.isOpen())
		{
        	return;
		}
		writer.write(data.data(), data.size());
	}

	template<class pathT> void saveLines(pathT& path, DynamicArray<String>& data)
	{
		FileWriter writer(path);
        if(!writer.isOpen())
		{
        	return;
		}
		for(String& currentString: data)
		{
			writer.write(currentString.data(), currentString.size());
			writer.write('\n');
		}
	}

	template<class pathT> bool exits(pathT& path)
	{
		int result = ::access(tocstring(path), F_OK);
		errno = 0;//access меняет errno после неудачи.
		return result == 0;
	}

	template<class pathT> errno_t createDirectory(pathT& path)
	{
		#ifdef IS_LINUX
			if(mkdir(tocstring(path), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH ) == 0)
			{
				return 0;
			}
		#else
			#error not implemented
		#endif
		RETURN_ERRNO
	}

	template<class pathT> size_t createFile(pathT& path)
	{
		FILE* filePtr = fopen(tocstring(path), "w");
		if(filePtr)
		{
			fclose(filePtr);
			return 0;
		}
		else
		{
			RETURN_ERRNO
		}
	}

	template<class pathT> errno_t remove(const pathT& path)
	{
		if(::remove(tocstring(path)))
		{
			RETURN_ERRNO
		}
		return 0;
	}

	template<class firststr, class secondstr> errno_t rename(firststr& oldPath, secondstr& newPath)
	{
		if(::rename(tocstring(oldPath), tocstring(newPath)))
		{
			RETURN_ERRNO
		}
	}

	template<class pathT> bool isDirectory(pathT& path)
	{
		#if IS_LINUX
			struct stat st;
			stat(tocstring(path), &st);
			return S_ISDIR(st.st_mode);
		#else
			#error not implemented
			return 0;
		#endif
	}

	template<class pathT> uint64 fileSize(pathT& path)
	{
		struct stat st;
		stat(tocstring(path), &st);
		return st.st_size;
	}

	//Получает путь к папке со всеми временными файлами приложения.
	const char* getTempDirectory();

	//Возвращяет имя с расширением.
	void getFullNameFromPath(const char* path, String& result);

	void getFullNameFromPath(const String& path, String& result);

	//Возвращает имя без расширения.
	void getNameFromPath(const char* path, String& result);

	void getNameFromPath(const String& path, String& result);

	const char* getExtensionFromPath(const char* path);

	//Инициализирует директорию с временными файлами.
	void initTempDirectory(const char* name);
}

#endif
