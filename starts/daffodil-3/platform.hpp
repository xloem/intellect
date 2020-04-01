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
	static auto static_call(class_type & object, value_types... values)
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
