#pragma once
#include <danikk_framework/dynamic_array.h>

namespace danikk_framework
{
	template<class value_t> class DualArray
	{
	private:
		DynamicArray<value_t> array_pair[2]{};
		uint32 current_array = 0;
		bool32 is_switched = false;

		DynamicArray<value_t>& getWriteArray()
		{
			return array_pair[current_array];
		}

		DynamicArray<value_t>& getReadArray()
		{
			return array_pair[getSwitchedIndex()];
		}

		void switchArray()
		{
			current_array = getSwitchedIndex();
			is_switched = true;
		}

		int getSwitchedIndex()
		{
			if(current_array == 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	public:
		DualArray() = default;

		value_t pop()
		{
			if(getReadArray().size() == 0)
			{
				switchArray();
			}
			return move(getReadArray().pop());
		}

		value_t& push(const value_t& value)
		{
			return getWriteArray().push(value);
		}

		void clear()
		{
			array_pair[0].clear();
			array_pair[1].clear();
		}

		bool isSwitched()
		{
			bool res = is_switched;
			is_switched = false;
			return res;
		}
	};
}
