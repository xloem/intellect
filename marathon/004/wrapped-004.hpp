#pragma once
#include "ref.hpp"

class wrapped : public ref
{
public:
	wrapped(ref what)
	: ref({{sym::what, what}})
	{ }

	wrapped(wrapped const & other) = default;

	ref get()
	{
		return ref::get(sym::what);
	}

	void set(ref what)
	{
		ref::set(sym::what, what);
	}

	/*
	ref & operator*()
	{
		return ref::operator[](sym::what);
	}
	*/
};
