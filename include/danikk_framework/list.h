#ifndef DANIKK_PLATFORM_LIST_H
#define DANIKK_PLATFORM_LIST_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/math.h>

namespace danikk_framework
{

	template <class T> class List
	{
	public:
		struct Node
		{
			T data;
			Node* next;

			Node(T data) : data(data), next(nullptr){}

			Node(T data, Node* next) : data(data), next(next){}
		};
	private:
		Node* m_first = NULL;
		size_t m_count = 0;

		void push(const T data, Node* memory)
		{
			if(m_count > 0)
			{
				Node* lastFirst = m_first;
				m_first = new (memory)Node(data,lastFirst);
			}
			else
			{
				m_first = new (memory)Node(data);
			}
			m_count++;
		}
	public:

		class ListIterator
		{
		private:
			Node* nodePtr;
		public:

			ListIterator(Node* ptr)
			{
				nodePtr = ptr;
			}
			bool operator!=(ListIterator const& other) const
			{
				return other.nodePtr != nodePtr;
			}
			bool operator==(ListIterator const& other) const
			{
				return other.nodePtr == nodePtr;
			}
			Node* operator*() const
			{
				return nodePtr;
			}

			ListIterator& operator++()
			{
				nodePtr = nodePtr->next;
				return *this;
			}
		};

		List() = default;

		void push(const T data)
		{
			push(data, (Node*)malloc(sizeof(Node)));
		}

		T pop()
		{
			if(m_count > 0)
			{
				Node* lastFirst = m_first;
				m_first = lastFirst->next;
				T result = lastFirst->data;
				m_count--;
				free((char*)lastFirst);
				return result;
			}
			return 0;
		}

		bool isEmpty() const
		{
			return m_first == NULL;
		}

		T firstData()
		{
			return m_first->data;
		}

		size_t count() const
		{
			return m_count;
		}

		ListIterator begin()
		{
			return ListIterator(m_first);
		}

		ListIterator end()
		{
			return ListIterator(NULL);
		}
	};
}

#endif
