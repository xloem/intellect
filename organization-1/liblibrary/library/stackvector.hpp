#pragma once

namespace std {
	template <class T> class initializer_list;
}

namespace library {

template <typename T, unsigned long _reserved = 128>
class stackvector
{
public:
	using element_type = T;

	stackvector();
	stackvector(unsigned long size);
	stackvector(unsigned long size, element_type const * data);
	stackvector(stackvector const & other);
	stackvector(stackvector && other);
	stackvector(std::initializer_list<element_type> const & items);
	~stackvector();

	stackvector & operator=(stackvector const & other);
	stackvector & operator=(stackvector && other);
	element_type & operator[](unsigned long index);
	element_type const & operator[](unsigned long index) const;
	void push_back(element_type const & value);
	void push_back(element_type && value);

	unsigned long size() const;
	unsigned long constexpr reserved() const;
	unsigned long size_bytes() const;
	unsigned long constexpr reserved_bytes() const;
	void resize(unsigned long new_size);

	void splice(unsigned long index, unsigned long old_length, element_type const * source, unsigned long new_length);

	element_type & front();
	element_type & back();

	element_type * data();
	element_type * begin();
	element_type * end();

	element_type const * data() const;
	element_type const * begin() const;
	element_type const * end() const;
private:
	element_type storage[_reserved];
	unsigned long _size;
};


} // namespace library
