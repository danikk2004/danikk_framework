#include <danikk_framework/filestream.h>
#include <danikk_framework/string.h>
#include <danikk_framework/filesystem.h>
#include <danikk_framework/errno.h>
#include <filesystem>

using namespace std::filesystem;
using namespace std;

namespace danikk_framework
{
	FileStream::~FileStream()
    {
		close();
    }

	void FileStream::open()
	{
        filePtr = fopen(path.c_string(), flag);
        checkSize();
	}

    void FileStream::checkSize()
    {
        if(filePtr != NULL)
        {
        	size_t tmp = tell();
        	end();
            m_size = tell();
            set(tmp);
        }
    }

    size_t FileStream::writeNull(size_t size)
    {
		static constexpr size_t null_filler_size = 256;
		char null_filler[null_filler_size]{0};
		size_t res = 0;

    	while(size > null_filler_size)
		{
    		res += write((char*)null_filler, null_filler_size);
			size -= null_filler_size;
		}
    	res += write((char*)null_filler, size);

    	return res;
    }

    long FileStream::tell()
    {
        return ftell(filePtr);
    }

    void eseek(FILE* ptr, long offset, int mode)
    {
        int res = fseek(ptr, offset, mode);
        if(res != 0)
        {
        	res = errno;
        	errno = 0;
        	throw FileException(error_codes::cant_seek, res);
        }
    }

    void FileStream::seek(long offset, int mode)
    {
    	eseek(filePtr, offset, mode);
    }

    void FileStream::set(long offset)
    {
    	eseek(filePtr, offset, SEEK_SET);
    }

    void FileStream::move(long offset)
    {
    	eseek(filePtr, offset, SEEK_CUR);
    }

    void FileStream::end()
    {
    	eseek(filePtr, 0, SEEK_END);
    }

    void FileStream::resize(uint64 new_size)
    {
    	flush();
    	close();
    	truncate(path.c_string(), new_size);
    	open();
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

    int FileStream::flush()
    {
    	return fflush(filePtr);
    }

    size_t FileStream::size()
    {
    	return m_size;
    }
}
