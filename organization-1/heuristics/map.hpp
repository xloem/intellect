#pragma once

/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// Generalized Step
//
// A generalized step is roughly a predicate that can be moved around.

template <typename element_type, typename operation_type, typename operation_data_type>
class map
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
