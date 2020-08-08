#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include <unordered_set>

template <class t>
using il = std::initializer_list<t>;

// holds concept data
struct con;

// refers to concept data, can be copied lightweight
struct ref : public std::shared_ptr<con>
{
	ref(il<il<ref>> refs = {}, std::any data = {});

	void add_all(ref refs);
	ref get(ref what);
};

#define symbol(name) ref name{{}, std::string(#name)}

namespace sym {
	symbol(nothing);
}

// holds concept data, rarely used directly
struct con
{
	std::multimap<ref,ref> refs;
	std::any data;
};

using exception = ref;

#define self (*this)

ref::ref(il<il<ref>> initial_refs, std::any data)
: shared_ptr(new ::con{{}, data})
{
	auto & refs = self->refs;
	for (std::vector<ref> link : initial_refs) {
		switch (link.size()) {
		case 1:
			refs.insert(std::pair<ref,ref>(link[0], link[0]));
		case 2:
			refs.insert(std::pair<ref,ref>(link[0], link[1]));
		}
	}
}

ref ref::get(ref what)
{
	auto range = self->refs;
	// ...
	return sym::nothing;
}

void ref::add_all(ref refs)
{
	self->refs.insert(self->refs.begin(), self->refs.end());
}

#undef self
