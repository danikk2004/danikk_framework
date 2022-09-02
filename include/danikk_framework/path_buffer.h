#ifndef DANIKK_PLATFORM_PATH_BUFFER_H
#define DANIKK_PLATFORM_PATH_BUFFER_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <danikk_framework/current_system.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/number.h>

namespace danikk_framework
{
#ifdef IS_LINUX
    constexpr char pathSlash = '/';
#elif IS_WINDOWS
    constexpr char pathSlash = '\\';
#endif

    //Класс для работы с путями.
    class PathBuffer
    {
    private:
        static constexpr size_t defaultCapacity = 512 - sizeof(char*);

        char m_data[defaultCapacity];
        char* m_abp;

        void findabp();

        void push(const char* data, size_t size);
    public:

        PathBuffer();

        PathBuffer(const String& path);

        PathBuffer& operator << (char chr);

        PathBuffer& setPath(const char* path);

        PathBuffer& setFileName(const char* name);

        PathBuffer& setFileName(const String& name, const String& extension);

        template<class stringT> PathBuffer& setFileName(const stringT& name);

        template<class dirT> PathBuffer& pushDirectory(const dirT& name);

    	PathBuffer& popDirectory();

    	friend const PathBuffer& getcwd();

        friend const PathBuffer& getDesktopDirectory();

    	friend const PathBuffer& getExecutableDirectory();

        PathBuffer& getcwd();

    	PathBuffer& getDesktopDirectory();

    	PathBuffer& getExecutableDirectory();

    	const char* getLastDirectoryName();

        friend ostream& operator <<(ostream& cout, const PathBuffer& path);

        const char* c_string() const;

        void skip(size_t offset);

        bool isEmpty() const;

        char* abp();

        char* data();

        const char* data() const;

        size_t size() const;

        char& operator[](size_t index);

		char firstChar() const;

		char lastChar() const;
    };

    template<class stringT> PathBuffer& PathBuffer::pushDirectory(const stringT& name)
    {
    	if constexpr(is_cstring<stringT>::value)
    	{
    		const char* namePtr = name;

    		if(*namePtr == pathSlash && firstChar() == pathSlash)
    		{
        		*this << (namePtr + 1);
    		}
    		else
    		{
    			*this << (namePtr);
    		}
    		if(*(m_abp - 1) != pathSlash)
    		{
    			*this << (pathSlash);
    		}
    		*m_abp = '\0';
    	}
    	else
    	{
    		if(name.firstChar() == pathSlash && firstChar() == pathSlash)
    		{
    			push(name.data() + 1, name.size() - 1);
    		}
    		else
    		{
    			push(name.data(), name.size());
    		}
    		if(name.lastChar() != pathSlash)
    		{
    			*this << (pathSlash);
    		}
        	*m_abp = '\0';

    	}
        return *this;
    }

    template<class stringT> PathBuffer& PathBuffer::setFileName(const stringT& name)
    {
		char* tempabp = m_abp;
		size_t nameSize = name.size();
		memcpy(tempabp, name.data(), nameSize);
		tempabp += name.size();
		*tempabp = '\0';

		return *this;
    }

    const PathBuffer& getcwd();

    const PathBuffer& getDesktopDirectory();

    const PathBuffer& getExecutableDirectory();
}

#endif
