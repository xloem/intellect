#pragma once

#include <cstddef>

namespace std {
	template <class T> class initializer_list;
}

namespace library {

template <typename T, size_t _reserved>
class stackvector
{
public:
	stackvector();
	stackvector(size_t size);
	stackvector(stackvector const & other);
	stackvector(stackvector && other);
	stackvector(std::initializer_list<T> const & items);
	~stackvector();

	T & operator[](size_t index);
	void push_back(T const & value);
	void push_back(T && value);

	size_t size();
	T * data();

	T * begin();
	T * end();
private:
	T storage[_reserved];
	size_t _size;
};


} // namespace library
