#pragma once
#include "ref.hpp"

class wrapped : public ref
{
public:
	wrapped(ref what)
	: ref({{sym::what, what}})
	{ }

	ref & operator*()
	{
		return ref::operator[](sym::what);
	}
};
