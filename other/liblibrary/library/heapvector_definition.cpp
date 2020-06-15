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
heapvector<element_type>::heapvector(size_t size)
: heapvector()
{
	vec.reserve(size);
	vec.resize(size);
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
	*this = std::forward(other);
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
element_type & heapvector<element_type>::operator[](size_t index)
{
	return vec[index];
}

template <typename element_type>
element_type const & heapvector<element_type>::operator[](size_t index) const
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
size_t heapvector<element_type>::size() const
{
	return vec.size();
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
