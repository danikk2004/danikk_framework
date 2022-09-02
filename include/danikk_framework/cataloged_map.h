#ifndef DANIKK_PLATFORM_NAMED_RESOURCE_MAP_H
#define DANIKK_PLATFORM_NAMED_RESOURCE_MAP_H

#include <danikk_framework/danikk_framework.h>
#include <danikk_framework/fixed_array.h>
#include <danikk_framework/stream.h>
#include <danikk_framework/memory.h>
#include <danikk_framework/cataloged_map_key_parser.h>
#include <danikk_framework/string.h>
#include <danikk_framework/number.h>
#include <danikk_framework/cout_features.h>
#include <concepts>

namespace danikk_framework
{
	template <class valueT> class CatalogedMap
	{
	private:

		using this_type = CatalogedMap<valueT>;
		/*
		 * Числовой тип для хранения строк внутри.
		 * Просто в эти 8 байт записывается строка, а работать с ней можно как с числом.
		 */
		using chunkT = CatalogedMapKeyParser::chunkT;

		template<class operatorValueT> friend std::ostream& operator <<(std::ostream& cout, CatalogedMap<operatorValueT>& map);

		struct Node
		{
			valueT value;
			chunkT keyChunk;
			struct
			{
				size_t capacity;
				size_t size;
				Node* ptr;
			}childArray;

			union
			{
				struct
				{
					//Если массив дочерних узлов выделялся в динамической памяти, то при очищении вызываем free,
					//если в статической, то ничего не делаем, так как статическая память чистится при вызове деструктора карты.
					bool isDynamicArray;
					bool isNewCatalog;
					bool valueIsExits;
				}flags;
				size_t flagsMemory;
			};
		} root;

		//При чтении карты ресурсов в бинарном виде вся память выделяется одним большим куском,
		//это указатель на него.
		Node* staticNodeMemory;

		class NodeIterator
		{
		public:
			struct NodeStackElement
			{
				Node* node = 0;
				Node* currentChild = 0;
				Node* childEnd = 0;

				NodeStackElement() = default;

				NodeStackElement(Node* ptr)
				:node(ptr),
				 currentChild(ptr->childArray.ptr),
				 childEnd(ptr->childArray.ptr + ptr->childArray.size){}
			};

			FixedArray<NodeStackElement, 16> nodeStack;
			size_t rootIsChecked = false;

			NodeIterator() = default;

			NodeIterator(CatalogedMap<valueT>& map)
			{
				nodeStack.push(&map.root);
			}

			Node* next()
			{
				NodeStackElement& lastElement = nodeStack.peek();

				if(rootIsChecked)
				{
					while(lastElement.currentChild == lastElement.childEnd)//Это нужно для "прыжка" вверх по йерархии.
					{
						nodeStack.pop();
						lastElement = nodeStack.peek();
					}
					Node* result = lastElement.currentChild++;
					nodeStack.push_move(NodeStackElement(result));
					return result;
				}
				else
				{
					rootIsChecked = true;
					return nodeStack[0].node;
				}
			}

			size_t depth()
			{
				return nodeStack.size() - 1;
			}

			bool isEnd()
			{
				if(rootIsChecked)
				{
					NodeStackElement& lastElement = nodeStack.peek();
					while(lastElement.currentChild == lastElement.childEnd)
					{
						nodeStack.pop();
						if(nodeStack.size() == 0)
						{
							return true;
						}
						lastElement = nodeStack.peek();
					}
				}
				return false;
			}
		};
	public:
		CatalogedMap()
		{
			root.keyChunk = 0;
			root.childArray.capacity = 0;
			root.childArray.size = 0;
			root.childArray.ptr = NULL;
			root.flagsMemory = 0;//Все флаги инициализируем в false.
			staticNodeMemory = NULL;
		}

		~CatalogedMap()
		{
			//#TODO:сделать вычищение памяти.
		}

		class Iterator
		{
		private:
			NodeIterator nodeIterator;
		public:
			class Element
			{
			private:
				valueT& m_value;
				chunkT m_key[16]{};
			public:
				valueT& value()
				{
					return m_value;
				}

				const char* key()
				{
					return (const char*)m_key;
				}
			};

			Iterator(CatalogedMap<valueT>& map):nodeIterator(map){}

			bool isEnd()
			{
				return nodeIterator.isEnd();
			}

			Element next()
			{
				Element result;
				result.m_value = nodeIterator.nodeStack.peek().node->value;
				for(size_t keyChunkIndex = 0; keyChunkIndex < nodeIterator.nodeStack.lenght(); keyChunkIndex++)
				{
					result.m_key[keyChunkIndex] = nodeIterator.nodeStack[keyChunkIndex];
				}
				nodeIterator.next();
				return result;
			}
		};

		bool isEmpty()
		{
			return root.childArray.ptr == NULL && !root.flags.valueIsExits;
		}

		valueT& insert(const char* key, size_t keySize, const valueT& value)
		{
			CatalogedMapKeyParser parser(key, keySize);

			if(parser.end())//Если строка пустая.
			{
				root.value = std::move(value);
				root.flags.valueIsExits = true;
				return root.value;
			}

			Node* currentNode = &root;

			bool isNewCatalog;
			chunkT currentChunk;

			while(true)
			{
				branch_find_cycle_body:
				isNewCatalog = parser.isNewCatalog();
				currentChunk = parser.next();

				//Ищем следующий подходящий узел.
				Node* end = currentNode->childArray.ptr + currentNode->childArray.size;
				for(Node* currentChildNode = currentNode->childArray.ptr; currentChildNode < end; currentChildNode++)
				{
					if(currentChildNode->flags.isNewCatalog == isNewCatalog && currentChildNode->keyChunk == currentChunk)
					{
						currentNode = currentChildNode;
						if(parser.end())
						{
							currentNode->value = std::move(value);
							currentNode->flags.valueIsExits = true;
							return currentNode->value;
						}
						else
						{
							goto branch_find_cycle_body;
						}
					}
				}
				goto branch_create_cycle_body;
			}

			//Если вышло в этот цикл, то дальше ветку нужно создавать.
			while(true)
			{
				isNewCatalog = parser.isNewCatalog();
				currentChunk = parser.next();

				branch_create_cycle_body:
				if(currentNode->childArray.capacity == currentNode->childArray.size)
				{
					Node* newChildArray = (Node*)malloc(sizeof(Node) * ++currentNode->childArray.capacity);//Создаём массив, увеличенный на 1.

					if(currentNode->flags.isDynamicArray)
					{
						Node* currentRead = currentNode->childArray.ptr;
						Node* currentWrite = newChildArray;
						Node* end = currentWrite + currentNode->childArray.size;
						while(currentWrite < end)
						{
							*currentWrite = std::move(*currentRead);
							currentRead++;
							currentWrite++;
						}

						free(currentNode->childArray.ptr);
					}
					else
					{
						currentNode->flags.isDynamicArray = true;
					}
					currentNode->childArray.ptr = newChildArray;
				}

				currentNode = &currentNode->childArray.ptr[currentNode->childArray.size++];

				if(parser.end())
				{
					currentNode->value = std::move(value);
					currentNode->keyChunk = currentChunk;
					currentNode->childArray.capacity = 0;
					currentNode->childArray.size = 0;
					currentNode->childArray.ptr = NULL;
					currentNode->flags.isDynamicArray = false;
					currentNode->flags.isNewCatalog = isNewCatalog;
					currentNode->flags.valueIsExits = true;
					return currentNode->value;
				}
				else
				{
					//currentNode->value пропускаем, значение не инициализировано.
					currentNode->keyChunk = currentChunk;
					currentNode->childArray.capacity = 1;
					currentNode->childArray.size = 0;
					currentNode->childArray.ptr = structmalloc<Node>();
					currentNode->flags.isDynamicArray = true;
					currentNode->flags.isNewCatalog = isNewCatalog;
					currentNode->flags.valueIsExits = false;
				}
			}
		}

		valueT& insert(const char* key, const valueT& value)
		{
			return insert(key, strlen(key), value);
		}

		valueT& insert(const String& key, const valueT& value)
		{
			return insert(key.data(), key.size(), value);
		}

		valueT& get(const char* key, size_t keySize)
		{
			if(isEmpty())
			{
				return createNullRef<valueT>();
			}

			CatalogedMapKeyParser parser(key, keySize);

			Node* currentNode = &root;

			while(true)
			{
				bool isNewCatalog = parser.isNewCatalog();
				chunkT currentChunk = parser.next();

				Node* currentChildNode = currentNode->childArray.ptr;
				Node* end = currentChildNode + currentNode->childArray.size;

				while(currentChildNode < end)
				{
					if(currentChildNode->flags.isNewCatalog == isNewCatalog && currentChildNode->keyChunk == currentChunk)
					{
						currentNode = currentChildNode;
						if(parser.end())
						{
							if(!currentNode->flags.valueIsExits)//Если значения не существует, то выдаёт ошибку через errno.
							{
								return createNullRef<valueT>();
							}
							return currentNode->value;
						}
						else
						{
							break;
						}
					}

					currentChildNode++;
				}

				//МБ баг будет.
				if(parser.end())
				{
					return createNullRef<valueT>();
				}
			}
			return createNullRef<valueT>();
		}

		valueT& get(const char* key)
		{
			return get(key, strlen(key));
		}

		valueT& get(const String& key)
		{
			return get(key.data(), key.size());
		}
	};

	template<class operatorValueT> std::ostream& operator << (std::ostream& cout, CatalogedMap<operatorValueT>& map)
	{
		typedef typename CatalogedMap<operatorValueT>::Node Node;
		typedef typename CatalogedMap<operatorValueT>::chunkT chunkT;
		typedef typename CatalogedMap<operatorValueT>::NodeIterator NodeIterator;

		NodeIterator iterator(map);

		while(!iterator.isEnd())
		{
			Node* currentNode = iterator.next();

			cout << indent(iterator.depth());

			if(currentNode->keyChunk != 0)
			{
				char* keyChunkPtr = (char*)&currentNode->keyChunk;
				for(size_t charIndex = 0; charIndex < sizeof(chunkT); charIndex++)
				{
					cout << keyChunkPtr[charIndex];
				}
				cout << ' ';
			}
			else
			{
				cout << "empty ";
			}
			if(currentNode->flags.valueIsExits)
			{
				cout << currentNode->value;
			}
			else
			{
				cout << "null";
			}
			cout << endl;
		}
		return cout;
	}
}

#endif
