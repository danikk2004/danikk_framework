#ifndef DANIKK_PLATFORM_MEMORY_STREAM_H
#define DANIKK_PLATFORM_MEMORY_STREAM_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/stream.h>
#include <danikk_framework/number.h>

namespace danikk_framework
{
	//Класс для последовательной записи данных произвольного типа в память.
	//Так-же можно использовать как буффер.
	class MemoryStream : public IOStream
    {
    private:
		char* m_data = NULL;
		size_t m_size = 0;
		size_t m_capacity = 0;
    public:
		~MemoryStream();

        MemoryStream() = default;

        MemoryStream(size_t m_capacity);

        void reserve(size_t dataSize);

        void resize(size_t dataSize);

        void clear();

        void skip(size_t size);

        void write(const char* source, size_t size);

        template <class valueT> void write(const valueT& input)
        {
        	if constexpr(is_number<valueT>::value)
			{
            	reserve(sizeof(valueT));
            	*((valueT*)(used())) = input;
            	m_size += sizeof(valueT);
			}
        	else
        	{
        		input.writeTo(*this);
        	}
        }

        void read(char* destination, size_t size);

        template <class valueT> void read(valueT& output)
        {
        	if constexpr(is_number<valueT>::value)
			{
            	output = *((valueT*)(used()));
            	m_size += sizeof(valueT);
			}
        	else
        	{
        		output.readFrom(*this);
        	}
        }

        char* c_string();

        char* data();

        char* used();

		size_t size();

		size_t capacity();

		void moveTo(String& data);
    };
}

#endif
