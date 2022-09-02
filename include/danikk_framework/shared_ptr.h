#pragma once

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/memory.h>

namespace danikk_framework
{
	template<class valueT> class SharedPtr
	{
	private:
		valueT* value;
		size_t* ref_count_ptr;
		size_t ref_count;
	public:
		SharedPtr()
		{
			value = NULL;
			ref_count_ptr = NULL;
			ref_count = NULL;
		}

		SharedPtr(valueT&& value)
		{
			value = new (structmalloc<valueT>()) valueT(std::move(value));
		}

		~SharedPtr()
		{
			if(value != NULL)
			{
				value->~valueT();
				free(value);
			}
		}

		SharedPtr& operator=(SharedPtr& other)
		{

		}

		operator valueT&()
		{
			return &value;
		}
	};
}
