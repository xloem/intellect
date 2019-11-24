#pragma once

#include "common.hpp"
#include "ref.hpp"

#include <map>
#include <vector>

namespace intellect {
namespace level0 {

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<ref,ref> links;
	using array = std::vector<ref>;

	ref id();

	void link(ref const & type, ref const & target);
	void unlink(ref const & type, ref const & target);
	void unlink(ref const & type);

	bool linked(ref const & type) const;
	bool linked(ref const & type, ref const & target) const;

	array getAll(ref const & type) const;

	// get and set enforce that only 1 link of a given type is present
	ref get(ref const & type) const;
	void set(ref const & type, ref const & target);

	template <typename T>
	vref<T> vget(ref const & type) const { return get(type); }
};

}
}
