#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/matrix_iter.h>
#include <danikk_framework/log.h>
#include <danikk_framework/glm.h>
#include <danikk_framework/assert.h>

namespace danikk_framework
{
	template<class value_type, uvec3 m_size> class FixedTensor
	{
	public:
		typedef value_type value_t;
		typedef FixedTensor<value_type, m_size> this_t;
	private:
		static constexpr size_t data_size = m_size.x * m_size.y * m_size.z;
		value_type m_data[data_size];
	public:
		value_t& operator[](uvec3 index)
		{
			assert(index.x < m_size.x);
			assert(index.y < m_size.y);
			assert(index.z < m_size.z);

			return m_data[index.x +
						  index.y * m_size.x +
						  index.z * m_size.x * m_size.y];
		}
		value_t& operator[](index_t index)
		{
			assert(index < data_size);
			return m_data[index];
		}

		this_t& operator =(const this_t& other)
		{
			for(index_t i = 0; i < data_size; i ++)
			{
				this->m_data[i] = other.m_data[i];
			}
			return *this;
		}

		bool operator ==(const this_t& other) const
		{
			for(index_t i = 0; i < data_size; i ++)
			{
				if(this->m_data[i] != other.m_data[i])
				{
					return false;
				}
			}
			return true;
		}

		uvec2 size() const
		{
			return m_size;
		}

		value_type* begin()
		{
			return m_data;
		}

		value_type* end()
		{
			return m_data + data_size;
		}

		value_type* begin() const
		{
			return m_data;
		}

		value_type* end() const
		{
			return m_data + data_size;
		}
	};
}
