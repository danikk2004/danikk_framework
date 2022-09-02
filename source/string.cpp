#include <danikk_framework/string.h>
#include <danikk_framework/localization.h>
#include <danikk_framework/fixed_array.h>
#include <danikk_framework/cstring.h>
#include <danikk_framework/number.h>
#include <stdlib.h>
#include <string>

using namespace std;

namespace danikk_framework
{
	String::String()
	{
    	m_data = 0;
    	m_size = 0;
    	m_capacity = 0;
	}

    String::String(const String& other)
    {
        m_size = other.m_size;
        m_capacity = m_size + 1;
		m_data = (char*)malloc(m_capacity);
		memcpy(m_data, other.m_data, m_size);
    }

    String::String(String&& other)
    {
		m_data = other.m_data;
		other.m_data = NULL;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
    }

	String::String(const char* ptr, size_t size)
	{
        m_size = size;
        m_capacity = m_size + 1;
		m_data = (char*)malloc(m_capacity);
		memcpy(m_data, ptr, m_size);
    	m_data[m_size] = '\0';
	}

	String::String(const char* ptr):String(ptr, strlen(ptr)){}

	String::String(char* ptr):String(ptr, strlen(ptr)){}

	String::String(char* ptr, size_t size):String(ptr, size, size + 1){}

    String::String(char* ptr, size_t size, size_t capacity)
    {
    	m_data = ptr;
    	m_size = size;
    	m_capacity = capacity;
    }

	String::String(DynamicArray<char>&& data)
	{
    	m_data = data.data();
    	m_size = data.size();
    	m_capacity = data.capacity();
		new (&data) DynamicArray<char>();//Обнуляем все переменные у массива.
	}

    String::String(size_t size)
	{
    	m_size = size;
    	m_capacity = size + 1;
    	m_data = (char*)malloc(m_capacity);
    	m_data[m_size] = '\0';
	}

	String::String(size_t size, char initChar)
	{
    	m_size = size;
    	m_capacity = size + 1;
    	m_data = (char*)malloc(m_capacity);
    	memset(m_data, initChar, m_size);
    	m_data[m_size] = '\0';
	}

	String::String(const std::string& value):String((const char*)value.data(), value.length()){}

	String::~String()
	{
		if(m_data != NULL)
		{
			free(m_data);
			m_data = NULL;
		}
	}

	String String::copy() const
	{
		return String(*this);
	}

	void String::copyData(const String& source)
	{
		copyData(source.data(), source.size());
	}

	void String::copyData(const char* source)
	{
		copyData(source, strlen(source));
	}

	void String::copyData(const char* source, size_t sourceSize)
	{
		resize(sourceSize);
		memcpy(m_data, source, sourceSize);
	}

	void String::copyData(const char* source, const char* endptr)
	{
		copyData(source, endptr - source);
	}

    bool String::operator!=(const String& other) const
	{
    	if(other.m_size != m_size)
    	{
    		return true;
    	}
    	char* otherData = other.m_data;
    	for(size_t index = 0; index < m_size; index++)
    	{
    		if(m_data[index] != otherData[index])
    		{
    			return true;
    		}

    	}
    	return false;
	}

    bool String::operator==(const String& other) const
	{
    	if(other.size() != m_size)
    	{
    		return false;
    	}
    	char* otherData = other.m_data;
    	for(size_t index = 0; index < m_size; index++)
    	{
    		if(m_data[index] != otherData[index])
    		{
    			return false;
    		}
    	}
    	return true;
	}

    bool String::operator == (const char* other) const
	{
    	const char* current = begin();
    	const char* endPtr = end();

    	while(current < endPtr)
    	{
    		char chr1 = *current;
    		char chr2 = *other;

    		if(chr1 != chr2)
    		{
    			return false;
    		}
    		current++;
    		other++;
    	}
    	if(*other != '\0')
    	{
    		return false;
    	}
    	return true;
	}

	String& String::operator = (const String& other)
	{
        m_size = other.m_size;
        m_capacity = m_size;
		m_data = (char*)malloc(m_size);
		memcpy(m_data, other.m_data, m_size);
        return *this;
	}

	String& String::operator = (String&& other)
	{
		m_data = other.m_data;
		other.m_data = NULL;
		m_size = other.m_size;
		m_capacity = other.m_capacity;
        return *this;
	}

	char& String::operator[](size_t index) const
	{
		return m_data[index];
	}

    String& String::operator << (char value)
    {
    	reserve(1);
    	m_data[m_size] = value;
    	m_size++;
        return *this;
    }

