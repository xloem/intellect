#include "reference.hpp"

// We are hoping Karl will do something else.
// 	[but what?]
// 		[considered cleaning-truck: someone didn't agree]

class kinded-assignable
{
public:
	kinded-assignable(reference source, reference kind)
	: source(source), kind(kind)
	{ }

	kinded-assignable & operator=(reference const & other)
	{
		source.kind-set(kind, other);
		return *this;
	}

	operator reference() const
	{
		return source.kind-get(kind);
	}

private:
	reference source;
	reference kind;
};

class ordered-assignable
{
public:
	ordered-assignable(reference source, reference index)
	: source(source), kind(kind)
	{ }

	ordered-assignable & operator=(reference const & other)
	{
		source.order-set(index, other);
		return *this;
	}

	opereator reference() const
	{
		return source.order-get(index);
	}

private:
	reference source;
	reference index;
};
