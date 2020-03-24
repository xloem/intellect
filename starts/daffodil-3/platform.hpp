#pragma once

// Compiled code has 'far' and 'near' pointers/references.
// The deafult is 'near' references, which are written as relative
// to where in the code they are, so they break if the code is
// moved for trying changes.

// To avoid near references, static functions, static variables,
// and non-virtual member functions, must not be used directly.

// Function pointers must be to static functions, as member
// function pointers don't have a way to identify location from
// value.


// force something to be inline so it is not near-called
#define strong_inline __attribute__((always_inline)) inline

// get a far static reference
#if defined(__x86_64__)
template <typename T>
strong_inline T & far_static_reference(T & static_object)
{
	T * pointer;
	asm("movabs %1,%0":"=r"(pointer):"g"(&static_object):);
	return *pointer;
}
#endif

// define an inline function that returns a far static reference to a new static object
#define define(Type, NAME)\
	strong_inline static Type & NAME() { return far_static_storage<Type, __COUNTER__>(#NAME); }
template <typename T, size_t id> __attribute__((always_inline))
strong_inline T & far_static_storage(char const *note)
{
	static T storage(note);
	return far_static_reference(storage);
}

// this calls a non-virtual member function using only far calls.
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
