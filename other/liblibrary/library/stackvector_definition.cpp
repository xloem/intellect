#ifndef TYPE
#error "Please define TYPE to the stackvector type to define."
#endif
#ifndef RESERVED
#error "Please define RESERVED to the stackvector size to define."
#endif

#include <library/stackvector.hpp>

#include <array>


using namespace std;

namespace library {
#define container array
#define vec (*(container<TYPE,RESERVED>*)storage)

static_assert(sizeof(stackvector<TYPE,RESERVED>::storage) == sizeof(container<TYPE,RESERVED>));

stackvector<TYPE,RESERVED>::stackvector()
: size(0)
{
	new(&storage) container<TYPE,RESERVED>();
}

stackvector<TYPE,RESERVED>::~stackvector()
{
	storage.~container<TYPE,RESERED>();
	size = 0;
}

stackvector<TYPE,RESERVED>::stackvector(std::initializer_list<TYPE> items)
: stackvector()
{
	for (TYPE & item : items) {
		push_back(item);
	}
}

stackvector<TYPE,RESERVED>::stackvector(size_t size)
: stackvector()
{
	vec.reserve(size);
	vec.resize(size);
}

stackvector<TYPE,RESERVED>::stackvector(stackvector const & other)
: stackvector()
{
	vec = *(container<TYPE,RESERVED>*)other.storage;
}

stackvector<TYPE,RESERVED>::stackvector(stackvector && other)
: stackvector()
{
	vec = std::move(*(container<TYPE,RESERVED>*)other.data);
}

TYPE & stackvector<TYPE,RESERVED>::operator[](size_t index)
{
	return vec[index];
}

void stackvector<TYPE,RESERVED>::push_back(TYPE const & value)
{
	vec.push_back(value);
}

void stackvector<TYPE,RESERVED>::push_back(TYPE && value)
{
	vec.push_back(std::move(value));
}

size_t stackvector<TYPE,RESERVED>::size()
{
	return vec.size();
}

TYPE * stackvector<TYPE,RESERVED>::data()
{
	return vec.data();
}

TYPE * stackvector<TYPE,RESERVED>::begin()
{
	return vec.data();
}

TYPE * stackvector<TYPE,RESERVED>::end()
{
	return vec.data() + vec.size();
}

#undef vec
} // namespace library
