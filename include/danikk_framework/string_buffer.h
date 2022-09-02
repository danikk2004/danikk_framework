#ifndef DANIKK_PLATFORM_STRING_BUFFER_H
#define DANIKK_PLATFORM_STRING_BUFFER_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/string.h>
#include <danikk_framework/number.h>
#include <danikk_framework/cout_features.h>
#include <danikk_framework/type_traits.h>

#include <string>

namespace danikk_framework
{
	template<size_t _capacity> class StringBuffer
	{
	public:
		static constexpr size_t capacity = _capacity;
	private:
		char m_data[capacity];
		char* m_end;

		void write(const char* data, size_t size)
		{
			memcpy(m_end, data, size);
			m_end += size;
		}
	public:
		StringBuffer()
		{
			m_end = m_data;
		}

		StringBuffer(StringBuffer& other)
		{
			memcpy(m_data, other.m_data, other.size());
			m_end = other.m_end;
		}

		StringBuffer(StringBuffer&& other)
		{
			memcpy(m_data, other.m_data, other.size());
			m_end = other.m_end;
		}

		StringBuffer& operator=(StringBuffer& other)
		{
			memcpy(m_data, other.m_data, other.size());
			m_end = other.m_end;
		}

		template<class str_t> StringBuffer(const str_t& data)
		{
			if constexpr(is_cstring<str_t>::value)
			{
				const char* ptr = data;
				char* push_ptr = m_data;
				size_t remaining = capacity;
				while(*ptr != '\0')
				{
					if(remaining == 0)
					{
						throw "[danikk_framework::StringBuffer] capacity < data_size";
					}
					*push_ptr = *ptr;
					remaining--;
					push_ptr++;
					ptr++;
				}
				m_end = push_ptr;
			}
			else
			{
				size_t data_size = data.size();
				if(capacity < data_size)
				{
					throw "[danikk_framework::StringBuffer] capacity < data_size";
				}
				memcpy(m_data, data.data(), data_size);
				m_end = m_data + data_size;
			}
		}

		template<class str_t> StringBuffer& operator =(const str_t& data)
		{
			new (this) StringBuffer<_capacity>(data);
			return *this;
		}

		bool sizeWillFit(size_t checkSize)
		{
			return checkSize < (capacity - size());
		}

		bool willFit(const char* data)
		{
			return sizeWillFit(strlen(data));
		}

		bool willFit(const String& data)
		{
			return sizeWillFit(data.size());
		}

		StringBuffer& operator << (char data)
		{
			*m_end = data;
			m_end++;
			return *this;
		}

		char* c_string()
		{
			*m_end = '\0';
			return m_data;
		}

		const char* c_string() const
		{
			*m_end = '\0';
			return m_data;
		}

		char* data()
		{
			return m_data;
		}

		const char* data() const
		{
			return m_data;
		}

		void clear()
		{
			m_end = m_data;
		}

		char& operator [](size_t index)
		{
			return m_data[index];
		}

		size_t size() const
		{
			return (size_t)m_end - (size_t)m_data;
		}

		char* end()
		{
			return m_end;
		}

		const char* end() const
		{
			return m_end;
		}

		char firstChar()
		{
			return *m_data;
		}

		char endChar()
		{
			return  *(m_end - 1);
		}
	};

	template<class stream_t, size_t string_buffer_size> stream_t& operator <<(stream_t& stream,const StringBuffer<string_buffer_size>& data)
	{
		const char* current = data.data();
		const char* endptr = data.end();

		while(current < endptr)
		{
			stream << *current;
			current++;
		}
		return stream;
	}
}

#endif
