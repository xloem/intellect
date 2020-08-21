#pragma once
#include "ref.hpp"

class wrapped : public ref
{
public:
	wrapped(ref what)
	: ref({{syms::what, what}})
	{ }

	ref & operator*()
	{
		return ref::operator[](syms::what);
	}
};
