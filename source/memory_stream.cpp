#include <danikk_framework/memory_stream.h>
#include <danikk_framework/danikk_framework.h>
#include <stdlib.h>
#include <cstring>

namespace danikk_framework
{
    MemoryStream::~MemoryStream()
    {
        if(m_data!=NULL)
        {
            ::free(m_data);
        }
    }

	MemoryStream::MemoryStream(size_t capacity)
	{
		m_data = (char*)malloc(capacity);
		m_size = 0;
		m_capacity = capacity;
	}

    void MemoryStream::reserve(size_t reservecapacity)
    {
        if (m_capacity - m_size < reservecapacity)
        {
			if(m_capacity != 0)
			{
				size_t newCapacity = m_capacity + reservecapacity;
				char* newMemory = (char*)malloc(newCapacity);
				memcpy(newMemory, m_data, m_capacity);
				free(m_data);
				m_data = newMemory;
				m_capacity = newCapacity;
			}
			else
			{
				m_capacity = reservecapacity;
				m_data = (char*)malloc(m_capacity);
			}
        }
    }

    void MemoryStream::resize(size_t dataSize)
    {
        reserve(dataSize);
        m_size = dataSize;
    }

    void MemoryStream::clear()
	{
		m_size = 0;
	}

    void MemoryStream::skip(size_t size)
    {
    	m_size += size;
    }

	void MemoryStream::write(const char* source, size_t size)
    {
    	reserve(size);
        std::memcpy(m_data + m_size, source, size);
        m_size += size;
    }

    void MemoryStream::read(char* destination, size_t size)
    {
        memcpy(destination, m_data + m_size, size);
        m_size += size;
    }

    char* MemoryStream::c_string()
    {
        m_data[m_size] = '\0';
        return m_data;
    }

    char* MemoryStream::data()
    {
    	return m_data;
    }

    char* MemoryStream::used()
    {
    	return m_size + m_data;
    }

    size_t MemoryStream::size()
    {
    	return m_size;
    }

    size_t MemoryStream::capacity()
    {
    	return m_capacity;
    }

    void MemoryStream::moveTo(String& data)
    {
    	data.m_data = m_data;
    	data.m_size = m_size;
    	data.m_capacity = m_capacity;
    	m_size = m_capacity = 0;
    	m_data = NULL;
    }
}
