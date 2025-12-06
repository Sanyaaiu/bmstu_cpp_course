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
                if (data_ != nullptr)
                {
                        for (size_t i = 0; i < size_; ++i)
                        {
                                data_[i].~T();
                        }
                        operator delete(data_);
                }
        }

        bool empty() const noexcept { return size_ == 0; }

        template <typename... Args>
        void emplace(Args&&... args)
  {
    T* new_data = (T*)operator new(sizeof(T)*(size_ + 1));
    for (size_t i=0; i<size_; ++i){
      new (new_data + i) T(std::move(data_[i]));
    }
    new(new_data + size_) T(std::forward<Args>(args) ...);
    for (size_t i=0; i< size_; ++i){
      data_[i].~T();
    }
    operator delete(data_); 
    data_ = new_data;
    size_++;
  }

        void push(const T& value) 
        {
                T* new_data = (T*)operator new(sizeof(T) * (size_+1));
                for (size_t i = 0; i < size_; ++i)
                {
                        new (new_data + i) T(std::move(data_[i]));
                        data_[i].~T();
                }
                new (&new_data[size_]) T(value);
                operator delete(data_);
                data_ = new_data;
                size_+=1;
        }

        T& top() {
                if (empty()){
                        throw std::underflow_error("Stack is empty");
                }
                 return data_[size_-1]; }

        const T& top() const { 
                if (empty()){
                        throw std::underflow_error("Stack is empty");
                }
                return data_[size_-1]; }

        size_t size() const {return size_; }

        void clear()
        {
                operator delete(data_);
                data_ = nullptr;
                size_ = 0;
        }

        void push(T&& value) 
        {
                T* new_data = (T*)operator new(sizeof(T) * (size_+1));
                for (size_t i = 0; i < size_; ++i)
                {
                        new (new_data + i) T(std::move(data_[i]));
                        data_[i].~T();
                }
                new (&new_data[size_]) T(std::move(value));
                operator delete(data_);
                data_ = new_data;
                size_+=1;
        }
        void pop()
{
    if (empty())
    {
        throw std::underflow_error("Stack is empty");
    }
    data_[size_-1].~T();
    --size_;
}

   private:
        T *data_ = nullptr;
        size_t size_ = 0;
};
}
