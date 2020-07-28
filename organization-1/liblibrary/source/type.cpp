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

typed_valued & typed_valued::operator=(typed_valued const & other)
{
	assign(other);
	return *this;
}

void typed_valued::assign(typed_valued const & other)
{
	if (this->type() != other.type()) {
		throw type_mismatch();
	}
	assign(other.void_pointer());
}

typable_valued & typable_valued::operator=(typed_valued const & other)
{
	assign(other);
	return *this;
}

void typable_valued::assign(typed_valued const & other)
{
	assign(other.void_pointer(), other.type());
}

typed_typable_valued & typed_typable_valued::operator=(typed_typable_valued const & other)
{
	typable_valued::operator=((typed_valued const &)other);
	return *this;
}

} // namespace library
