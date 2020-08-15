#include <vector>

#include <library/heapvector.hpp>

namespace library {
#define container std::vector
#define vec (*(container<element_type>*)storage)

template <typename element_type>
heapvector<element_type>::heapvector(std::initializer_list<element_type> const & items)
: storage(new container<element_type>(items))
{ }

template <typename element_type>
heapvector<element_type>::~heapvector()
{
	delete &vec;
}

template <typename element_type>
heapvector<element_type>::heapvector()
: heapvector(std::initializer_list<element_type>{})
{ }

template <typename element_type>
heapvector<element_type>::heapvector(unsigned long size)
: heapvector()
{
	vec.reserve(size);
	vec.resize(size);
}

template <typename element_type>
heapvector<element_type>::heapvector(unsigned long size, element_type const * data)
: heapvector(size)
{
	for (unsigned long i = 0; i < size; ++ i)
	{
		(*this)[i] = data[i];
	}
}

template <typename element_type>
heapvector<element_type>::heapvector(heapvector const & other)
: heapvector()
{
	*this = other;
}

template <typename element_type>
heapvector<element_type>::heapvector(heapvector && other)
: heapvector()
{
	*this = std::forward<heapvector>(other);
}

template <typename element_type>
heapvector<element_type> & heapvector<element_type>::operator=(heapvector const & other)
{
	vec = *(container<element_type>*)other.storage;
	return *this;
}
template <typename element_type>
heapvector<element_type> & heapvector<element_type>::operator=(heapvector && other)
{
	vec = std::move(*(container<element_type>*)other.storage);
	return *this;
}

template <typename element_type>
element_type & heapvector<element_type>::operator[](unsigned long index)
{
	return vec[index];
}

template <typename element_type>
element_type const & heapvector<element_type>::operator[](unsigned long index) const
{
	return vec[index];
}

template <typename element_type>
void heapvector<element_type>::push_back(element_type const & value)
{
	vec.push_back(value);
}

template <typename element_type>
void heapvector<element_type>::push_back(element_type && value)
{
	vec.push_back(std::move(value));
}

template <typename element_type>
unsigned long heapvector<element_type>::size() const
{
	return vec.size();
}

template <typename element_type>
unsigned long heapvector<element_type>::reserved() const
{
	return vec.capacity();
}

template <typename element_type>
void heapvector<element_type>::resize(unsigned long new_size)
{
	vec.resize(new_size);
}

template <typename element_type>
void heapvector<element_type>::splice(unsigned long index, unsigned long old_length, element_type const * source, unsigned long new_length)
{
	auto output_position = vec.begin() + index;
	if (old_length < new_length) {
		std::copy(source, source + old_length, output_position);
		vec.insert(output_position + old_length, source + old_length, source + new_length);
	} else {
		std::copy(source, source + new_length, output_position);
		vec.erase(output_position + new_length, output_position + old_length);
	}
}

template <typename element_type>
void heapvector<element_type>::splice(unsigned long index, unsigned long old_length, element_type const & source, unsigned long new_length)
{
	auto output_position = vec.begin() + index;
	if (old_length < new_length) {
		std::fill(output_position, output_position + old_length, source);
		vec.insert(output_position + old_length, new_length - old_length, source);
	} else {
		std::fill(output_position, output_position + new_length, source);
		vec.erase(output_position + new_length, output_position + old_length);
	}
}

template <typename element_type>
element_type & heapvector<element_type>::front()
{
	return *begin();
}

template <typename element_type>
element_type & heapvector<element_type>::back()
{
	return vec[size() - 1];
}

template <typename element_type>
element_type * heapvector<element_type>::data()
{
	return vec.data();
}

template <typename element_type>
element_type * heapvector<element_type>::begin()
{
	return vec.data();
}

template <typename element_type>
element_type * heapvector<element_type>::end()
{
	return vec.data() + vec.size();
}

template <typename element_type>
element_type const * heapvector<element_type>::data() const
{
	return vec.data();
}

template <typename element_type>
element_type const * heapvector<element_type>::begin() const
{
	return vec.data();
}

template <typename element_type>
element_type const * heapvector<element_type>::end() const
{
	return vec.data() + vec.size();
}

#undef container
#undef vec
} // namespace library
