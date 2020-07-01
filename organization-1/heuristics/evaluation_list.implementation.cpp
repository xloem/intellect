#include <typeinfo>
#include "evaluation_list.hpp"

template <typename Type>
Type & typed_pointer::get<Type>()
{
	return *(Type*)value();
}

template <unsigned long bytes>
template <typename Type>
simple_typed_storage<size>::simple_typed_storage(Type const & data)
: _type(Type()),
  _size(sizeof(Type))
{
	if constexpr (sizeof(Type) > bytes) {
		static_assert(bytes > sizeof(void*));
		*(void**)&this->data = new unsigned char[sizeof(Type)];
		memcpy(*(void**)&this->data, &data, _sizeof(Type));
	} else {
		memcpy(this->data, &data, sizeof(Type));
	}
}
template <unsigned long bytes>
simple_typed_storage<size>::simple_typed_storage<typed_value>(typed_value const & data)
: _type(data.type()),
  _size(data.size())
{
	static_assert(bytes > sizeof(void*));
	if (_size > bytes) {
		*(void**)&this->data = new unsigned char[_size];
		memcpy(*(void**)&this->data, &data, _size);
	} else if (_size == bytes) {
		memcpy(this->data, data.value(), bytes);
	} else {
		memcpy(this->data, data.value(), _size);
	}
}

template <unsigned long bytes>
void simple_typed_storage::clear()
{
	if (_size > bytes) {
		delete *(void**)(&this->data)
	}
	_size = 0;
}

template <unsigned long bytes>
simple_typed_storage::~simple_typed_storage()
{
	clear();
}

template <unsigned long bytes>
unsigned long simple_typed_storage<bytes>::size()
{
	return _size;
}

template <unsigned long bytes>
void const * simple_typed_storage<bytes>::type()
{
	return _type;
}

template <unsigned long bytes>
void * simple_typed_storage<bytes>::value()
{
	return data;
}

template <unsigned long bytes>
typed_value & simple_typed_storage<bytes>::operator=(typed_pointer const & other)
{
	clear();
	_type = other.type();
	_size = other.size();
	if (_size > bytes) {
		*(void**)&this->data = new unsigned char[_size];
		memcpy(*(void**)&this->data, &data, _size);
	} else if (_size == bytes) {
		memcpy(this->data, data.value(), bytes);
	} else {
		memcpy(this->data, data.value(), _size);
	}
}

template <typename Return, typename ... Parameters>
struct std_function_operation<Return, Parameters...>::detail
{
	using Primary = std_function_operation<Return, Parameters...>

	template <unsigned long... indices>
	static void call(std::index_sequence<indices...>, Primary & primary, unsigned char * pointer_base)
	{
		*primary.result = primary.function(*(Parameters*)(pointer_base + (unsigned long)primary.parameters[indices]) ...);
	}
};

template <typename Return, typename... Parameters>
std_function_operation::std_function_operation(std::function<Return(Parameters...)> function)
: function(function)
{ }

template <typename Return, typename... Parameters>
void const * std_function_operation<Return, Parameters...>::type()
{
	return &typeid(std_function_operation<Return,Parameters...>);
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::input_count()
{
	return sizeof...(Parameters);
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::output_count()
{
	return 1;
}


template <typename Return, typename... Parameters>
void * std_function_operation<Return, Parameters...>::get_input(unsigned long whic)
{
	return parameters[which];
	//return *tuple_address_of(parameters, index);
}

template <typename Return, typename... Parameters>
void * std_function_operation<Return, Parameters...>::get_output(unsigned long which)
{
	return result;
}

template <typename Return, typename... Parameters>
void std_function_operation<Return, Parameters...>::set_input(unsigned long whic, void * address, void * base)
{
	parameters[which] = address - base;
	//return *tuple_address_of(parameters, index);
}

template <typename Return, typename... Parameters>
void std_function_operation<Return, Parameters...>::set_output(unsigned long which, void * address, void * base)
{
	result = address - base;
}

template <typename Parameter, typename... Parameters>
static void const * indexed_typeid(unsigned long index)
{
	if (index == 0) {
		return &typeid(Parameter);
	}
	if constexpr (sizeof...(Parameters) == 0) {
		return 0;
	} else {
		return indexed_typeid<Parameters...>(index - 1);
	}
}

template <typename Return, typename... Parameters>
void const * std_function_operation<Return, Parameters...>::input_type(unsigned long index)
{
	if constexpr (sizeof...(Parameters) == 0) {
		return 0;
	} else {
		return indexed_typeid<Parameters...>(index);
	}
}

template <typename Return, typename... Parameters>
void const * std_function_operation<Return, Parameters...>::output_type(unsigned long index)
{
	return &typeid(Return);
}

template <unsigned long index, typename... Types>
using indexed_type_t = typename indexed_type<index, Types...>::type;

template <typename Parameter, typename... Parameters>
static unsigned long indexed_sizeof(unsigned long index)
{
	if (index == 0) {
		return sizeof(Parameter);
	}
	if constexpr (sizeof...(Parameters) == 0) {
		return 0;
	} else {
		return indexed_sizeof<Parameters...>(index - 1);
	}
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::input_size(unsigned long index)
{
	//std::tuple<Parameters...> parameters;
	//return tuple_sizeof(parameters, index);
	return indexed_sizeof<Parameters...>(index);
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::output_size(unsigned long index)
{
	return sizeof(Return);
}


template <typename Return, typename... Parameters>
void std_function_operation<Return, Parameters...>::call(void * pointer_base)
{
	detail::call(std::index_sequence_for<Parameters...>, *this, pointer_base);

	// need to adjust all parameters now!
	//((Return*)return_) = std::apply(function, parameters);
	// how will we expand parameter indices
	//function(&parameters[offset<Parameters...>()]);
	//	would need an index list
}



/// old stuff

/*
template <typename... Parameters>
static unsigned long offset(unsigned long index)
{
	return 0;
}
template <typename Parameter, typename... Parameters>
static unsigned long offset<Parameter, Parameters...>(unsigned long index)
{
	if (index == 0) {
		return 0;
	} else {
		return offset<Parameters...>(index - 1) + sizeof(Parameter);
	}
}

namespace std {
template <unsigned long index, typename Return, typename... Parameters>
auto & get(std_function_operation<Return,Parameters...> & object)
{
	if (index == 
}
}
*/

/*
template <class Tuple, unsigned long template_index = 0>
inline void * tuple_address_of(Tuple & tuple, unsigned long function_index)
{
	if constexpr (template_index >= std::tuple_size_v<Tuple>) {
		return 0;
	} else {
		if (template_index == function_index) {
			return &std::get<template_index>(tuple);
		} else {
			return tuple_address_of<Tuple, template_index + 1>(tuple, function_index);
		}
	}
}
*/

/*
template <class Tuple, unsigned long template_index = 0>
inline unsigned long tuple_sizeof(Tuple & tuple, unsigned long function_index)
{
	if constexpr (template_index >= std::tuple_size_v<Tuple>) {
		return 0;
	} else {
		if (template_index == function_index) {
			return sizeof(std::tuple_element_t<template_index, Tuple>);
		} else {
			return tuple_typeid<Tuple, template_index + 1>(tuple, function_index);
		}
	}
}
*/

/*
template <unsigned long index, typename Type, typename... Types>
struct indexed_type
{
	using type = typename indexed_type<index - 1, Types...>::value;
};

template <typename Type, typename... Types>
struct indexed_type<0, Type, Types...>
{
	using type = Type;
};
*/
