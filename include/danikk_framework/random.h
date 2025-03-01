#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/memory_buffer.h>
#include <danikk_framework/dynamic_array.h>
#include <danikk_framework/assert.h>
#include <danikk_framework/type_traits.h>
#include <danikk_framework/array.h>

namespace danikk_framework
{
	class Random
	{
	private:
		static void* random_source;
		static constexpr size_t buffer_capacity = kib(1);
		index_t buffer_used = buffer_capacity;
		char buffer[buffer_capacity];

		void rawBlock(char* data, size_t size);
	public:
		void init();

		void read(char* data, size_t size = 1);

		template<class number_t> number_t number(number_t min = 0, number_t max = 0)
		{
			number_t num;
			read((char*)&num, sizeof(number_t));
			if(max == 0)
			{
				return num;
			}
			return min + num % (max - min);
		}

		float nextFloat()
		{
			static constexpr int rand_max = 100000;
			return (float)number<int>(0, rand_max) / rand_max * 2 - 1.0f;
		}

		template<class array_t, class index_t=uint> array_t::value_type& element(array_t& data)
		{
			if(data.size() == 1)
			{
				return data[0];
			}
			else
			{
				return data[number<index_t>(0, data.size())];
			}
		}

		template<class array_t> const array_t::value_type& element(const array_t& data)
		{
			if(data.size() == 1)
			{
				return data[0];
			}
			else
			{
				return data[number<uint>(0, data.size())];
			}
		}

		template<class array_t> array_t::value_type pull(array_t& array)
		{
			if(array.size() == 1)
			{
				return array.pop();
			}
			else
			{
				return array.pull(index(array));
			}
		}

		template<class array_t> void shuffle(array_t& data)
		{
			if(data.size() < 2)
			{
				return;
			}
			for(index_t i = 0; i < data.size(); i++)
			{
				swap(data[i], element(data));
			}
		}

		template<class array_t> index_t index(const array_t& data)
		{
			return number<uint32>(0, data.size());
		}

		template<class value_t> void indexarr(DynamicArray<index_t>& target, const DynamicArray<value_t>& data)
		{
			target.clear();
			target.reserve(data.size());

			for(index_t indexer = data.size() - 1;;indexer--)
			{
				target.push(indexer);
				if(indexer == 0)
				{
					break;
				}
			}
			shuffle(target);
		}
	};

	extern Random default_random;

	struct random_booler
	{
		Random& random = default_random;
		uint64 saved_num = 0;
		uint32 bool_count = 0;

		bool next();
	};
}
