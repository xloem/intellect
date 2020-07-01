#include <library/type_definition.cpp>
#include "evaluation_list.hpp"

template <typename Type>
Type & typed_pointer::get<Type>()
{
	return *(Type*)value();
}

template <typename Type>
simple_typed_pointer::simple_typed_pointer(Type * pointer)
: pointer_(pointer),
  type_(&library::type<Type>())
{ }

simple_typed_pointer::simple_typed_pointer(library::type_info const & type, void * pointer)
: pointer_(pointer),
  type_(&type)
{ }

template <unsigned long bytes>
simple_typed_storage<bytes>::simple_typed_storage()
: _type(&library::type<void>())
{ }

template <unsigned long bytes>
simple_typed_storage<bytes>::simple_typed_storage(library::type_info const & type)
: _type(&type)
{
	static_assert(bytes >= sizeof(void*));
	if (_type->size > bytes) {
		*(void**)&this->data = new unsigned char[_type->size];
	}
	_type->construct_default(value());
}

template <unsigned long bytes>
template <typename Type>
simple_typed_storage<bytes>::simple_typed_storage(Type const & data)
: _type(&library::type<Type>())
{
	if constexpr (sizeof(Type) > bytes) {
		static_assert(bytes >= sizeof(void*));
		*(void**)&this->data = new unsigned char[sizeof(Type)];
	}
	_type->construct_copy(value(), &data);
}
template <unsigned long bytes>
simple_typed_storage<bytes>::simple_typed_storage<typed_value>(typed_value const & data)
: _type(&data.type())
{
	static_assert(bytes >= sizeof(void*));
	if (_type->size > bytes) {
		*(void**)&this->data = new unsigned char[_type->size];
	}
	_type->construct_copy(value(), data.value());
}

template <unsigned long bytes>
void simple_typed_storage<bytes>::clear()
{
	_type->destroy(value());
	if (_type->size > bytes) {
		delete *(void**)(&this->data)
	}
	_type = library::type<void>();
}

template <unsigned long bytes>
simple_typed_storage<bytes>::~simple_typed_storage()
{
	clear();
}

template <unsigned long bytes>
void library::type_info const & simple_typed_storage<bytes>::type()
{
	return _type;
}

template <unsigned long bytes>
void * simple_typed_storage<bytes>::value()
{
	if (_type->size > bytes) {
		return *(void**)&data;
	} else {
		return data;
	}
}

template <unsigned long bytes>
typed_value & simple_typed_storage<bytes>::operator=(typed_pointer const & other)
{
	if (*_type != other.type()) {
		clear();
		_type = &other.type();
		if (_type->size > bytes) {
			*(void**)&this->data = new unsigned char[_type->size];
		}
		_type->construct_copy(value(), other.value());
	} else {
		_type->assign(value(), other.value());
	}
	return *this;
}

template <unsigned long bytes>
template <typename Type>
simple_typed_storage & simple_typed_storage<bytes>::operator=(Type const & other)
{
	if (*_type != library::Type<Type>()) {
		clear();
		_type = &library::type<Type>();
		if (_type->size > bytes) {
			*(void**)&this->data = new unsigned char[_type->size];
		}
		_type->construct_copy(value(), &data);
	} else {
		_type_>assign(value(), other.value());
	}
	return *this;
}

template <typename Return, typename ... Parameters>
struct std_function_operation<Return, Parameters...>::detail
{
	using Primary = std_function_operation<Return, Parameters...>

	template <typename Parameter, typename... Parameters>
	static library::type_info const & indexed_type(unsigned long index)
	{
		if constexpr (index > sizeof...(Parameters)) {
			return library::type<void>();
		} else if (index == 0) {
			return library::type<Parameter>();
		} else {
			return indexed_type<Parameters...>(index - 1);
		}
	}

	template <unsigned long... indices>
	static void call(std::index_sequence<indices...>, Primary & primary, unsigned char * pointer_base)
	{
		*(Return*)(pointer_base + (unsigned long)primary.result) = primary.function(*(Parameters*)(pointer_base + (unsigned long)primary.parameters[indices]) ...);
	}
};

template <typename Return, typename... Parameters>
std_function_operation::std_function_operation(std::function<Return(Parameters...)> function)
: function(function)
{ }

template <typename Return, typename... Parameters>
void library::type_info const & std_function_operation<Return, Parameters...>::type()
{
	return library::type<std_function_operation<Return,Parameters...>>();
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

template <typename Return, typename... Parameters>
void library::type_info const & std_function_operation<Return, Parameters...>::input_type(unsigned long index)
{
	if constexpr (sizeof...(Parameters) == 0) {
		return library::type<void>();
	} else {
		return detail::template indexed_type<Parameters...>(index);
	}
}

template <typename Return, typename... Parameters>
void library::type_info const & std_function_operation<Return, Parameters...>::output_type(unsigned long index)
{
	return library::type<Return>();
}

template <typename Return, typename... Parameters>
void std_function_operation<Return, Parameters...>::call(void * pointer_base)
{
	detail::call(std::index_sequence_for<Parameters...>, *this, pointer_base);
}



/// old stuff

template <unsigned long index, typename... Types>
using indexed_type_t = typename indexed_type<index, Types...>::type;

/*
template <typename Parameter, typename... Parameters>
static unsigned long indexed_sizeof(unsigned long index)
{
	if constexpr (index > sizeof...(Parameters)) {
		return 0;
	} else if (index == 0) {
		return sizeof(Parameter);
	} else {
		return indexed_sizeof<Parameters...>(index - 1);
	}
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::input_size(unsigned long index)
{
	//std::tuple<Parameters...> parameters;
	//return tuple_sizeof(parameters, index);
	//return indexed_sizeof<Parameters...>(index);
}

template <typename Return, typename... Parameters>
unsigned long std_function_operation<Return, Parameters...>::output_size(unsigned long index)
{
	return sizeof(Return);
}

*/

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
