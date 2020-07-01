#pragma once

// A Map or Generalized Step is a view of a function that takes only 1 argument.
// In a space of concepts, the functions used would be those that navigate the space.  The location you are at is passed along tip-to-tail as 1 parameter,
// whereas the way of navigation, and any needed associated data, is the Map Step.

// std::bind provides enough interface to create these as callables from arbitrary other callables

// a map seems simply a callable with a consistent interface, reusable for a context
// 	(a language-translation to a navigation space)


template <typename T>
class map_step
{
public:
	virtual T step(T origin) = 0;
};

/*

template <typename _element_type, typename _operation_type, typename _operation_data_type>
class template_map
{
public:
	map(operation_type operation, operation_data_type data)
	: operation(operation), data(data)
	{}

	auto apply(element_type & focus) {
		return operation(focus, data);
	}

	operation_type const operation;
	operation_data_type const data;
};


// could use template-as-template-parameter tricks to make set_map use map
template <typename _reference_to_data, typename _operator_type = std::function<reference_to_data(reference_to_data, reference_to_data)>>
class map
{
public:
	using reference_to_data = _reference_to_data;
	using operator_type = _operator_type;

	map(operator_type operation, reference_to_data const & data)
	: operation(operation), data(data)
	{ }

	reference_to_data apply(reference_to_data & focus) {
		return operation(focus, data);
	}

	operation_type const operation;
	referene_to_data const data;
};

// it seems like a map is roughly a function call with all but one of the parameters specified
// which is equivalent to binding the function call with parameters e.g. std::bind

template <typename functionable, unsigned long missing_parameter, typename... provided_parameter_types>
{
public:
	
}
*/
