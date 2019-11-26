#pragma once

#include "common.hpp"

#include <any>
#include <map>
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
};

}
}
