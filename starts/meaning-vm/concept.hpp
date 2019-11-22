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
	concept* operator->() { return ptr; }

	// for use by containers
	bool operator<(ref const & other) const { return ptr < other.ptr; }

	// for helpers
	ref(std::string const &);
	ref(char const * str) : ref(std::string(str)) { }
	ref(bool b) : ref(b ? "true" : "false") { }
	ref() : ref("nothing") { }
	value<std::string> & name() const;
	operator const char *() const;

	concept operator=(ref other); // helper constructs new concept with this as link
	ref operator[](concept links); // helper sets all links from passed concept

	bool isa(ref what) const;
	bool isan(ref what) const;

	concept * ptr;
};

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<ref,ref> links;
	using array = std::vector<ref>;

	ref id();
	bool linked(ref type);
	bool linked(ref type, ref target);
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
