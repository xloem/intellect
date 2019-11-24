#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

template <typename T> struct vref;
struct concept;
template <typename T> struct value;
class statementevaluable;

struct ref
{
	ref(concept *p) : ptr(p) { if (p == 0) throw std::logic_error("null reference"); }
	ref(ref const &) = default;
	concept* operator->() { return ptr; }
	bool operator==(ref const & that) const { return this->ptr == that.ptr; }
	bool operator!=(ref const & that) const { return this->ptr != that.ptr; }

	bool operator<(ref const &) const { throw std::logic_error("ref has redefined syntax sugar: do not use in containers"); }

	// for helpers, mostly names
	ref(std::string const &);
	ref(char const * str) : ref(std::string(str)) { }
	ref(bool b) : ref(b ? "true" : "false") { }
	ref() : ref("nothing") { }
	vref<std::string> name() const;
	operator const char *() const;

	// helper linking syntax sugar
	statementevaluable operator=(ref that);
	ref operator<<(ref target);
	ref operator[](ref links);

	bool isa(ref what) const;
	bool isan(ref what) const;

	concept * ptr;
};

template <typename T>
struct vref
{
	vref(value<T> *p) : ptr(p) { }
	value<T>* operator->() { return ptr; }
	operator T const &() const { return *ptr; }

	vref(T const & val);

	vref(ref const & that) : ptr(static_cast<value<T>*>(that.ptr)) { }
	operator ref() { return ptr; }

	// for use by containers
	//bool operator<(ref const & that) const { return ptr < that.ptr; }

	value<T> * ptr;
};

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<concept*,concept*> links;
	using array = std::vector<ref>;

	ref id();
	bool linked(ref type);
	bool linked(ref type, ref target);
	ref get(ref type, bool quick = false); // returns first
	template <typename T>
	vref<T> vget(ref type, bool quick = false) { return get(type, quick); }
	array getAll(ref type);
	void link(ref type, ref target);
	void unlink(ref type, ref target);
	void unlink(ref type);
};

template <typename T>
struct value : public concept
{
	value(T const & val) : data(val) { }
	value(value<T> const & val) = default;
	static value<T>& of(ref c)
	{
		return *static_cast<value<T>*>(c.ptr);
	}

	operator T&() { return data; }
	operator T const &() const { return data; }

	T data;
};
