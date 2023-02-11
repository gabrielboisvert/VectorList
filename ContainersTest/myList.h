#pragma once

#include "SpyAllocator.h"

namespace my
{
	template <typename T, class Allocator = SpyAllocator<T>>
	class list
	{
	private:
		//Node class
		class Node
		{
		private:
			T data;
			Node* prev;
			Node* next;
		public:
			//Constructor Destructor
			Node()
			{
				this->prev = nullptr;
				this->next = nullptr;
			}

			Node(const T& val) : data(val)
			{
				this->prev = nullptr;
				this->next = nullptr;
			}

			Node(T&& val) : data(std::move(val))
			{
				this->prev = nullptr;
				this->next = nullptr;
			}

			Node(const Node& node) : data(node.data)
			{
				this->next = node.next;
				this->prev = node.prev;
			}

			Node(Node&& node) : data(node.data)
			{
				this->next = node.next;
				this->prev = node.prev;
				node.next = nullptr;
				node.prev = nullptr;
			}

			Node& operator=(const Node& node)
			{
				if (this == &node)
					return *this;

				this->data = node.data;
				this->next = node.next;
				this->prev = node.prev;

				return *this;
			}

			Node& operator=(Node&& node)
			{
				if (this == &node)
					return *this;

				this->data = std::move(node.data);
				this->next = node.next;
				this->prev = node.prev;

				node.next = nullptr;
				node.prev = nullptr;

				return *this;
			}

			~Node()
			{
				this->prev = nullptr;
				this->next = nullptr;
			}

			//Element access
			T& getData()
			{
				return this->data;
			}

			const T getData() const
			{
				return this->data;
			}

			Node*& getPrev()
			{
				return this->prev;
			}

			Node* getPrev() const
			{
				return this->prev;
			}

			Node*& getNext()
			{
				return this->next;
			}

			Node* getNext() const
			{
				return this->next;
			}
		};

		size_t currentSize;
		Node* head;
		Node* tail;

	public:
		//Constructor, Destructor
		list()
		{
			this->currentSize = 0;
			this->head = nullptr;
			this->tail = this->head;
		}

		list(const list& lt)
		{
			this->currentSize = lt.currentSize;
			for (Node* it = lt.head; it != nullptr; it = it->getNext())
				this->push_back(it->getData());
		}

		list(list&& lt) noexcept
		{
			this->currentSize = lt.currentSize;
			this->head = lt.head;
			this->tail = lt.tail;
			lt.currentSize = 0;
			lt.head = nullptr;
			lt.tail = nullptr;
		}

		list& operator=(const list& lt)
		{
			if (this == &lt)
				return *this;

			this->~list();
			this->currentSize = lt.currentSize;
			for (Node* it = lt.head; it != nullptr; it = it->getNext())
				this->push_back(it->data);

			return *this;
		}

		list& operator=(list&& lt) noexcept
		{
			if (this == &lt)
				return *this;

			this->~list();
			this->currentSize = lt.currentSize;
			this->head = lt.head;
			this->tail = lt.tail;
			lt.currentSize = 0;
			lt.head = nullptr;
			lt.tail = nullptr;

			return *this;
		}

		~list()
		{
			for (Node* it = this->head; it != nullptr;/**/)
			{
				Node* tmp = it->getNext();
				
				it->~Node();
				Allocator::template rebind<Node>::other().deallocate(it, 1);

				it = tmp;
			}

			this->head = nullptr;
			this->tail = nullptr;
			this->currentSize = 0;
		}

		//Iterator class
		class iterator
		{
		private:
			Node* current;
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			//Constructor Destructor
			iterator()
			{
				this->current = nullptr;
			}

			iterator(Node* it)
			{
				this->current = it;
			}

			iterator(const iterator& it)
			{
				this->current = it.current;
			}

			iterator(iterator&& it) noexcept
			{
				this->current = it.current;
				it.current = nullptr;

			}

			iterator& operator=(const iterator& it)
			{
				if (this == &it)
					return *this;

				this->current = it.current;

				return *this;
			}

			iterator& operator=(iterator&& it) noexcept
			{
				if (this == &it)
					return *this;

				this->current = it.current;
				it.current = nullptr;

				return *this;
			}

			~iterator()
			{
				this->current = nullptr;
			}

			//Element access
			Node*& getCurrent()
			{
				return this->current;
			}

			Node* getCurrent() const
			{
				return this->current;
			}

			T& operator*()
			{
				return this->current->getData();
			}

			T* operator->() const
			{
				return &this->current->getData();
			}

			//Modifiers
			iterator operator++(int)
			{
				#ifdef _DEBUG
					if (this->current == nullptr)
						throw std::out_of_range("List: iterator out of range");
				#endif

				iterator tmp = iterator(this->current);
				this->current = this->current->getNext();
				return tmp;
			}

