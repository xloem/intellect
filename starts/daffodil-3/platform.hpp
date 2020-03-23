#pragma once

// define(Type, NAME) defines a function called NAME that returns an absolute reference to a unique static Type

#include <stdio.h>

#if defined(__x86_64__)
#define define(Type, NAME)\
	__attribute__((always_inline))\
	inline Type & NAME() { return absolute_reference<Type, __COUNTER__>(#NAME); }
template <typename T, size_t id> __attribute__((always_inline))
inline T & absolute_reference(char const *note)
{
	static T storage(note);
	T* pointer;
	asm("movabs %1,%0":"=r"(pointer):"g"(&storage):);
	return *pointer;
}
#endif

