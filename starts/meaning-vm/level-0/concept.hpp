#pragma once

#include "common.hpp"

#include <any>
#include <map>
#include <unordered_set>
#include <vector>

namespace intellect {
namespace level0 {

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<concept*,concept*> links;
	// and optional associated arbitrary data
	std::any data;

	using array = std::vector<concept*>;

	concept* id();

	void link(concept* type, concept* target);
	void unlink(concept* type, concept* target);
	void unlink(concept* type);
	void unlink(decltype(links)::iterator it);

	bool crucial() { return iscrucial || crucialparts.size(); }
	bool crucial(concept* type, concept* target);
	bool crucial(decltype(links)::iterator it) { return crucialparts.count(it); }
	void setcrucial() { iscrucial = true; }
	void setcrucial(concept* type, concept* target);
	void setcrucial(decltype(links)::iterator it) { crucialparts.insert(it); }

	bool linked(concept* type) const;
	bool linked(concept* type, concept* target) const;

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

private:
	// for permanence
	bool iscrucial;
	struct linksit_hash
	{
		size_t operator()(decltype(links)::iterator const &it) const
		{
			return std::hash<decltype(&*it)>()(&*it);
		}
	};
	std::unordered_set<decltype(links)::iterator, linksit_hash> crucialparts;
};

}
}
