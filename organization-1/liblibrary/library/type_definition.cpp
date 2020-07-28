#include <library/type.hpp>

#include <new>
#include <typeinfo>
#include <type_traits>

// for debugging
//#include <library/string.hpp>

namespace library {

#if defined(TYPE_ALLOCATION_DEBUG)
extern class allocation_debugger_t
{
public:
	allocation_debugger_t();
	~allocation_debugger_t() noexcept(false);
	void allocate(void const * pointer, type_info const & type);
	void access(void const * pointer, type_info const & type);
	void deallocate(void const * pointer, type_info const & type);
	unsigned long id(void const * pointer, type_info const & type);

private:
	void * storage;
	void * storage_2;
} allocation_debugger;
#else
static class allocation_debugger_t
{
public:
	allocation_debugger_t() {}
	~allocation_debugger_t() {}
	inline void allocate(void const * pointer, type_info const & type) {}
	inline void access(void const * pointer, type_info const & type) {}
	inline void deallocate(void const * pointer, type_info const & type) {}
} allocation_debugger;
#endif

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
				//stderr::line("construct default: "_s + placement + " as " + type<Type>().name);
				allocation_debugger.allocate(placement, type<Type>());
				new (placement) Type();
			}
		}

		static void _construct_copy(void * placement, void const * other)
		{
			if constexpr (std::is_copy_constructible_v<Type>) {
				//stderr::line("construct copy: "_s + placement + " from " + other + " as " + type<Type>().name);
				allocation_debugger.allocate(placement, type<Type>());
				allocation_debugger.access(other, type<Type>());
				new (placement) Type(*(Type const *)other);
			}
		}

		static void _assign(void * object, void const * other)
		{
			if constexpr (std::is_copy_assignable_v<Type>) {
				//stderr::line("assign: "_s + object + " from " + other + " as " + type<Type>().name);
				allocation_debugger.access(other, type<Type>());
				*(Type*)object = *(Type const *)other;
			}
		}

		static void _destroy(void * object)
		{
			if constexpr (std::is_destructible_v<Type>) {
				//stderr::line("destroy: "_s + object + " as " + type<Type>().name);
				allocation_debugger.deallocate(object, type<Type>());
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
Type const & typed_valued::reference() const
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return *(Type const *)void_pointer();
}

template <typename Type>
Type const * typed_valued::pointer() const
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return (Type const *)void_pointer();
}

template <typename Type>
Type & typed_valued::reference()
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return *(Type*)void_pointer();
}

template <typename Type>
Type * typed_valued::pointer()
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	return (Type*)void_pointer();
}

template <typename Type>
typed_valued & typed_valued::operator=(Type const & other)
{
	assign(other);
	return *this;
}

template <typename Type>
void typed_valued::assign(Type const & other)
{
	if (type() != library::type<Type>()) {
		throw type_mismatch();
	}
	assign((void const*)&other);
}

template <typename Type>
Type const & typable_valued::reference() const
{
	return *(Type const*)void_pointer(library::type<Type>());
}

template <typename Type>
Type const * typable_valued::pointer() const
{
	return (Type const*)void_pointer(library::type<Type>());
}

template <typename Type>
Type & typable_valued::reference()
{
	return *(Type*)void_pointer(library::type<Type>());
}

template <typename Type>
Type * typable_valued::pointer()
{
	return (Type*)void_pointer(library::type<Type>());
}

template <typename Type>
typable_valued & typable_valued::operator=(Type const & other)
{
	assign(other);
	return *this;
}

template <typename Type>
void typable_valued::assign(Type const & other)
{
	assign((void const*)&other, library::type<Type>());
}

} // namespace library
