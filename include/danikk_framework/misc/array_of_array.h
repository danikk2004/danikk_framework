#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/assert.h>
#include <danikk_framework/sub_array.h>
#include <danikk_framework/array.h>

namespace danikk_framework
{
	template <size_t subarray_count, size_t element_count, class value_t> class ArrayOfArray
	{
	private:
		typedef uint32 metadata_size_t;

		struct metadata_t
		{
			uint32 data_start_index;
			uint32 size;
		};

		metadata_size_t metadata_array_size = subarray_count;
		metadata_t _metadata[subarray_count];
		value_t _data[element_count];
	public:

		/*uint64 metadataHash()
		{
			uint64 res = subarray_count + element_count;
			for(metadata_t metadata : metadata)
			{
				res += metadata.data_start_index + metadata.size;
				SubArray<value_t> arr = SubArray<value_t>((value_t*)data[metadata->data_start_index], metadata->size);
				for(size_t index = 0; index < arr.size(); index++)
				{
					res += arr[index];
				}
			}
			return res;
		}*/

		SubArray<value_t> operator[](int index)
		{
			metadata_t* metadata = &_metadata[index];
			return SubArray<value_t>((value_t*)&_data[metadata->data_start_index], metadata->size);
		}

		ArrayOfArray() = default;

		ArrayOfArray(InitList<size_t> array_sizes)
		{
			metadata_t* current_metadata = (metadata_t*)_metadata;
			metadata_t* prev_metadata = current_metadata;
			current_metadata->data_start_index = 0;
			current_metadata->size = 0;

			for(size_t sub_array_size : array_sizes)
			{
				current_metadata->data_start_index = prev_metadata->data_start_index + prev_metadata->size;
				current_metadata->size = sub_array_size;
				prev_metadata = current_metadata;
				current_metadata++;
			}
		}

		ArrayOfArray(Array<size_t, subarray_count> array_sizes)
		{
			metadata_t* current_metadata = (metadata_t*)_metadata;
			metadata_t* prev_metadata = current_metadata;
			current_metadata->data_start_index = 0;
			current_metadata->size = 0;

			for(size_t sub_array_size : array_sizes)
			{
				current_metadata->data_start_index = prev_metadata->data_start_index + prev_metadata->size;
				current_metadata->size = sub_array_size;
				prev_metadata = current_metadata;
				current_metadata++;
			}
		}

		void setValue(const value_t& _value)
		{
			for(size_t index = 0; index < metadata_array_size;index++)
			{
				SubArray<value_t> subarr = (*this)[index];
				for(value_t& value : subarr)
				{
					value = _value;
				}
			}
		}

		value_t* begin()
		{
			return _data;
		}

		value_t* end()
		{
			return _data + element_count;
		}
	};
}
