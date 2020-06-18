#include <array>

#include <library/stackvector.hpp>



namespace library {
#define container_namespace std::
#define container array
#define vec (*(container_namespace container<element_type,reserved>*)&storage)

template<typename element_type, unsigned long reserved>
stackvector<element_type, reserved>::stackvector()
{
	new(&storage) container_namespace container<element_type,reserved>();
	resize(0);
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved>::~stackvector()
{
	vec.~container();
	resize(0);
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved>::stackvector(std::initializer_list<element_type> const & items)
: stackvector()
{
	for (element_type & item : items) {
		push_back(item);
	}
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved>::stackvector(unsigned long size)
: stackvector()
{
	resize(size);
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved>::stackvector(stackvector const & other)
: stackvector()
{
	*this = other;
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved>::stackvector(stackvector && other)
: stackvector()
{
	*this = std::forward(other);
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved> & stackvector<element_type,reserved>::operator=(stackvector const & other)
{
	vec = *(container_namespace container<element_type,reserved>*)other.storage;
	resize(other.size());
	return *this;
}

template<typename element_type, unsigned long reserved>
stackvector<element_type,reserved> & stackvector<element_type,reserved>::operator=(stackvector && other)
{
	vec = std::move(*(container_namespace container<element_type,reserved>*)other.storage);
	resize(other.size); other.resize(0);
	return *this;
}

template<typename element_type, unsigned long reserved>
element_type & stackvector<element_type,reserved>::operator[](unsigned long index)
{
	return vec[index];
}

template<typename element_type, unsigned long reserved>
element_type const & stackvector<element_type,reserved>::operator[](unsigned long index) const
{
	return vec[index];
}

template<typename element_type, unsigned long reserved>
void stackvector<element_type,reserved>::push_back(element_type const & value)
{
	vec[size()] = value;
	resize(size()+1);
}

template<typename element_type, unsigned long reserved>
void stackvector<element_type,reserved>::push_back(element_type && value)
{
	vec[size()] = std::move(value);
	resize(size()+1);
}

template<typename element_type, unsigned long reserved>
unsigned long stackvector<element_type,reserved>::size() const
{
	return _size;
}

template<typename element_type, unsigned long reserved>
void stackvector<element_type,reserved>::resize(unsigned long size)
{
	_size = size;
}

template<typename element_type, unsigned long reserved>
element_type * stackvector<element_type,reserved>::data()
{
	return vec.data();
}

template<typename element_type, unsigned long reserved>
element_type * stackvector<element_type,reserved>::begin()
{
	return vec.data();
}

template<typename element_type, unsigned long reserved>
element_type * stackvector<element_type,reserved>::end()
{
	return vec.data() + size();
}

template<typename element_type, unsigned long reserved>
element_type const * stackvector<element_type,reserved>::data() const
{
	return vec.data();
}

template<typename element_type, unsigned long reserved>
element_type const * stackvector<element_type,reserved>::begin() const
{
	return vec.data();
}

template<typename element_type, unsigned long reserved>
element_type const * stackvector<element_type,reserved>::end() const
{
	return vec.data() + size();
}

#undef container_namespace
#undef container
#undef vec
} // namespace library
