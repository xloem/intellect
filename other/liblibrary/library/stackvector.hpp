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
	using element_type = T;

	stackvector();
	stackvector(size_t size);
	stackvector(stackvector const & other);
	stackvector(stackvector && other);
	stackvector(std::initializer_list<element_type> const & items);
	~stackvector();

	stackvector & operator=(stackvector const & other);
	stackvector & operator=(stackvector && other);
	element_type & operator[](size_t index);
	element_type const & operator[](size_t index) const;
	void push_back(element_type const & value);
	void push_back(element_type && value);

	size_t size() const;
	void resize(size_t new_size);

	element_type * data();
	element_type * begin();
	element_type * end();

	element_type const * data() const;
	element_type const * begin() const;
	element_type const * end() const;
private:
	element_type storage[_reserved];
	size_t _size;
};


} // namespace library
