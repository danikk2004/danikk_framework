#include <danikk_framework/file_stream.h>
#include <danikk_framework/string.h>
#include <danikk_framework/filesystem.h>
#include <danikk_framework/errno.h>
#include <filesystem>

using namespace std::filesystem;
using namespace std;

#define DEFINE_BASE(CLASS_NAME)\
void CLASS_NAME::open(const String& filePath){open(filePath.c_string());}\
void CLASS_NAME::open(PathBuffer& filePath){ open(filePath.c_string());}

#define SET_ERROR_IF_NULL_FILE() if(filePtr == NULL) error = errno; errno = 0;

namespace danikk_framework
{
	FileStream::~FileStream()
    {
		close();
    }

    long FileStream::tell()
    {
        return ftell(filePtr);
    }

    void FileStream::seek(long offset, int mode)
    {
        fseek(filePtr, offset, mode);
    }

    void FileStream::set(long offset)
    {
        fseek(filePtr, offset, SEEK_SET);
    }

    void FileStream::move(long offset)
    {
        fseek(filePtr, offset, SEEK_CUR);
    }

    int FileStream::eof()
    {
    	return feof(filePtr);
    }

    bool FileStream::isOpen()
    {
    	return filePtr != NULL;
    }

    void FileStream::close()
    {
        if (filePtr != NULL)
        {
            fclose(filePtr);
            filePtr = NULL;
    	}
    }

    void FileReader::open(const char* filePath)
    {
        filePtr = fopen(filePath, "r");
        m_size = fileSize(filePath);
        SET_ERROR_IF_NULL_FILE()
    }

    size_t FileReader::size()
    {
    	return m_size;
    }

    void FileWriter::open(const char* filePath)
    {
        filePtr = fopen(filePath, "w");
        SET_ERROR_IF_NULL_FILE()
    }

    void FileWorker::open(const char* filePath, const char* mode)
    {
        filePtr = fopen(filePath, mode);
        SET_ERROR_IF_NULL_FILE()
    }

    void FileWorker::open(const String& filePath, const char* mode)
    {
    	open(filePath.c_string(), mode);
    }

    void FileWorker::open(PathBuffer& filePath, const char* mode)
    {
    	open(filePath.c_string(), mode);
    }

    DEFINE_BASE(FileReader)
	DEFINE_BASE(FileWriter)
}
