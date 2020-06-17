#pragma once

namespace std {
	template <class T> class initializer_list;
}

namespace library {

template <typename T>
class heapvector
{
public:
	using element_type = T;

	heapvector();
	heapvector(unsigned long size);
	heapvector(heapvector const & other);
	heapvector(heapvector && other);
	heapvector(std::initializer_list<element_type> const & items);
	~heapvector();

	heapvector & operator=(heapvector const & other);
	heapvector & operator=(heapvector && other);
	element_type & operator[](unsigned long index);
	element_type const & operator[](unsigned long index) const;
	void push_back(element_type const & value);
	void push_back(element_type && value);

	unsigned long size() const;

	element_type * data();
	element_type * begin();
	element_type * end();

	element_type const * data() const;
	element_type const * begin() const;
	element_type const * end() const;
private:
	void * storage;
};


} // namespace library
