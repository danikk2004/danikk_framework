#include <string>
#include <unistd.h>
#include <cstring>

#include <danikk_framework/path_buffer.h>
#include <danikk_framework/cmd.h>
#include <danikk_framework/string.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/localization.h>

#ifdef IS_LINUX
	//#include <unistd.h>
	//#include <sys/types.h>
	//#include <pwd.h>
#else
#error not implemented
#endif

namespace danikk_framework
{
	static bool cwd_request = true;
	static PathBuffer cwd_buffer;
	static bool desktop_directory_request = true;
	static PathBuffer desktop_directory_buffer;
	static bool executable_directory_request = true;
	static PathBuffer executable_directory_buffer;

	const char* executablePath = "";//Инициализируется в argv.cpp

    PathBuffer::PathBuffer()
    {
        m_abp = m_data;
    }

    PathBuffer::PathBuffer(const String& path)
    {
        memcpy(m_data, path.data(), path.size());
        findabp();
    }

    void PathBuffer::push(const char* data, size_t size)
    {
        memcpy(m_abp, data, size);
        m_abp += size;
    }

    PathBuffer& PathBuffer::operator << (char chr)
    {
    	*m_abp = chr;
    	m_abp++;
    	return *this;
    }

    void PathBuffer::findabp()
    {
        //Ищет слеш, закрывающий директорию, если её нет, то добавляет её.
    	//m_afterBracketSpace становится последним символом после слеша.
        char* currentDataPtr = m_data;
		while (true)
		{
			char currentChar = *currentDataPtr;
			if (currentChar == '\0')
			{
                char previousChar = *(currentDataPtr - 1);
                if(previousChar != pathSlash)
                {
                    *currentDataPtr = pathSlash;
                    ++currentDataPtr;
                    *currentDataPtr = '\0';
                    m_abp = currentDataPtr;
                }
				return;
			}
			if (currentChar == pathSlash)
			{
				m_abp = currentDataPtr + 1;
			}
			currentDataPtr++;
		}
    }

    PathBuffer& PathBuffer::setPath(const char* path)
    {
    	char currentChar;
    	char* dataPtr = m_data;
    	while((currentChar = *path) != '\0')
    	{
    		if(currentChar == pathSlash)
    		{
    			m_abp = dataPtr + 1;
    		}
    		*dataPtr = currentChar;
    		dataPtr++;
    		path++;
    	}
    	*dataPtr = '\0';

    	return *this;
    }

    PathBuffer& PathBuffer::setFileName(const char* name)
    {
		char* tempm_abp = m_abp;
		char currentChar = *name++;
		while(currentChar != '\0')
		{
			*tempm_abp++ = currentChar;
			currentChar = *name++;
		}
		*tempm_abp = '\0';

		return *this;
    }

	PathBuffer& PathBuffer::setFileName(const String& name, const String& extension)
	{
		char* tempm_abp = m_abp;
		size_t nameSize = name.size();
		memcpy(tempm_abp, name.data(), nameSize);
		tempm_abp += name.size();
		*tempm_abp++ = '.';
		size_t extensionSize = extension.size();
		memcpy(tempm_abp, extension.data(), extensionSize);
		tempm_abp += extensionSize;
		*tempm_abp = '\0';

		return *this;
	}

	PathBuffer& PathBuffer::popDirectory()
	{
		--m_abp;//Пропуск null терминатора.
		--m_abp;//пропуск pathSlash.

		while(*m_abp != pathSlash)
		{
			--m_abp;
		}
		++m_abp;
		*m_abp = '\0';//Ставим null терминатор.
		return *this;
	}

    PathBuffer& PathBuffer::getcwd()
    {
    	setPath(danikk_framework::getcwd().c_string());
        return *this;
    }

    PathBuffer& PathBuffer::getDesktopDirectory()
    {
    	setPath(danikk_framework::getDesktopDirectory().c_string());
        return *this;
    }

    PathBuffer& PathBuffer::getExecutableDirectory()
    {
    	setPath(danikk_framework::getExecutableDirectory().c_string());
        return *this;
    }

    const char* PathBuffer::getLastDirectoryName()
    {
    	const char* dirname = m_abp;

    	dirname--;
    	dirname--;

    	while(*dirname != pathSlash)
    	{
    		dirname--;
    	}

    	dirname++;

    	return dirname;
    }

	const char* PathBuffer::c_string() const
	{
		return m_data;
	}

	void PathBuffer::skip(size_t offset)
	{
		m_abp += offset;
	}

	bool PathBuffer::isEmpty() const
	{
		return m_abp == m_data;
	}

	char* PathBuffer::abp()
    {
    	return m_abp;
    }

    char* PathBuffer::data()
    {
    	return m_data;
    }

    const char* PathBuffer::data() const
    {
    	return m_data;
    }

    size_t PathBuffer::size() const
    {
    	const char* end = m_data;
    	while(*end != '\0')
		{
    		end++;
		}
    	return end - m_data;
    }

    char& PathBuffer::operator[](size_t index)
    {
    	return m_data[index];
    }

	char PathBuffer::firstChar() const
	{
		return *m_data;
	}

	char PathBuffer::lastChar() const
	{
		return m_data[size()];
	}

    ostream& operator <<(ostream& cout, const PathBuffer& data)
    {
    	cout << data.c_string();
    	return cout;
    }

    const PathBuffer& getcwd()
    {
    	if(cwd_request)
    	{
    		::getcwd((char*)cwd_buffer.m_data, PathBuffer::defaultCapacity);
    		cwd_buffer.findabp();
    		cwd_request = false;
    	}
		return cwd_buffer;
    }

    const PathBuffer& getDesktopDirectory()
	{
    	if(desktop_directory_request)
    	{
            #if IS_LINUX
                String desktopPath;
                executeCommand(desktopPath, "xdg-user-dir DESKTOP");
                while(desktopPath.lastChar() != '\n')
                {
                	desktopPath.resize(desktopPath.size() - 1);
                }
                desktop_directory_buffer.pushDirectory(desktopPath);
            #else
                #error not implemented
            #endif
            desktop_directory_request = false;
    	}

		return desktop_directory_buffer;
	}

    const PathBuffer& getExecutableDirectory()
    {
    	if(executable_directory_request)
    	{
        	if(*executablePath == '\0')
        	{
        		cerr << localization("error/call_argv_init");
        		abort();
        	}

        	size_t copyLen = (size_t)strfindlast(executablePath, pathSlash) - (size_t)executablePath + 1;
        	memcpy(executable_directory_buffer.data(), executablePath, copyLen);
        	executable_directory_buffer.m_abp = executable_directory_buffer.m_data + copyLen;
        	executable_directory_request = false;
    	}

    	return executable_directory_buffer;
    }
}
