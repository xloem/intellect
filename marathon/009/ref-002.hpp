#pragma once

#include <any>
#include <unordered_map>
#include <memory>

// a single map instead of a multi map is used.
// observation implies that implementing support may be more code than the use of that support.

using r = std::initializer_list<std::pair<ref,std::any>>;
using refdata = std::unordered_map<ref,std::any>;


class ref : public std::shared_ptr<refdata>
{
public:
	ref(r refs = {});

	std::any & data(ref what);
	template<class t> t & get(ref what) { return *std::any_cast<t>(&data(what)); }

	ref get(ref what);
	void set(ref what, std::any to);


	static ref selfref;

private:
	ref();
};


#define symbol(name) ref name({{::sym::text, std::string(#name));
namespace sym {
	ref text({{ref::selfref, std::string("text")}});
	symbol(nothing);
	symbol(is);
}

#define self (*this)
ref::ref(r refs)
{
	for (auto & r : refs) {
		if (r.first == selfref) {
			r.first = self;
		}
		if (r.second == selfref) {
			r.second = self;
		}
	}
	self = new refdata(refs);
}

ref ref::get(ref what)
{
	std::any & result = self->find(what);
	if (result == self->end()) {
		return sym::nothing;
	} else {
		return std::any_cast<ref>(result);
	}
}

void ref::set(ref what, ref to)
{
}
