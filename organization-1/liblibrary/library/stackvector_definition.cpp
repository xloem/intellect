#include <array>

#include <library/stackvector.hpp>



namespace library {
#define container_namespace std::
#define container array
#define vec (*(container_namespace container<element_type,_reserved>*)&storage)

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector()
{
	new(&storage) container_namespace container<element_type, _reserved>();
	resize(0);
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::~stackvector()
{
	vec.~container();
	resize(0);
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector(std::initializer_list<element_type> const & items)
: stackvector()
{
	for (element_type const & item : items) {
		push_back(item);
	}
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector(unsigned long size)
: stackvector()
{
	resize(size);
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector(unsigned long size, element_type const * data)
: stackvector(size)
{
	for (unsigned long i = 0; i < size; ++ i) {
		(*this)[i] = data[i];
	}
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector(stackvector const & other)
: stackvector()
{
	*this = other;
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved>::stackvector(stackvector && other)
: stackvector()
{
	*this = std::forward<stackvector>(other);
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved> & stackvector<element_type, _reserved>::operator=(stackvector const & other)
{
	vec = *(container_namespace container<element_type, _reserved>*)other.storage;
	resize(other.size());
	return *this;
}

template<typename element_type, unsigned long _reserved>
stackvector<element_type, _reserved> & stackvector<element_type, _reserved>::operator=(stackvector && other)
{
	vec = std::move(*(container_namespace container<element_type, _reserved>*)other.storage);
	resize(other.size()); other.resize(0);
	return *this;
}

template<typename element_type, unsigned long _reserved>
element_type & stackvector<element_type, _reserved>::operator[](unsigned long index)
{
	return vec[index];
}

template<typename element_type, unsigned long _reserved>
element_type const & stackvector<element_type, _reserved>::operator[](unsigned long index) const
{
	return vec[index];
}

template<typename element_type, unsigned long _reserved>
void stackvector<element_type, _reserved>::push_back(element_type const & value)
{
	vec[size()] = value;
	resize(size()+1);
}

template<typename element_type, unsigned long _reserved>
void stackvector<element_type, _reserved>::push_back(element_type && value)
{
	vec[size()] = std::move(value);
	resize(size()+1);
}

template<typename element_type, unsigned long _reserved>
unsigned long stackvector<element_type, _reserved>::size() const
{
	return _size;
}

template<typename element_type, unsigned long _reserved>
unsigned long constexpr stackvector<element_type,_reserved>::reserved() const
{
	return _reserved;
}

template<typename element_type, unsigned long _reserved>
unsigned long stackvector<element_type, _reserved>::size_bytes() const
{
	return _size * sizeof(element_type);
}

template<typename element_type, unsigned long _reserved>
unsigned long constexpr stackvector<element_type,_reserved>::reserved_bytes() const
{
	return _reserved * sizeof(element_type);
}

template<typename element_type, unsigned long _reserved>
void stackvector<element_type, _reserved>::resize(unsigned long size)
{
	_size = size;
}

template <typename element_type, unsigned long _reserved>
void stackvector<element_type, _reserved>::splice(unsigned long index, unsigned long old_length, element_type const * source, unsigned long new_length)
{
	unsigned long old_size = size();
	unsigned long new_size = old_size - old_length + new_length;
	auto output_position = vec.begin() + index;
	if (old_length < new_length) {
		resize(new_size);
		std::move_backward(output_position + old_length, vec.begin() + old_size, vec.begin() + new_size);
		std::copy(source, source + new_length, output_position);
	} else {
		std::copy(source, source + new_length, output_position);
		std::move(output_position + old_length, output_position + old_size - index, output_position + new_length);
		resize(new_size);
	}
}

template<typename element_type, unsigned long _reserved>
element_type & stackvector<element_type, _reserved>::front()
{
	return *vec.data();
}

template<typename element_type, unsigned long _reserved>
element_type & stackvector<element_type, _reserved>::back()
{
	return vec[size() - 1];
}

template<typename element_type, unsigned long _reserved>
element_type * stackvector<element_type, _reserved>::data()
{
	return vec.data();
}

template<typename element_type, unsigned long _reserved>
element_type * stackvector<element_type, _reserved>::begin()
{
	return vec.data();
}

template<typename element_type, unsigned long _reserved>
element_type * stackvector<element_type, _reserved>::end()
{
	return vec.data() + size();
}

template<typename element_type, unsigned long _reserved>
element_type const * stackvector<element_type, _reserved>::data() const
{
	return vec.data();
}

template<typename element_type, unsigned long _reserved>
element_type const * stackvector<element_type, _reserved>::begin() const
{
	return vec.data();
}

template<typename element_type, unsigned long _reserved>
element_type const * stackvector<element_type, _reserved>::end() const
{
	return vec.data() + size();
}

#undef container_namespace
#undef container
#undef vec
} // namespace library
