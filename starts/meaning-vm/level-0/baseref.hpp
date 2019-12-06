#pragma once

#include "common.hpp"
#include "errors.hpp"
#include "memorystore.hpp"

#include <map>
#include <vector>

namespace intellect {
namespace level0 {

template <typename ref>
class baseref
{
	struct array; struct links_t;
public:
	baseref(concept *p)
	: p(p)
	{
		if (p == 0) {
			throw null_reference();
		}
	}

	baseref & operator=(concept *p)
	{
		self.p = p;
		return self;
	}

	void link(ref const & type, ref const & target) { p->link(type.p, target.p); }
	void unlink(ref const & type, ref const & target) { p->unlink(type.p, target.p); }
	void unlink(ref const & type) { p->unlink(type.p); }

	bool linked(ref const & type) const { return p->linked(type.p); }
	bool linked(ref const & type, ref const & target) const { return p->linked(type.p, target.p); }

	ref get(ref const & type) const { return p->get(type.p); }
	void set(ref const & type, ref const & target) { p->set(type.p, target.p); }

	array getAll(ref const & type) const;
	links_t links() const;

	template <typename... Ref>
	ref link(Ref... refspack)
	{
		std::initializer_list<ref> refs{refspack...};
		for (auto it = refs.begin(); it != refs.end();) {
			ref type = *it++;
			ref target = *it++;
			link(type, target);
		}
		return ptr();
	}

	template <typename T>
	T& vget(ref const & type) const { return p->vget<T>(type.p); }
	template <typename T>
	void vset(ref const & type, T const & v) { p->set(type.p, level0::alloc(v)); }

	template <typename T>
	T& val() { return p->val<T>(); }
	template <typename T>
	void val(T const & v) { p->val<T>(v); }

	operator concept*() const { return p; }
	concept*& ptr() { return p; }
	concept* const & ptr() const { return p; }

	operator level0::ref &() { return *reinterpret_cast<level0::ref*>(this); }
	operator level1::ref &() { return *reinterpret_cast<level1::ref*>(this); }
	operator level2::ref &() { return *reinterpret_cast<level2::ref*>(this); }
	operator level3::ref &() { return *reinterpret_cast<level3::ref*>(this); }
	operator level4::ref &() { return *reinterpret_cast<level4::ref*>(this); }
	operator level5::ref &() { return *reinterpret_cast<level5::ref*>(this); }

	bool operator==(ref const & other) const { return self.p == other.p; }
	bool operator!=(ref const & other) const { return self.p == other.p; }
	bool operator<(ref const & other) const { return self.p < other.p; }

protected:
	concept * p;

private:
	template <typename val, typename It>
	struct mutated_it
	{
		mutated_it(It const & it) : it(it) { }

		using mutit = mutated_it<val, It>;

		mutit & operator++() { ++ self.it; return self; }
		mutit operator++(int i) { return self.it.operator++(i); }
		bool operator==(mutit const & other) const { return self.it == other.it; }
		bool operator!=(mutit const & other) const { return self.it != other.it; }

		val & operator*() { return *(val*)&self.it.operator*(); }
		val & operator->() { return *(val*)&self.it.operator->(); }

	private:
		It it;
	};

	struct array
	{
		mutated_it<ref,typename concept::array::iterator> begin() { return array.begin(); }
		mutated_it<ref,typename concept::array::iterator> end() { return array.end(); }
	
		typename concept::array array;
	};

	struct links_t
	{
		mutated_it<std::pair<ref,ref>,typename decltype(concept::links)::iterator> begin()
		{ return links.begin(); }
		mutated_it<std::pair<ref,ref>,typename decltype(concept::links)::iterator> end()
		{ return links.end(); }

		decltype(concept::links) & links;
	};
};

template <typename ref>
typename baseref<ref>::array baseref<ref>::getAll(ref const & type) const
{
	return {p->getAll(type.p)};
}
template <typename ref>
typename baseref<ref>::links_t baseref<ref>::links() const
{
	return {p->links};
}

}
}
