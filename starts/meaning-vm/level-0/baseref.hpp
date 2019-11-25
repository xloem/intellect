#pragma once

#include "common.hpp"
#include "errors.hpp"

#include <map>
#include <vector>

namespace intellect {
namespace level0 {

template <typename ref, template<typename> typename vref, typename concept>
class baseref {
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
	vref<T> vget(ref const & type) const { return p->template vget<T>(type.p); }

	template <typename T>
	vref<T> val() { return p->template val<T>(); }

	operator concept*() const { return p; }
	concept*& ptr() { return p; }
	concept* const & ptr() const { return p; }

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

template <typename ref, template<typename> typename vref, typename concept>
typename baseref<ref,vref,concept>::array baseref<ref,vref,concept>::getAll(ref const & type) const
{
	return {p->getAll(type.p)};
}
template <typename ref, template<typename> typename vref, typename concept>
typename baseref<ref,vref,concept>::links_t baseref<ref,vref,concept>::links() const
{
	return {p->links};
}

}
}
