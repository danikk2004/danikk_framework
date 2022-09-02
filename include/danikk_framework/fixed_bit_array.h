#ifndef DANIKK_PLATFORM_FIXED_BIT_ARRAY_H
#define DANIKK_PLATFORM_FIXED_BIT_ARRAY_H

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{

	//Представляет собой фиксированный битный массив для двоичных значений.
	//ContainerType должен быть числовым значением со знаком.
	class BitArray64
	{
	private:
		uint64 m_data = 0;

		static constexpr size_t fullValue = 0xFFFFFFFFFFFFFFFF;
		static constexpr size_t emptyValue = 0x0000000000000000;
	public:
		BitArray64() = default;

		BitArray64(bool value)
		{
			m_data = value ? fullValue : emptyValue;
		}

		BitArray64(uint64 value)
		{
			m_data = value;
		}

		bool getValue(const size_t index)
		{
			return (m_data << index) < 0;
		}

		void setValue(const size_t index, const bool value)
		{
			if(value)
			{
				m_data = m_data || (1ULL << index);
			}
			else
			{
				m_data = m_data && !(1ULL << index);
			}
		}

		size_t indexOfTrue()
		{
			size_t index = 0;
			while(!getValue(index))
			{
				index++;
			}
			return index;
		}

		bool any()
		{
			return m_data != emptyValue;
		}

		bool all()
		{
			return m_data == fullValue;
		}

		uint64& data()
		{
			return m_data;
		}
	};
}

#endif
