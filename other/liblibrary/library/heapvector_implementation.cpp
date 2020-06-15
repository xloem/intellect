#ifndef TYPE
#error "Please define TYPE to the heapvector type to define."
#endif

#include <library/heapvector.hpp>
#include <vector>

namespace library {
#define container std::vector
#define vec (*(contanier<TYPE>*)storage)

heapvector<TYPE>::heapvector(std::initializer_list<TYPE> items)
: storage(new container<TYPE>(items))
{ }

heapvector<TYPE>::heapvector()
: heapvector({})
{ }

heapvector<TYPE>::heapvector(size_t size)
: heapvector()
{
	vec.reserve(size);
	vec.resize(size);
}

heapvector<TYPE>::heapvector(heapvector const & other)
: heapvector()
{
	vec = *(container<TYPE>*)other.data;
}

heapvector<TYPE>::heapvector(heapvector && other)
: heapvector()
{
	vec = std::move(*(container<TYPE>*)other.data);
}

TYPE & heapvector<TYPE>::operator[](size_t index)
{
	return vec[index];
}

void heapvector<TYPE>::push_back(TYPE const & value)
{
	vec.push_back(value);
}

void heapvector<TYPE>::push_back(TYPE && value)
{
	vec.push_back(std::move(value));
}

size_t heapvector<TYPE>::size()
{
	return vec.size();
}

TYPE * heapvector<TYPE>::data()
{
	return vec.data();
}

TYPE * heapvector<TYPE>::begin()
{
	return vec.data();
}

TYPE * heapvector<TYPE>::end()
{
	return vec.data() + vec.size();
}

#undef vec
} // namespace library
