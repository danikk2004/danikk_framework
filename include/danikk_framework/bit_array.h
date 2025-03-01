#pragma once

#include <danikk_framework/danikk_framework.h>

namespace danikk_framework
{
	struct BitArrayPointer
	{
		byte& data;
		byte offset;

		BitArrayPointer(byte& _data, byte _offset) : data(_data), offset(_offset){}

		void operator=(bool value)
		{
			byte unite_value = (byte)1 << offset;
			if(value)
			{
				data = data | unite_value;
			}
			else
			{
				unite_value = 255 - unite_value;
				data = data & unite_value;
			}
		}

		explicit operator bool() const
		{
			return data & ((byte)1 << offset);
		}
	};

	template<size_t byte_count>class BitArray
	{
	private:
		byte m_data[byte_count]{0};
	public:
		BitArray() = default;

		BitArray(uint64 value)
		{
			assert(byte_count <= 8);
			memcpy((byte*)m_data, (byte*)&value, byte_count);
		}

		BitArrayPointer operator[](index_t index)
		{
			index_t byte_index = index / 8;
			index_t bit_index = index % 8;
			assert(byte_index < byte_count);
			return BitArrayPointer(m_data[byte_index], bit_index);
		}
	};
}
