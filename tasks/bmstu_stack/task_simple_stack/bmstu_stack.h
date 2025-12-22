#pragma once

#include <exception>
#include <iostream>
#include <utility>

namespace bmstu
{
template <typename T>
class stack
{
   public:
	stack() = default;
	~stack()
	{
		clear();
		
	}
	stack(const stack& other) : size_(other.size_), data_(nullptr)
	{
		if (size_ > 0 && other.data_ != nullptr)
		{
			data_ = (T*)(operator new(sizeof(T) * size_));
			for (size_t i = 0; i < size_; ++i)
			{
				new (data_ + i) T(other.data_[i]);
			}
		}
	}

	stack(stack&& other)
	{
		size_ = other.size_;
		data_ = other.data_;
		other.data_ = nullptr;
		other.size_ = 0;
	}
	stack& operator=(const stack& other)
	{

		if (this != &other)
		{
			clear();
			size_ = other.size_;
			if (size_ > 0 && other.data_ != nullptr)
			{
				data_ = (T*)(operator new(sizeof(T) * size_));


				for (size_t i = 0; i < size_; i++)
				{
					new (data_ + i) T(other.data_[i]);
				}
			}
			else
			{
				data_ = nullptr;
			}
		}
		return *this;
	}
	stack& operator=(stack&& other) noexcept{
		if(this != &other){
			clear();
			size_ = other.size_;
			data_ = other.data_;
			other.size_ = 0;
			other.data_ = nullptr;
		}
		return *this;
	}
	bool empty() const noexcept { return size_ == 0; }

	template <typename... Args>
	void emplace(Args&&... args)
	{
		T* new_data = static_cast<T*>(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (new_data + size_) T(std::forward<Args>(args)...);

		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}

		operator delete(data_);

		data_ = new_data;
		size_++;
	}

	void push(const T& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (&new_data[size_]) T(value);

		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}

		operator delete(data_);
		data_ = new_data;
		size_ += 1;
	}

	T& top()
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

	const T& top() const
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		return data_[size_ - 1];
	}

	size_t size() const { return size_; }

	void clear()
	{
		for (size_t i = 0; i < size_; i++)
		{
			data_[i].~T();
		}
		operator delete(data_);
		data_ = nullptr;
		size_ = 0;
	}

	void push(T&& value)
	{
		T* new_data = (T*)(operator new(sizeof(T) * (size_ + 1)));

		for (size_t i = 0; i < size_; ++i)
		{
			new (new_data + i) T(std::move(data_[i]));
		}

		new (&new_data[size_]) T(std::move(value));

		for (size_t i = 0; i < size_; ++i)
		{
			data_[i].~T();
		}

		operator delete(data_);

		data_ = new_data;
		size_ += 1;
	}
	void pop()
	{
		if (empty())
		{
			throw std::underflow_error("Stack is empty");
		}
		data_[size_ - 1].~T();
		--size_;
	}

   private:
	T* data_ = nullptr;
	size_t size_ = 0;
};
}  // namespace bmstu
