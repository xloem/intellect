#pragma once

// NOTE: libpcre already has a cpp interface that seems pretty powerful and easy
// examples in /usr/include/pcrecpp.h on a system with libpcre3-dev

/*
namespace library {

class string;

template <typename element_type> class heapvector;
template <typename element_type, unsigned long reserved> class stackvector;

class regular_expression
{
public:
	regular_expression(string const & expression);
	~regular_expression();

	struct range {
		unsigned long begin;
		unsigned long end;
	};

	// first range of each result is whole match; rest are subexpressions
	heapvector<stackvector<range, 10>> find_all(string const & subject) const;
	stackvector<range, 10> find_one(string const & subject, unsigned long offset = 0) const;

private:
	void * storage;
};

using re = regular_expression;
*/

} // namespace library
