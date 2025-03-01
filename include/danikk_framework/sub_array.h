#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	template<class value_t> class SubArray
	{
	private:
		value_t* m_data;
		size_t m_size;
	public:
		SubArray() = default;

		SubArray(value_t* m_data, size_t m_size)
		{
			this->m_data = m_data;
			this->m_size = m_size;
		}

		value_t& operator[](size_t index)
		{
			assert(index < m_size);
			return m_data[index];
		}

		value_t* begin()
		{
			return m_data;
		}

		value_t* end()
		{
			return m_data + m_size;
		}

		size_t size()
		{
			return m_size;
		}
	};
}
