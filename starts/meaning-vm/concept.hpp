#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct concept;
template <typename T> struct value;

struct ref
{
	ref(concept *p) : ptr(p) { }
	operator concept*() const { return ptr; }

	// helper names
	ref(std::string);
	operator std::string();
	ref(char const * str) : ref(std::string(str)) { }

	concept * ptr;
};

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<ref,ref,std::less<concept*>> links;
	using array = std::vector<ref>;

	ref id();
	ref get(ref type); // returns first
	array getAll(ref type);
	void link(ref type, ref target);
	void unlink(ref type, ref target);
};

template <typename T>
struct value : public concept, public T
{
	value(T const & val) : T(val) {}
	value(value<T> const & val) = default;
	static value<T>& of(ref c)
	{
		return *static_cast<value<T>*>(c.ptr);
	}
};