			iterator& operator++()
			{
				#ifdef _DEBUG
					if (this->current == nullptr)
						throw std::out_of_range("List: iterator out of range");
				#endif

				this->current = this->current->getNext();
				return *this;
			}

			iterator operator--(int)
			{
				#ifdef _DEBUG
					if (this->current == nullptr)
						throw std::out_of_range("List: iterator out of range");
				#endif

				iterator tmp = iterator(this->current);
				this->current = this->current->getPrev();
				return tmp;
			}

			iterator& operator--()
			{
				#ifdef _DEBUG
					if (this->current == nullptr)
						throw std::out_of_range("List: iterator out of range");
				#endif

				this->current = this->current->getPrev();
				return *this;
			}

			//Comparator
			const bool operator==(const iterator& it) const
			{
				return this->current == it.current;
			}

			const bool operator==(const Node* n) const
			{
				return this->current == n;
			}

			const bool operator!=(const iterator& it) const
			{
				return this->current != it.current;
			}
		};

		//Capacity
		size_t size()
		{
			return this->currentSize;
		}

		//Modifiers
		void push_back(const T& data)
		{
			Node* node = Allocator::template rebind<Node>::other().allocate(1);
			new (node) Node(data);

			if (this->head == nullptr)
			{
				this->head = node;
				this->tail = this->head;
			}
			else
			{
				this->tail->getNext() = node;
				this->tail->getNext()->getPrev() = this->tail;
				this->tail = this->tail->getNext();
			}
			this->currentSize++;
		}

		void push_back(T&& data)
		{
			Node* node = Allocator::template rebind<Node>::other().allocate(1);
			new (node) Node(std::move(data));

			if (this->head == nullptr)
			{
				this->head = node;
				this->tail = this->head;
			}
			else
			{
				this->tail->getNext() = node;
				this->tail->getNext()->getPrev() = this->tail;
				this->tail = this->tail->getNext();
			}
			this->currentSize++;
		}

		void remove(const T& data)
		{
			for (Node* it = this->head; it != nullptr; /**/)
			{
				if (it->getData() == data)
				{
					Node* tmp = it->getNext();

					if (it->getPrev() != nullptr)
						it->getPrev()->getNext() = it->getNext();

					if (it->getNext() != nullptr)
						it->getNext()->getPrev() = it->getPrev();

					if (it == this->head)
						this->head = tmp;

					if (it == this->tail)
						this->tail = it->getPrev();

					it->~Node();
					Allocator::template rebind<Node>::other().deallocate(it, 1);
					it = tmp;
					this->currentSize--;
				}
				else
					it = it->getNext();
			}
		}

		iterator insert(const iterator& it, const T& data)
		{
			if (it == nullptr)
			{
				this->push_back(data);
				return iterator(this->tail);
			}

			Node* newNode = Allocator::template rebind<Node>::other().allocate(1);
			new (newNode) Node(data);
			newNode->getNext() = it.getCurrent();
			newNode->getPrev() = it.getCurrent()->getPrev();

			if (it == this->head)
				this->head = newNode;

			if (it.getCurrent()->getPrev() != nullptr)
				it.getCurrent()->getPrev()->getNext() = newNode;
			it.getCurrent()->getPrev() = newNode;

			this->currentSize++;
			return iterator(newNode);
		}

		iterator insert(const iterator& it, const size_t n, const T& data)
		{
			iterator newIt;
			for (size_t i = 0; i < n; i++)
				if (i == 0)
					newIt = this->insert(it, data);
				else
					this->insert(it, data);

			return newIt;
		}

		template <class InputIterator> iterator insert(const iterator& it, const InputIterator& first, const InputIterator& last)
		{
			#ifdef _DEBUG
				if (first.getCurrent() >= last.getCurrent())
					throw std::out_of_range("List: vector iterator range transposed or vector iterators in range are from different containers");
			#endif

			iterator newIt;
			for (InputIterator itVec = first; itVec != last; ++itVec)
				if (itVec == first)
					newIt = this->insert(it, *itVec.getCurrent());
				else
					this->insert(it, *itVec.getCurrent());

			return newIt;
		}

		template<typename ...Args> void emplace_back(Args&&... value)
		{
			Node* node = Allocator::template rebind<Node>::other().allocate(1);
			new (node) Node((value)...);

			if (this->head == nullptr)
			{
				this->head = node;
				this->tail = this->head;
			}
			else
			{
				this->tail->getNext() = node;
				this->tail->getNext()->getPrev() = this->tail;
				this->tail = this->tail->getNext();
			}
			this->currentSize++;
		}

		//Iterator access
		iterator begin()
		{
			return iterator(this->head);
		}

		const iterator begin() const
		{
			return iterator(this->head);
		}

		iterator end()
		{
			if (this->tail == nullptr)
				return iterator(this->head);
			return iterator(this->tail->getNext());
		}

		const iterator end() const
		{
			if (this->tail == nullptr)
				return iterator(this->head);
			return iterator(this->tail->getNext());
		}
	};
}