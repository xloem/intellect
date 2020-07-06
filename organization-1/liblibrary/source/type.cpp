#include <library/type_definition.cpp>

namespace library {

static type_info const * type_initializations[] = {
	&type<void>(),
	&type<void const>(),
	&type<  signed char>(),
	&type<  signed char const>(),
	&type<unsigned char>(),
	&type<unsigned char const>(),
	&type<  signed int>(),
	&type<  signed int const>(),
	&type<unsigned int>(),
	&type<unsigned int const>(),
	&type<  signed short>(),
	&type<  signed short const>(),
	&type<unsigned short>(),
	&type<unsigned short const>(),
	&type<  signed long>(),
	&type<  signed long const>(),
	&type<unsigned long>(),
	&type<unsigned long const>(),
	&type<  signed long long>(),
	&type<  signed long long const>(),
	&type<unsigned long long>(),
	&type<unsigned long long const>(),
};

typed_mutable & typed_mutable::operator=(typed_const const & other)
{
	assign(other);
	return *this;
}

void typed_mutable::assign(typed_const const & other)
{
	if (type() != other.type()) {
		throw type_mismatch();
	}
	assign(other.void_pointer());
}

typed_retypable & typed_retypable::operator=(typed_const const & other)
{
	assign(other);
	return *this;
}

void typed_retypable::assign(typed_const const & other)
{
	assign(other.void_pointer(), other.type());
}

} // namespace library
