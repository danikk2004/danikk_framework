#pragma once

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	template<size_t general_size> class CompactStringBuffer
	{
	private:
		constexpr static size_t m_capacity = general_size - 1;
		uint8 m_size = 0;
		mutable char m_data[m_capacity];
	public:
		typedef CompactStringBuffer<general_size> this_t;

		size_t capacity()
		{
			return m_capacity;
		}

		uint8 size()
		{
			return m_size;
		}

		void clear()
		{
			m_size = 0;
		}

		this_t& operator<<(char chr)
		{
			m_data[m_size++] = chr;
			return *this;
		}

		char* c_string()
		{
			m_data[m_size] = '\0';
			return (char*)m_data;
		}

		const char* c_string() const
		{
			m_data[m_size] = '\0';
			return (char*)m_data;
		}

		char& operator[](index_t i)
		{
			return m_data[i];
		}
	};

	template<class stream_t, size_t general_size> stream_t& operator << (stream_t& stream, const CompactStringBuffer<general_size>& it)
	{
		return stream << it.c_string();
	}
}
