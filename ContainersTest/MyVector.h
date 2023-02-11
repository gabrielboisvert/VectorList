#pragma once

#include "SpyAllocator.h"

namespace my
{
	template <typename T, class Allocator = SpyAllocator<T>>
	class vector
	{
	public:
		//Constructor, Destructor
		vector<T, Allocator>()
		{
			this->currentCapacity = 0;
			this->datas = nullptr;
			this->currentSize = 0;
		}

		vector<T, Allocator>(const vector& vector)
		{
			this->currentSize = vector.currentSize;
			this->currentCapacity = vector.currentCapacity;
			this->datas = Allocator().allocate(this->currentCapacity);
			for (unsigned int i = 0; i < this->currentSize; i++)
				new (&this->datas[i]) T(vector.datas[i]);
		}

		vector<T, Allocator>(vector&& vector) noexcept
		{
			this->currentSize = vector.currentSize;
			this->currentCapacity = vector.currentCapacity;
			this->datas = vector.datas;

			vector.datas = nullptr;
			vector.currentCapacity = 0;
			vector.currentSize = 0;
		}

		vector& operator=(const vector& vector)
		{
			if (this == &vector)
				return *this;

			this->clear();
			if (this->currentCapacity != vector.currentCapacity)
			{
				if (this->datas != nullptr)
					Allocator().deallocate(this->datas, this->currentCapacity);
				this->currentCapacity = vector.currentCapacity;
				this->datas = Allocator().allocate(this->currentCapacity);
			}

			this->currentSize = vector.currentSize;
			for (unsigned int i = 0; i < this->currentSize; i++)
				new (&this->datas[i]) T(vector.datas[i]);
			return *this;
		}

		vector& operator=(vector&& vector) noexcept
		{
			if (this == &vector)
				return *this;

			this->~vector();
			this->currentSize = vector.currentSize;
			this->currentCapacity = vector.currentCapacity;
			this->datas = vector.datas;

			vector.datas = nullptr;
			vector.currentCapacity = 0;
			vector.currentSize = 0;
			return *this;
		}

		~vector()
		{
			if (this->datas == nullptr)
				return;

			for (unsigned int i = 0; i < this->currentSize; i++)
				this->datas[i].~T();
			Allocator().deallocate(this->datas, this->currentCapacity);
			
			this->datas = nullptr;
		}

		//Iterator class
		class iterator
		{
		private:
			T* current;
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;
			using pointer = T*;
			using reference = T&;

			//Constructor Destructor
			iterator()
			{
				this->current = nullptr;
			}

			iterator(T* it)
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
			T*& getCurrent()
			{
				return this->current;
			}

			T* getCurrent() const
			{
				return this->current;
			}

			T& operator*()
			{
				return *this->current;
			}

			T* operator->() const
			{
				return this->current;
			}

			//Modifiers
			iterator operator++(int)
			{
				iterator tmp = iterator(this->current);
				this->current++;
				return tmp;
			}

			iterator& operator++()
			{
				++this->current;
				return *this;
			}

			iterator operator+(const size_t value) const
			{
				return iterator(this->current + value);
			}

			iterator operator-(const size_t value) const
			{
				return iterator(this->current - value);
			}

			iterator operator--(int)
			{
				iterator tmp = iterator(this->current);
				this->current--;
				return tmp;
			}

			iterator& operator--()
			{
				--this->current;
				return *this;
			}

			iterator& operator+=(const size_t value)
			{
				this->current += value;
				return *this;
			}

			iterator& operator-=(const size_t value)
			{
				this->current -= value;
				return *this;
			}

			//Comparator
			const bool operator==(const iterator& it) const
			{
				return this->current == it.current;
			}

			const bool operator!=(const iterator& it) const
			{
				return this->current != it.current;
			}
		};

		//Element access
		T& operator[](const size_t idx)
		{
			#ifdef _DEBUG
				if (idx >= this->currentSize)
					throw std::out_of_range("Vector: idx out of range");
			#endif
			return this->datas[idx];
		}

