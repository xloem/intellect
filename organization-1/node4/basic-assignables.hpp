#pragma once

#include "reference.hpp"

// link objects could replace this with a lot more functionality.

class kinded-assignable //: public reference
{
public:
	/*
	DECLARE reference source;
	DECLARE reference kind;
	DECLARE reference inverse;
	*/

	kinded-assignable(reference source, reference kind)
	: source(source), kind(kind)
	{ }

	kinded-assignable & operator=(reference const & other)
	{
		source.kind-set(kind, other);
		return *this;
	}

	operator reference()
	{
		return source.kind-get(kind);
	}

protected:
	reference source;
	reference kind;
};

class ordered-assignable
{
public:
	ordered-assignable(reference source, reference index)
	: source(source), index(index)
	{ }

	ordered-assignable & operator=(reference const & other)
	{
		source.order-set(index, other);
		return *this;
	}

	operator reference()
	{
		return source.order-get(index);
	}

	bool operator==(ordered-assignable const & other)
	{
		return source == other.source && index.data<index_t>() == other.index.data<index_t>();
	}

	bool operator!=(ordered-assignable const & other)
	{
		return source != other.source || index.data<index_t>() != other.index.data<index_t>();
	}

	ordered-assignable & operator++()
	{
		index = reference((std::any)(index_t)(index.data<index_t>() + 1));
		return *this;
	}

	ordered-assignable & operator--()
	{
		index = reference((std::any)(index_t)(index.data<index_t>() - 1));
		return *this;
	}

	ordered-assignable operator++(int)
	{
		ordered-assignable result(source, index);
		index = reference((std::any)(index_t)(index.data<index_t>() + 1));
		return result;
	}

	ordered-assignable operator--(int)
	{
		ordered-assignable result(source, index);
		index = reference((std::any)(index_t)(index.data<index_t>() - 1));
		return result;
	}

private:
	reference source;
	reference index;
};
