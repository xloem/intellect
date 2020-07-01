#include <library/type.hpp>

#include <typeinfo>
#include <type_traits>

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
	struct type_info : public library::type_info
	{
		type_info()
		{
			std = &typeid(Type);
			if constexpr (!std::is_same_v<Type, void>) {
				size = sizeof(Type);
			} else {
				size = 0;
			}
			name = std->name();
			construct_default = _construct_default;
			construct_copy = _construct_copy;
			assign = _assign;
			destroy = _destroy;
		}

		static void _construct_default(void * placement)
		{
			if constexpr (std::is_default_constructible_v<Type>) {
				new (placement) Type();
			}
		}

		static void _construct_copy(void * placement, void const * other)
		{
			if constexpr (std::is_copy_constructible_v<Type>) {
				new (placement) Type(*(Type const *)other);
			}
		}

		static void _assign(void * object, void const * other)
		{
			if constexpr (std::is_copy_assignable_v<Type>) {
				*(Type*)object = *(Type const *)other;
			}
		}

		static void _destroy(void * object)
		{
			if constexpr (std::is_destructible_v<Type>) {
				((Type*)object)->~Type();
			}
		}
	};
	static type_info info;
	return info;
}

template <typename Type>
type_info const & type(Type const &)
{
	return type<Type>();
}

} // namespace library
