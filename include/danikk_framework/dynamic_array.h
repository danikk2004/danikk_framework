#ifndef DANIKK_PLATFORM_DYNAMIC_ARRAY_H
#define DANIKK_PLATFORM_DYNAMIC_ARRAY_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/memory.h>
//#include <danikk_framework/algorithm.h>
#include <cstring>

namespace danikk_framework
{
	template <class valueT> class DynamicArray
	{
	private:
		valueT* m_data;
		size_t m_size;
		size_t m_capacity;

	public:
		static constexpr size_t elementSize = sizeof(valueT);

		DynamicArray()
		{
			m_data = 0;
			m_size = 0;
			m_capacity = 0;
		}
		DynamicArray(const DynamicArray<valueT>& other)
		{
			m_size = other.size();
			m_capacity = m_size;
            m_data = (valueT*)malloc(size());
            memcpy(m_data, other.m_data, m_size);
		}

        DynamicArray(DynamicArray<valueT>&& other)
        {
			m_data = other.m_data;
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			new (&other) DynamicArray<valueT>();
        }

		DynamicArray(size_t capacity)
		{
			m_capacity = capacity;
			m_data = (valueT*)malloc(m_capacity * elementSize);
			m_size = 0;
		}

		DynamicArray(valueT* ptr, size_t lenght)
		{
			m_data = ptr;
			m_size = lenght;
			m_capacity = lenght;
		}

		DynamicArray(size_t lenght, valueT filler)
		{
			m_size = lenght;
			m_capacity = lenght;
			m_data = malloc(m_capacity  * elementSize);
			fill(filler);
		}

		template<class argT> DynamicArray(std::initializer_list<argT> data)
		{
			m_size = data.size();
			m_capacity = m_size;
			m_data = (valueT*)malloc(m_capacity * elementSize);

			valueT* thisIterator = begin();
			valueT* thisEnd = end();
			const argT* dataIterator = data.begin();

			while(thisIterator < thisEnd)
			{
				*thisIterator = std::move(valueT(*dataIterator));
				thisIterator++;
				dataIterator++;
			}
		}

		DynamicArray<valueT>& operator=(DynamicArray<valueT>&& other)
		{
			new (this) DynamicArray<valueT>((DynamicArray<valueT>&&)other);
			return *this;
		}

		~DynamicArray()
		{
            if (m_data != NULL)
			{
            	clear();
				free(m_data);
				m_data = NULL;
			}
		}

		bool operator!=(const DynamicArray<valueT>& other) const
        {
            if(other.lenght() != m_size)
            {
                return true;
            }

            for(size_t index = 0; index < m_size; index++)
            {
                if(other[index] != m_data[index])
                {
                    return true;
                }
            }
            return false;
        }

        bool operator==(const DynamicArray<valueT>& other) const
        {
            if(other.lenght() != m_size)
            {
                return false;
            }

            for(size_t index = 0; index < m_size; index++)
            {
                if(other[index] != m_data[index])
                {
                    return false;
                }
            }
            return true;
        }

		template<class argT> bool contains(const argT& value)
		{
			for(valueT& current : *this)
			{
				if(current == value)
				{
					return true;
				}
			}
			return false;
		}

		void cutElements(valueT element)
		{
			size_t cuttedElementsCount = 0;
			valueT* endElementPtr = end();
			for(valueT* currentElementPtr = begin();currentElementPtr < endElementPtr; currentElementPtr++)
			{
				*(currentElementPtr - cuttedElementsCount) = *currentElementPtr;
				if(*currentElementPtr == element)
				{
					cuttedElementsCount++;
				}
			}
		}

		void fill(valueT filler)
		{
			valueT* endElementPtr = end();
			for(valueT* ptr = begin(); ptr < endElementPtr; ptr++)
			{
				*ptr = filler;
			}
		}

		void clear()
		{
			for(valueT& current : *this)
			{
				(&current)->~valueT();
			}
			m_size = 0;
		}

		void reserve(size_t count)
		{
			size_t targetCapacity = count + m_size;
			if(m_capacity >= targetCapacity)								//Если вместимости хватает.
			{
				return;														//То ничего не делает.
			}
			else if(m_data != NULL)
			{
				while(m_capacity < targetCapacity)							//Если память была выделена до этого
				{
					m_capacity *= 2;
				}
				m_data = (valueT*)(*&realloc)(m_data, m_capacity * elementSize);//то "расширяет" её в 2 раза, пока необходимая вместимость не будет достигнута.
				return;
			}
			else
			{
				m_capacity = 1;
				while(m_capacity < targetCapacity)
				{
					m_capacity *= 2;
				}
				m_data = (valueT*)malloc(elementSize * m_capacity);
			}
		}

		void resize(size_t count)
		{
			reserve(count);
			m_size = count + m_size;
		}

		//Все push функции возвращают ссылку на добавленные элемент в массиве.

		valueT& push(const valueT& newElement)
		{
			reserve(1);
			valueT& result = m_data[m_size];
			result = newElement;
			m_size++;
			return result;
		}

		template<class... args_t> valueT& push_ctor(const args_t&... args)
		{
			reserve(1);
			valueT& result = m_data[m_size];
			m_size++;
			new (&result) valueT(args...);
			return result;
		}

		valueT& push_move(valueT&& newElement)
		{
			reserve(1);
			valueT result = m_data[m_size];
			m_size++;
			new (&result) valueT(std::move(newElement));
			return result;
		}

		valueT& push_move(valueT& newElement)
		{
			return push_move(std::move(newElement));
		}

		valueT& operator[](const size_t index)
		{
			return m_data[index];
		}

		valueT& peek() const
		{
			return m_data[m_size - 1];
		}

		valueT pop()
		{
			return m_data[--m_size];
		}

		valueT* data()
		{
			return m_data;
		}

		valueT* begin()
		{
			return m_data;
		}

		valueT* end()
		{
			return m_data + m_size;
		}

		size_t indexOf(const valueT& element) const
		{
			const valueT* end = end();
			for(const valueT* ptr = begin(); ptr < end; ptr++)
			{
				if(*ptr == element)
				{
					return (ptr - begin()) / elementSize;
				}
			}
			return -1;
		}

		size_t capacity() const
		{
			return m_capacity;
		}

		size_t size() const
		{
			return m_size;
		}
	};
}

#endif
