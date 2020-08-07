#pragma once
#include "ref.hpp"

class wrapped : public ref
{
public:
	wrapped(ref what, il<std::pair<ref,ref>> refs = {})
	: ref(refs)
	{
		ref::set(sym::what, what);
	}

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
	 * making the change of commenting this out produced errors.
	 * need both fluid learning (microlearninghabit strength increase)
	 * and reduced changes from working states
	ref & operator*()
	{
		return ref::operator[](sym::what);
	}
	*/
};
