#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/number.h>

namespace danikk_framework
{
	//Класс для последовательной записи данных произвольного типа в память.
	//Так-же можно использовать как буффер.
	class MemoryBuffer
    {
    private:
		char* m_data = NULL;
		size_t m_size = 0;
		size_t m_capacity = 0;
    public:
		~MemoryBuffer();

        MemoryBuffer() = default;

        MemoryBuffer(size_t m_capacity);

        void reserve(size_t dataSize);

        void resize(size_t dataSize);

        void clear();

        void skip(size_t size);

        template<class value_t> void write(const value_t* source, size_t size)
        {
        	size_t real_size = size * sizeof(value_t);
        	reserve(size);
            std::memcpy(m_data + m_size, source, real_size);
            m_size += real_size;
        }


        template<class value_t> void read(value_t* destination, size_t size)
        {
        	size_t real_size = size * sizeof(value_t);
        	std::memcpy(destination, m_data + m_size, real_size);
            m_size += real_size;
        }

        template <class valueT> MemoryBuffer& operator << (const valueT& data)
        {
        	reserve(sizeof(valueT));
        	*((valueT*)(m_data + m_size)) = data;
        	m_size += sizeof(valueT);
        	return *this;
        }

        template <class valueT> MemoryBuffer& operator >> (valueT& data)
        {
        	data = *((valueT*)(m_data + m_size));
        	m_size += sizeof(valueT);
        	return *this;
        }

        char* c_string();

        char* data();

		size_t size();

		size_t capacity();

		void moveTo(String& data);
    };
}
