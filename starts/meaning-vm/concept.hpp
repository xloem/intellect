#pragma once

#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using cid = struct concept *;

struct concept
{
	// a concept is made of concept-typed links to other concepts
	std::multimap<cid,cid> links;
	using array = std::vector<cid>;

	cid id();
	cid get(cid type); // returns first
	array getAll(cid type);
	void link(cid type, cid target);
	void unlink(cid type, cid target);
};

template <typename T>
struct value : public concept, public T
{
	static value<T>& of(cid c)
	{
		return *static_cast<value<T>*>(c);
	}
};

cid concept::id()
{
	return this;
}

cid concept::get(cid type)
{
	auto result = links.equal_range(type);
	if (result.first == result.second) {
		throw std::out_of_range("no such concept link to get");
	}
	return result.first->second;
}

concept::array concept::getAll(cid type)
{
	array ret;
	for (
		auto range = links.equal_range(type);
		range.first != range.second;
		++ range.first
	) { 
		ret.push_back(range.first->second);
	}
	return ret;
}

void concept::link(cid type, cid target)
{
	links.insert({type, target});
}

void concept::unlink(cid type, cid target)
{
	auto ls = links.equal_range(type);
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target) {
			links.erase(l);
			return;
		}
	}
	throw std::out_of_range("no such concept link to erase");
}
