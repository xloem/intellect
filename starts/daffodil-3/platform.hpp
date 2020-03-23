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

// define an inline function that returns a far static reference
#if defined(__x86_64__)
#define define(Type, NAME)\
	strong_inline static Type & NAME() { return absolute_reference<Type, __COUNTER__>(#NAME); }
template <typename T, size_t id> __attribute__((always_inline))
strong_inline T & absolute_reference(char const *note)
{
	static T storage(note);
	T* pointer;
	asm("movabs %1,%0":"=r"(pointer):"g"(&storage):);
	return *pointer;
}
#endif
