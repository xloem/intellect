#pragma once

#include "common.hpp"
#include "errors.hpp"

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

	void link(ref const & type, ref const & target) { p->link(type.p, target.p); }
	void unlink(ref const & type, ref const & target) { p->unlink(type.p, target.p); }
	void unlink(ref const & type) { p->unlink(type.p); }

	bool linked(ref const & type) const { return p->linked(type.p); }
	bool linked(ref const & type, ref const & target) const { return p->linked(type.p, target.p); }

	ref get(ref const & type) const { return p->get(type.p); }
	void set(ref const & type, ref const & target) { p->set(type.p, target.p); }

	array getAll(ref const & type) const;
	links_t links() const;

	template <typename T>
	T& vget(ref const & type) const { return p->vget<T>(type.p)->data; }

	template <typename T>
	T& val() { return p->val<T>()->data; }

	operator concept*() const { return p; }
	concept*& ptr() { return p; }
	concept* const & ptr() const { return p; }

	level0::ref & r0() { return *reinterpret_cast<level0::ref*>(this); }
	level1::ref & r1() { return *reinterpret_cast<level1::ref*>(this); }
	level2::ref & r2() { return *reinterpret_cast<level2::ref*>(this); }
	level3::ref & r3() { return *reinterpret_cast<level3::ref*>(this); }
	level4::ref & r4() { return *reinterpret_cast<level4::ref*>(this); }
	level5::ref & r5() { return *reinterpret_cast<level5::ref*>(this); }

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
