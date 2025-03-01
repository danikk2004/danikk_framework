#error не доделано и мб не пригодится
#pragma once

#include<danikk_framework/danikk_framework.h>
#include<danikk_framework/dynamic_array.h>
#include<danikk_framework/memory.h>

namespace danikk_framework
{
	//Это класс очереди, который сохраняет выделяемую память.
	template<class value_t> class MemCachedQueue
	{
	private:
		struct object
		{
			value_t data;
			object* next;
			object* prev;
		};
		object* popptr;
		object* pushptr;

		DynamicArray<object*> memcache;

		object* allocate()
		{
			if(memcache.size() != 0)
			{
				return memcache.pop();
			}
			else
			{
				return structalloc<object>();
			}
		}

		void deallocate(object* data)
		{
			memcache.push(data);
		}
	public:
		MemCachedQueue()
		{
			popptr = NULL;
			pushptr = NULL;
		}

		void push(const value_t& data)
		{
			if(popptr == NULL && pushptr == NULL)
			{
				popptr = pushptr = structalloc<object>();
				pushptr->data = data;
				pushptr->next = NULL;
				pushptr->prev = NULL;
			}
			if(pushptr != NULL && head == NULL)
			{

			}
		}
	};
}
