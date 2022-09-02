#ifndef DANIKK_PLATFORM_FIXED_ARRAY_H
#define DANIKK_PLATFORM_FIXED_ARRAY_H

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	template <class valueT, size_t capacity> class FixedArray
	{
	private:
		valueT m_data[capacity];

		size_t m_size = 0;
	public:

		FixedArray() = default;

		void push(const valueT& element)
		{
			m_data[m_size++] = element;
		}

		void push_move(valueT&& element)
		{
			new (&m_data[m_size++]) valueT(std::move(element));
		}

		valueT pop()
		{
			return m_data[--m_size];
		}

		valueT& peek()
		{
			return m_data[m_size - 1];
		}

		bool isEmpty()
		{
			return m_size == 0;
		}

		size_t size()
		{
			return m_size;
		}

		valueT& operator[](size_t index)
		{
			return m_data[index];
		}

		valueT* begin()
		{
			return m_data;
		}

		valueT* end()
		{
			return m_data + m_size;
		}

		void clear()
		{
			m_size = 0;
		}
	};
}

#endif