		const T& operator[](const size_t idx) const
		{
			#ifdef _DEBUG
				if (idx >= this->currentSize)
					throw std::out_of_range("Vector: idx out of range");
			#endif
			
			return this->datas[idx];
		}

		//Capacity
		void reserve(const size_t newSize)
		{
			#ifdef _DEBUG
				//The function throws length_error if n is greater than max_size.
				if (newSize >= INT_MAX)
					throw std::length_error("Size bigger than max size");
			#endif

			if (newSize <= this->currentCapacity)
				return;

			T* tmp = Allocator().allocate(newSize);
			for (unsigned int i = 0; i < this->currentSize; i++)
				new (&tmp[i]) T(this->datas[i]);
			this->~vector();
			this->datas = tmp;
			this->currentCapacity = newSize;
		}

		const size_t capacity() const
		{
			return this->currentCapacity;
		}

		const size_t size() const
		{
			return this->currentSize;
		}

		//Modifiers
		void clear()
		{
			for (size_t i = 0; i < this->currentSize; i++)
				this->datas[i].~T();
			this->currentSize = 0;
		}

		void push_back(const T& data)
		{
			if (this->currentSize == this->currentCapacity)
			{
				size_t newCapacity = this->currentCapacity * 2 != 0 ? this->currentCapacity * 2 : 1;
				T* tmp = Allocator().allocate(newCapacity);
				new (&tmp[this->currentSize]) T(data);
				for (size_t i = 0; i < this->currentSize; i++)
					new (&tmp[i]) T(this->datas[i]);
				
				this->~vector();
				this->datas = tmp;
				this->currentCapacity = newCapacity;
				this->currentSize++;
			}
			else
				new (&this->datas[this->currentSize++]) T(data);
		}

		void push_back(T&& data)
		{
			if (this->currentSize == this->currentCapacity)
			{
				size_t newCapacity = this->currentCapacity * 2 != 0 ? this->currentCapacity * 2 : 1;
				T* tmp = Allocator().allocate(newCapacity);
				new (&tmp[this->currentSize]) T(std::move(data));
				for (size_t i = 0; i < this->currentSize; i++)
					new (&tmp[i]) T(this->datas[i]);

				this->~vector();
				this->datas = tmp;
				this->currentCapacity = newCapacity;
				this->currentSize++;
			}
			else
				new (&this->datas[this->currentSize++]) T(std::move(data));
		}

		void resize(const size_t size)
		{
			if (size > this->currentCapacity)
				this->reserve(size);
			
			if (size <= this->currentSize)
				for (size_t i = this->currentSize; i < size; i++)
					this->datas[i].~T();
			else
				for (size_t i = this->currentSize; i < size; i++)
					new (&this->datas[i]) T();

			this->currentSize = size;
		}

		template<typename ...Args> void emplace_back(Args&&... value)
		{
			if (this->currentSize == this->currentCapacity)
			{
				size_t newCapacity = this->currentCapacity * 2 != 0 ? this->currentCapacity * 2 : 1;
				T* tmp = Allocator().allocate(newCapacity);
				new (&tmp[this->currentSize]) T((value)...);
				for (size_t i = 0; i < this->currentSize; i++)
					new (&tmp[i]) T(this->datas[i]);

				this->~vector();
				this->datas = tmp;
				this->currentCapacity = newCapacity;
				this->currentSize++;
			}
			else
				new (&this->datas[this->currentSize++]) T((value)...);
		}

		//Iterator access
		iterator begin()
		{
			return iterator(&this->datas[0]);
		}

		const iterator begin() const
		{
			return iterator(&this->datas[0]);
		}

		iterator end()
		{
			return iterator(&this->datas[this->currentSize]);
		}

		const iterator end() const
		{
			return iterator(&this->datas[this->currentSize]);
		}

	private:
		T* datas;
		size_t currentCapacity;
		size_t currentSize;
	};
}