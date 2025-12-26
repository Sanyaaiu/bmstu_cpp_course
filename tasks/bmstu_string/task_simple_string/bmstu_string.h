#pragma once

#include <exception>
#include <iostream>

namespace bmstu
{
template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
class basic_string;

typedef basic_string<char> string;
typedef basic_string<wchar_t> wstring;
// typedef basic_string<char8_t> u8string;
typedef basic_string<char16_t> u16string;
typedef basic_string<char32_t> u32string;

template <typename T>
#ifdef _MSC_VER
class basic_string
#else
class basic_string
#endif
{
   public:
	/// Конструктор по умолчанию
	basic_string() : ptr_(new T[1]{0}), size_(0) {}

	basic_string(size_t size) : ptr_(new T[size + 1]), size_(size) 
	{
		for (size_t i = 0; i < size+1; i++){
			ptr_[i] = 0;
		}
	}

	basic_string(std::initializer_list<T> il)
		: ptr_(new T[il.size() + 1]), size_(il.size())
	{
		size_t i = 0;
		for (const auto& elem : il){
			ptr_[i++] = elem;
		}
		ptr_[size_] = 0;
	}

	/// Конструктор с параметром си-с
	basic_string(const T* c_str) 
	{
		size_ = strlen_(c_str);
		ptr_ = new T[size_+1];
		for (size_t i = 0; i < size_; i++){
			ptr_[i] = c_str[i];
		}
		ptr_[size_] = 0;
	}

	/// Конструктор копирования
	basic_string(const basic_string& other) : ptr_(new T[other.size_+1]), size_(other.size_) 
	{
		for (size_t i = 0; i < (size_+1);i++){
			ptr_[i] = other.ptr_[i];
		}
	}

	/// Перемещающий конструктор
	basic_string(basic_string&& dying) : ptr_(dying.ptr_), size_(dying.size_)
	{
		dying.ptr_ = nullptr;
		dying.size_ = 0;
	}

	/// Деструктор
	~basic_string() 
	{
		clean_();
	}

	/// Геттер на си-строку
	const T* c_str() const { return ptr_; }

	size_t size() const { return size_; }

	/// Оператор перемещающего присваивания
	basic_string& operator=(basic_string&& other)
	{
		if (this != &other){
			delete[] ptr_;
			ptr_ = other.ptr_;
			size_ = other.size_;
			other.ptr_ = nullptr;
			other.size_ = 0;
		}
		 return *this; 
	}

	/// Оператор копирующего присваивания си строки
	basic_string& operator=(const T* c_str) 
	{
		size_t new_size = strlen_(c_str);
		T* new_ptr = new T[new_size + 1];
		for (size_t i = 0; i < new_size; i++){
			new_ptr[i] = c_str[i];
		}
		new_ptr[new_size] = '\0';
		delete[] ptr_;
		ptr_ = new_ptr;
		size_ = new_size;
		return *this; 
	}

	/// Оператор копирующего присваивания
	basic_string& operator=(const basic_string& other)
	{
		if (this != &other){
			T* new_ptr = new T[other.size_+1];
			for (size_t i = 0; i < other.size_+1; i++){
				new_ptr[i] = other.ptr_[i];
			}
			clean_();
			ptr_ = new_ptr;
			size_ = other.size_;
		}
		return *this; 
	}

	friend basic_string<T> operator+(const basic_string<T>& left,
									 const basic_string<T>& right)
	{
		basic_string<T> res(left.size_ + right.size_);
		for (size_t i = 0; i < left.size_; i++){
			res.ptr_[i] = left.ptr_[i];
		}
		for (size_t i = 0; i < right.size_; i++){
			res.ptr_[left.size_+i] = right.ptr_[i];
		}
		res.ptr_[res.size_] = 0;
		return res;
	}

	template <typename S>
	friend S& operator<<(S& os, const basic_string& obj)
	{
		for (size_t i = 0; i < obj.size_; i++) {
			os << obj.ptr_[i];
		}
		return os;
	}

	template <typename S>
	friend S& operator>>(S& is, basic_string& obj)
	{
		T buffer[256];
		size_t i = 0;
		T ch;
		ch = is.get();
		while (ch != ' ' && ch!= '\t' && ch!='\n' && ch!=EOF && i < 255);
		return is;
	}

	basic_string& operator+=(const basic_string& other) { return *this; }

	basic_string& operator+=(T symbol) { return *this; }

	T& operator[](size_t index) noexcept { return *(ptr_ + index); }

	T& at(size_t index) { throw std::out_of_range("Wrong index"); }

	T* data() { return ptr_; }

   private:
	static size_t strlen_(const T* str) 
	{
		size_t length = 0;
		while (str[length] != '\0'){
			length++;
		}
		 return length; 
	}

	void clean_() 
	{
		if (ptr_ != nullptr){
			delete[] ptr_;
			ptr_ = nullptr;
			size_ = 0;
		}

	}

	T* ptr_ = nullptr;
	size_t size_;
};
}  // namespace bmstu
