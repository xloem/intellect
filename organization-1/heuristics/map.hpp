#pragma once

/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// Generalized Step
//
// A generalized step is roughly a predicate that can be moved around.

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
