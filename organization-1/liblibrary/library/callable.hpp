#pragma once

class callable
{
public:
	virtual unsigned long value_count() = 0;
	virtual library::type_info const & value_type(unsigned long index) = 0;
	virtual unsigned long first_output() = 0;
	virtual void call(void *[] values);
	  // get # arguments
	  // get type arguments
	  // call by say passing two vectors, one for input one for output
	  // we could also pass a calling object, that is queried for things
	  // we could pass a pointer to a list of pointers, to arguments and results
};

template <typename Result, typename ... Parameters>
class callable_template
{
public:
	// we can make callables compile-time inspectable, and even convert them to function pointers
};
