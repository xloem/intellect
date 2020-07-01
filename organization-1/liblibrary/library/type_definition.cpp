#include <library/type.hpp>

#include <typeinfo>

inline void * operator new(unsigned long, void * pointer) { return pointer; }

namespace library {

bool type_info::operator<(const type_info & other) const
{
	return std < other.std;
}

bool type_info::operator<=(const type_info & other) const
{
	return std <= other.std;
}

bool type_info::operator==(const type_info & other) const
{
	return *std == *other.std;
}

bool type_info::operator!=(const type_info & other) const
{
	return *std != *other.std;
}

bool type_info::operator>=(const type_info & other) const
{
	return std >= other.std;
}

bool type_info::operator>(const type_info & other) const
{
	return std > other.std;
}

template <typename Type>
type_info const & type()
{
	static struct type_info : public library::type_info
	{
		type_info()
		{
			std = &typeid(Type);
			size = sizeof(Type);
			name = std->name();
			construct_default = _construct_default;
			construct_copy = _construct_copy;
			assign = _assign;
			destroy = _destroy;
		}

		static void _construct_default(void * placement)
		{
			new (placement) Type();
		}

		static void _construct_copy(void * placement, void const * other)
		{
			new (placement) Type(*(Type const *)other);
		}

		static void _assign(void * object, void const * other)
		{
			*(Type*)object = *(Type const *)other;
		}

		static void _destroy(void * object)
		{
			((Type*)object)->~Type();
		}
	} info;
	return info;
}

template <typename Type>
type_info const & type(Type const &)
{
	return type<Type>();
}

} // namespace library
