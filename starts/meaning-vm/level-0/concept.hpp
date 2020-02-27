#pragma once

#include "common.hpp"

#include <any>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace intellect {
namespace level0 {

struct concept
{
	// a concept is made of concept-typed links to other concepts
	using links_t = std::unordered_multimap<concept*,concept*>;
	using linkcit = links_t::const_iterator;
	using linkit = linkcit; //links_t::iterator;
	links_t links;
	// and optional associated arbitrary data
	std::any data;


	concept();
	~concept() noexcept(false); // throws if referenced

	//using array = std::vector<concept*>;

	concept* id();

	void link(concept* type, concept* target);
	void unlink(concept* type, concept* target);
	void unlink(concept* type);
	linkit unlink(linkit it);
	void relink(linkit it, concept* target);

	bool crucial() { return iscrucial || crucialparts.size(); }
	bool crucial(concept* type, concept* target);
	bool crucial(linkit it) { return crucialparts.count(it); }
	void setcrucial() { iscrucial = true; }
	void setcrucial(concept* type, concept* target);
	void setcrucial(linkit it) { crucialparts.insert(it); }

	bool linked(concept* type) const;
	bool linked(concept* type, concept* target) const;

	struct array;
	array getAll(concept* type) const;

	// get and set enforce that only 1 link of a given type is present
	concept* get(concept* type) const;
	void set(concept* type, concept* target);

	template <typename T>
	T & vget(concept* type) const { return get(type)->val<T>(); }

	template <typename T>
	T & val() { return std::any_cast<T&>(data); }

	template <typename T>
	void val(T const & v) { data = v; }

	bool hasval() { return data.has_value(); }

	template <typename T>
	bool hasvalof() { return hasval() && data.type() == typeid(T); }

	size_t refcount() { return _refcount; }

	struct array {
		array() = default;
		array(array const &) = default;
		array(array &&) = default;
		array &operator=(array const &) = default;
		array(linkcit b, linkcit e)
		: itb(b), ite(e) { }
		struct iterator : public linkcit {
			iterator() = default;
			iterator(iterator const &) = default;
			iterator(iterator &&) = default;
			iterator& operator=(iterator const &) = default;
			iterator(linkcit it) : linkcit(it) { }
			iterator operator++() { return linkcit::operator++(); }
			iterator operator++(int i) { return linkcit::operator++(i); }
			//iterator operator--() { return linkit::operator--(); }
			//iterator operatir--(int i) { return linkit::operator--(i); }
			concept*const& operator*() { return linkcit::operator*().second; }
			concept*const* operator->() { return &linkcit::operator->()->second; }
		};
		iterator begin() { return itb; }
		iterator end() { return ite; }

		iterator itb, ite;
	};

private:
	size_t _refcount;
	// for permanence
	bool iscrucial;
	struct linksit_hash
	{
		size_t operator()(linkit const &it) const
		{
			return std::hash<decltype(&*it)>()(&*it);
		}
	};
	std::unordered_set<linkit, linksit_hash> crucialparts;
};

}
}
