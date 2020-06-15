#pragma once

namespace std {
	template <class T> class initializer_list;
}

namespace library {

template <typename T>
class heapvector
{
public:
	heapvector();
	heapvector(size_t size);
	heapvector(heapvector const & other);
	heapvector(heapvector && other);
	heapvector(std::initializer_list<T> const & items);
	~heapvector();

	T & operator[](size_t index);
	void push_back(T const & value);
	void push_back(T && value);

	size_t size();
	T * data();

	T * begin();
	T * end();
private:
	void * storage;
};


} // namespace library
