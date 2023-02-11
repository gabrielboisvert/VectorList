#pragma once

#include "SpyAllocator.h"
#define BUFFER_SIZE 16
#define END_OF_LINE_SIZE 1
#define END_OF_LINE '\0'

namespace my
{
	template <typename T, class Allocator = SpyAllocator<T>>
	class String
	{
	public:
		//Constructor Destructor
		String()
		{}

		String(const T* str) noexcept
		{
			size_t length;
			for (length = 0; str[length] != END_OF_LINE; length++);

			if (length >= BUFFER_SIZE)
			{
				this->str.reserve(length + END_OF_LINE_SIZE);
				for (size_t i = 0; i < length; i++)
					this->str.push_back(str[i]);
				this->str.push_back(T(END_OF_LINE));
			}
			else
				for (size_t i = 0; i < length; i++)
					this->buffer[i] = str[i];
		}

		String(const String& str)
		{
			for (size_t i = 0; str.buffer[i] != END_OF_LINE; i++)
				this->buffer[i] = str.buffer[i];
			this->str = str.str;
		}

		String(String&& str) noexcept
		{
			for (size_t i = 0; str.buffer[i] != END_OF_LINE; i++)
				this->buffer[i] = std::move(str.buffer[i]);
			this->str = std::move(str.str);
		}

		String& operator=(const String& str)
		{
			if (this == &str)
				return *this;

			for (size_t i = 0; str.buffer[i] != END_OF_LINE; i++)
				this->buffer[i] = str.buffer[i];
			this->str = str.str;

			return *this;
		}

		String& operator=(String&& str) noexcept
		{
			if (this == &str)
				return *this;

			for (size_t i = 0; str.buffer[i] != END_OF_LINE; i++)
				this->buffer[i] = std::move(str.buffer[i]);
			this->str = std::move(str.str);

			return *this;
		}

		~String()
		{}
		
		//Element access
		const size_t length() const
		{
			if (str.size() != 0)
				return str.size() - END_OF_LINE_SIZE;

			size_t length;
			for (length = 0; this->buffer[length] != END_OF_LINE; length++);
			return length;
		}

		T& operator[](const size_t idx)
		{
			if (this->length() + END_OF_LINE_SIZE > BUFFER_SIZE)
				return this->str[idx];

			#ifdef _DEBUG
						if (idx >= BUFFER_SIZE)
							throw std::out_of_range("String: idx out of range");
			#endif
			return this->buffer[idx];
		}

		const T operator[](const size_t idx) const
		{
			if (this->length() + END_OF_LINE_SIZE > BUFFER_SIZE)
				return this->str[idx];

			#ifdef _DEBUG
				if (idx >= BUFFER_SIZE)
					throw std::out_of_range("String: idx out of range");
			#endif
			return this->buffer[idx];
		}

		const T* c_str() const
		{
			if (this->length() + END_OF_LINE_SIZE <= BUFFER_SIZE)
				return &this->buffer[0];
			return &this->str[0];
			
		}

		String operator+(const String& str)
		{
			size_t thisLength = this->length();
			size_t otherLength = str.length();

			String newStr;
			if (thisLength + otherLength + END_OF_LINE_SIZE < BUFFER_SIZE)
			{
				size_t idx = 0;
				for (size_t i = 0; this->buffer[i] != END_OF_LINE; i++, idx++)
					newStr.buffer[idx] = this->buffer[i];

				for (size_t i = 0; str.buffer[i] != END_OF_LINE; i++, idx++)
					newStr.buffer[idx] = str.buffer[i];
				return newStr;
			}

			newStr.str.reserve(thisLength + otherLength + END_OF_LINE_SIZE);
			for (size_t i = 0; i < thisLength; i++)
				newStr.str.push_back(this->operator[](i));

			for (size_t i = 0; i < otherLength; i++)
				newStr.str.push_back(str[i]);

			newStr.str.push_back(T(END_OF_LINE));
			return newStr;
		}
	private:
		vector<T> str;
		T buffer[BUFFER_SIZE];
	};
	
	typedef String<char> string;
}