    void String::pushMemory(const char* memory, size_t size)
    {
    	reserve(size);
    	memcpy(m_data + m_size, memory, size);
    	m_size += size;
    }

	size_t String::size() const
	{
		return m_size;
	}

    char* String::data()
	{
		return m_data;
	}

	const char* String::data() const
	{
		return (const char*)m_data;
	}

	char* String::begin()
    {
        return m_data;
    }

	const char* String::begin() const
    {
        return m_data;
    }

	char* String::end()
    {
        return m_data + m_size;
    }

	const char* String::end() const
    {
        return m_data + m_size;
    }

    const char* String::c_string() const
    {
        if(m_data != NULL)
        {
        	reserve(1);
            m_data[m_size] = '\0';
            return m_data;
        }
        return "";
    }

    bool String::haveEqualChar(const String& otherString) const
    {
    	const char* thisEnd = end();
    	const char* otherEnd = otherString.end();
        for (const char* thisIterator= begin(); thisIterator < thisEnd; thisIterator++)
        {
            for (const char* otherIterator = otherString.begin(); otherIterator < otherEnd; otherIterator++)
            {
                if (*thisIterator == *otherIterator)
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool String::haveOnlyEqualChars(const String & otherString) const
    {
    	const char* thisEnd = end();
    	const char* otherEnd = otherString.end();
        for (const char* thisIterator = begin(); thisIterator != thisEnd; thisIterator++)
        {
        	bool exits = false;
            for (const char* otherIterator = otherString.begin(); otherIterator != otherEnd; otherIterator++)
            {
                if (*thisIterator == *otherIterator)
                {
                	exits = true;
                }
            }
            if(!exits)
            {
            	return false;
            }
        }
        return true;
    }

    bool String::isEmpty() const
    {
    	return m_size == 0;
    }

    void String::split(const char* splitPtr, String& output1, String& output2) const
    {
    	output1 = copy();
    	output2 = copy();
        output1.substring(begin(), splitPtr - 1);
        output2.substring(splitPtr, end());
    }

    char* String::find(char character)
    {
		char* stringEnd = end();
        for (char* currentCharPtr = begin(); currentCharPtr < stringEnd; currentCharPtr++)
        {
            if (character == *currentCharPtr)
            {
                return currentCharPtr;
            }
        }
        return NULL;
    }

    const char* String::find(char character) const
    {
		const char* stringEnd = end();
        for (const char* currentCharPtr = begin(); currentCharPtr < stringEnd; currentCharPtr++)
        {
            if (character == *currentCharPtr)
            {
                return currentCharPtr;
            }
        }
        return NULL;
    }

	char* String::findLast(char character)
    {
        char* stringEnd = begin();
        for(char* currentCharPtr = end(); currentCharPtr != stringEnd; currentCharPtr--)
        {
        	if(*currentCharPtr == character)
        	{
        		return currentCharPtr;
        	}
        }
        return NULL;
    }

	const char* String::findLast(char character) const
    {
        const char* stringEnd = begin();
        for(const char* currentCharPtr = end(); currentCharPtr != stringEnd; currentCharPtr--)
        {
        	if(*currentCharPtr == character)
        	{
        		return currentCharPtr;
        	}
        }
        return NULL;
    }

    void String::clear()
    {
        m_size = 0;
    }

    void String::reserve(size_t count) const
	{
		size_t targetCapacity = count + m_size;
		if(m_capacity >= targetCapacity)												//Если вместимости хватает.
		{
			return;															//То ничего не делает.
		}
		else if(m_data != NULL)													//Если память была выделена до этого.
		{
			while(m_capacity < targetCapacity)
			{
				m_capacity *= 2;
			}
			m_data = (char*)realloc(m_data, m_capacity);
			return;
		}
		else
		{
			m_capacity = 1;
			while(m_capacity < targetCapacity)
			{
				m_capacity *= 2;
			}
			m_data = (char*)malloc(m_capacity);						//Если память не была выделена до этого, то просто выделяем её.
		}
	}

    void String::push(const char* data, size_t dataSize)
    {
        reserve(dataSize);
        memcpy(m_data + m_size, data, dataSize);
        m_size += dataSize;
    }

    void String::push(const String& otherString)
    {
        push(otherString.m_data, otherString.m_size);
    }

    void String::push(char character)
    {
		reserve(1);
		m_data[m_size++] = character;
    }

    void String::resize(size_t count)
    {
		reserve(count + 1);
		m_size = count;
    }

    char String::firstChar() const
    {
    	return *m_data;
    }

    char String::lastChar() const
    {
    	return *(m_data + m_size);
    }

    bool String::contains(char character) const
    {
    	const char* stringEnd = end();
        for (const char* currentIterator = begin(); currentIterator != stringEnd; currentIterator++)
        {
            if (character == *currentIterator)
            {
                return true;
            }
        }
        return false;
    }

    bool String::startsWith(const char* str) const
    {
    	const char* endPtr = end();

    	for(const char* current = begin(); current < endPtr; current++)
    	{
    		char chr1 = *str;
    		char chr2 = *current;
    		if(chr1 != chr2)
    		{
    			return false;
    		}
    		if(chr2 == '\0')
    		{
    			return true;
    		}
    		str++;
    		current++;
    	}
    	return false;
    }

    void String::splitToFirstSeparator(char separator, String& output1, String& output2) const
    {
    	split(find(separator), output1, output2);
    }

    void String::splitToLastSeparator(char separator, String& output1, String& output2) const
    {
    	split(findLast(separator), output1, output2);
    }

    String& String::cutChars(char chr)
    {
		char* readptr = m_data;
		char* writeptr = m_data;
		char* endptr = end();
		char curchr;

		while(readptr != endptr)
		{
			curchr = *readptr++;
			if(curchr != chr)
			{
				*writeptr++ = curchr;
			}
		}

		m_size = writeptr - m_data - 1;

    	return *this;
    }

    String& String::substring(const char* begin, const char* end)
    {
    	size_t m_size = end - begin;
    	memmove(m_data, begin, m_size);
		return *this;
    }

    String& String::cutAfterSeparator(char separator)
    {
    	substring(find(separator), end());
		return *this;
    }

    String& String::cutBeforeSeparator(char separator)
    {
    	substring(begin(), find(separator));
		return *this;
    }

    String& String::cutAfterLastSeparator(char separator)
    {
        substring(findLast(separator), end());
		return *this;
    }

    String& String::cutBeforeLastSeparator(char separator)
    {
        substring(begin(), findLast(separator));
		return *this;
    }

    String& String::cutSideChars(char chr)
    {
    	char* newData = begin();
    	char* newEnd = end();

    	while(*newData == chr)
    	{
    		newData++;
    	}

    	while(*newEnd == chr)
    	{
    		newEnd--;
    	}

    	substring(newData, newEnd);
		return *this;
    }

    String& String::replace(const String& sub, const String& result)
    {
    	size_t resultSize = result.size();
    	size_t subSize = sub.size();

    	if(resultSize < subSize)
    	{
    		char* endPtr = end();
    		char* currentWritePtr = m_data;
    		char* currentReadPtr = m_data;
    		size_t deltaSize = sub.size() - result.size();

    		while(currentReadPtr != endPtr)
    		{
    			if(strstartswith(currentReadPtr, sub))
    			{
    				memcpy(currentWritePtr, result.data(), resultSize);
    				currentWritePtr += resultSize;
    				currentReadPtr += subSize;
    				m_size -= deltaSize;
    			}
    			else
    			{
    				*currentWritePtr = *currentReadPtr;
    				currentWritePtr++;
    				currentReadPtr++;
    			}
    		}
    	}
    	else if(resultSize == subSize)
    	{
    		char* endPtr = end();
    		char* currentPtr = m_data;

    		while(currentPtr != endPtr)
    		{
    			if(strstartswith(currentPtr, sub))
    			{
    				memcpy(currentPtr, result.data(), resultSize);
    				currentPtr += resultSize;
    			}
    		}
    	}
    	else
    	{
    		size_t subCount = 0;

    		{
        		char* endPtr = end();
        		char* currentPtr = m_data;

        		while(currentPtr < endPtr)
        		{
        			if(strstartswith(currentPtr, sub))
        			{
        				currentPtr += subSize;
        				subCount++;
        			}
        			else
        			{
        				currentPtr++;
        			}
        		}
    		}

    		size_t deltaSize = resultSize - subSize;
    		size_t sizeGrow = subCount * deltaSize;

    		resize(m_size + sizeGrow);

    		//Вся строка будет перемещена во временный стековый буффер
    		char* tempStringCopy = (char*)alloca(m_size);
    		memcpy(tempStringCopy, m_data, m_size);
    		char* endPtr = tempStringCopy + m_size;
    		char* writePtr = m_data;
    		char* readPtr = tempStringCopy;

    		while(readPtr < endPtr)
    		{
    			if(strstartswith(readPtr, sub))
    			{
    				memcpy(writePtr, result.data(), resultSize);
    				readPtr += subSize;
    				writePtr += resultSize;
    			}
    			else
    			{
    				*writePtr++ = *readPtr++;
    			}
    		}
    	}

		return *this;
    }
}
