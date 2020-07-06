#include <library/type.hpp>

#include <new>
#include <typeinfo>
#include <type_traits>

namespace library {

inline bool type_info::operator<(const type_info & other) const
{
	return std < other.std;
}

inline bool type_info::operator<=(const type_info & other) const
{
	return std <= other.std;
}

inline bool type_info::operator==(const type_info & other) const
{
	return *std == *other.std;
}

inline bool type_info::operator!=(const type_info & other) const
{
	return *std != *other.std;
}

inline bool type_info::operator>=(const type_info & other) const
{
	return std >= other.std;
}

inline bool type_info::operator>(const type_info & other) const
{
	return std > other.std;
}

template <typename Type>
constexpr void const * const type_id = &type_id<Type>;

// for quickly crafting solutions to recursion problems
template <typename identifier = void, auto other_identifier = 0>
struct recursion_detector {
	recursion_detector() {
		++ (_depth);
	}
	unsigned depth() {
		return _depth - 1;
	}
	~recursion_detector() {
		-- (_depth);
	}
private:
	static thread_local unsigned _depth;
};
template <typename identifier, auto other_identifier>
thread_local unsigned recursion_detector<identifier, other_identifier>::_depth = 0;

template <typename Type>
type_info const & type()
{
	struct type_info : public library::type_info
	{
		type_info()
		{
			std = &typeid(Type);
			id = type_id<Type>;
			if constexpr (!std::is_same_v<std::remove_const_t<Type>, void>) {
				size = sizeof(Type);
			} else {
				size = 0;
			}
			name = std->name();

			if constexpr (std::is_const_v<Type>) {
				is_const = true; is_mutable = false;
				as_const = this;
				recursion_detector<library::type_info> stack;
				if (stack.depth()) {
					as_mutable = &type<std::remove_const_t<Type>>();
					const_cast<library::type_info&>(*as_mutable).as_const = this;
				}
			} else {
				is_const = false; is_mutable = true;
				as_mutable = this;
				recursion_detector<library::type_info> stack;
				if (stack.depth()){
					as_const = &type<const Type>();
					const_cast<library::type_info&>(*as_const).as_mutable = this;
				}
			}
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

template <typename Type>
Type const & typed_const::reference() const
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return *(Type const*)void_pointer();
}

template <typename Type>
Type & typed_mutable::reference()
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return *(Type*)void_pointer();
}

template <typename Type>
typed_mutable & typed_mutable::operator=(Type const & other)
{
	assign(other);
	return *this;
}

template <typename Type>
void typed_mutable::assign(Type const & other)
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	assign((void*)&other);
}

template <typename Type>
typed_retypable & typed_retypable::operator=(Type const & other)
{
	assign(other);
	return *this;
}

template <typename Type>
void typed_retypable::assign(Type const & other)
{
	assign((void const*)&other, library::type<Type>());
}

} // namespace library
