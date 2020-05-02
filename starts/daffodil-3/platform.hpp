#pragma once

// Compiled code has 'far' and 'near' pointers/references.
// The default is 'near' references, which are written as relative
// to where in the code they are, so they break if the code is
// moved for trying changes.

// To avoid near references, static functions, static variables,
// and non-virtual member functions, must not be used directly.


// force something to be inline so it is not near-called
#define strong_inline __attribute__((always_inline)) inline

// get a far static reference
template <auto original_pointer>
strong_inline auto far_pointer()
{
	decltype(original_pointer) pointer;
	#if defined(__x86_64__)
	asm("movabs %1,%0":"=r"(pointer):"g"(original_pointer):);
	#elif defined(__i386__)
		#if defined(__PIC__) || defined(__PIE__)
			#error "please use -fno-pic to remove the near-thunk in functions"
		#endif
	asm("mov %1, %0":"=r"(pointer):"g"(original_pointer):);
	#elif defined(__aarch64__)
	// this is simplest using a musl toolchain.  glibc doesn't support it well.
	// you can get a musl (pronounced 'muscle') toolchain at musl.cc
	// link the binary aarch64-linux-musl-g++ from it into your path.
	asm(
		"movz %0, #:abs_g0_nc:%1\n"
		"movk %0, #:abs_g1:%1\n"
		"movk %0, #:abs_g2:%1\n" // likely not needed (second highest dword, often 0)
		//"movk %0, #:abs_g3:%1\n" // not needed (highest dword, generally zero)
		:"=r"(pointer):"S"(original_pointer):);
	#elif defined(__arm__)
	// for now i have tried building a musl toolchain to do 32bit arm.
	// the problem for <armv6t2 appears like it would prefer a different approach.
	// it uses lower16 and upper16 instead of abs_g0_nc etc
	// movw for lower, movt for upper, requires >=armv6t2
		#error "far references unimplemented on this platform; add an assembly statement here"
	#else
		#error "far references unimplemented on this platform; add an assembly statement here"
	#endif
	return pointer;
}

// make a far static reference to a string
template <typename Letter, Letter... letters>
strong_inline Letter const * operator""_far()
{
	static Letter storage[] = { letters ..., 0 };
	return far_static_reference(storage);
}

// define an inline function that returns a far static reference to a new static object
#define define(Type, NAME)\
	strong_inline static Type & NAME() { return far_static_storage<Type, __COUNTER__>(#NAME); }
template <typename T, size_t id> __attribute__((always_inline))
strong_inline T & far_static_storage(char const *note)
{
	static T storage(note);
	return far_static_reference(storage);
}

// template class that calls constructor and destructor through far pointers
template <typename> class far_object;
template <typename Class> strong_inline void* operator new(size_t, far_object<Class> * pointer) { return pointer; } // prevents near call to new inside
template <typename _Class>
class far_object
{
public:
	using Class = _Class;

	template<typename... value_types>
	strong_inline far_object(value_types... values)
	{
		far_static_reference(construct<value_types...>)(this, values...);
	}
	strong_inline ~far_object()
	{
		far_static_reference(destruct)(this);
	}

	strong_inline operator Class &() { return reference(); }
	strong_inline Class & reference() { return *pointer(); }
	strong_inline Class * pointer() { return (Class*)storage; }
private:
	uint8_t storage[sizeof(Class)];

	template<typename... value_types>
	static void construct(far_object<Class> * pointer, value_types... values)
	{ new(pointer) Class(values...); }

	static void destruct(far_object<Class> * pointer)
	{ pointer->reference().~Class(); }
};

// converts member function pointer to pointer to static wrapping function
template <auto member_function_pointer>
struct member_function
{
private:
	template<typename class_type, typename... value_types>
	static auto static_call(class_type & object, value_types... values) noexcept // noexcept simplifies, and removes near call
	{
		return (object.*member_function_pointer)(values...);
	}

	template <typename class_type, typename return_type, typename... value_types>
	static constexpr auto member_to_static(return_type(class_type::*)(value_types...))
	{
		return static_call<class_type, value_types...>;
	}
public:

	static constexpr auto near_pointer = member_to_static(member_function_pointer);
	using type = decltype(near_pointer);
};

// this calls a non-virtual member function using only far calls.
#define far_member_call(object, method, ...) \
	far_static_reference(member_function<&decltype(object)::method>::near_pointer)(object,##__VA_ARGS__)
#define far_object_call(object, method, ...) \
	far_static_reference(member_function<&decltype(object)::Class::method>::near_pointer)(object,##__VA_ARGS__)
/*
// TODO: separate out the conversion from member-pointer to function-pointer
#define far_member_call(object, method, ...) far_member_dispatch<&decltype(object)::method>(object,##__VA_ARGS__)
template <auto member_pointer, typename class_type, typename... value_types>
__attribute__((always_inline)) inline
auto far_member_dispatch(class_type & object, value_types... values)
{
	using member_pointer_type = decltype(member_pointer);
	using class_pointer_type = class_type *;
	using return_type = decltype((static_cast<class_pointer_type>(0)->*member_pointer)(*static_cast<value_types*>(0)...));

	struct near
	{
		static return_type dispatch(class_type & object, value_types... values)
		{
			return (object.*member_pointer)(values...);
		}
	};

	return far_static_reference(near::dispatch)(object, values...);
}
*/
