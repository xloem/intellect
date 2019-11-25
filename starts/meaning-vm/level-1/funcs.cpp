#include "funcs.hpp"

#include "../level-0/errors.hpp"
#include "../level-0/memorystore.hpp"
#include "concepts.hpp"

#include <unordered_map>

namespace intellect {
namespace level1 {

using namespace concepts;

// ensure name link and backing structure are created prior to first use
static auto & namestruct()
{
	static struct name_t
	{
		std::unordered_map<std::string,ref,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;
		ref nameref;
		name_t()
		: nameref(level0::alloc())
		{
			auto namestr = valloc<std::string>("name");
			nameref.set(nameref, namestr);
			conceptsByName.emplace(namestr->data, nameref);
		}
	} namestruct;
	return namestruct;
}

concept* getnamed(std::string const & name)
{
	auto & ns = namestruct();
	auto res = ns.conceptsByName.find(name);
	if (res != ns.conceptsByName.end()) {
		return res->second;
	} else {
		level1::ref con = level0::alloc();
		level0::value<std::string>* namestr = level0::valloc(name);
		ns.conceptsByName.emplace(namestr->data, con);
		con.set(ns.nameref.r0(), namestr);
		return con.ptr();
	}
}

value<std::string>* getname(concept* r)
{
	try {
		return r->vget<std::string>(namestruct().nameref);
	} catch(level0::no_such_link_type&) {
		return getname(ref("UNNAMED"));
	}
}

bool isa(concept* member, concept* group)
{
	for (auto & g : member->getAll(is)) {
		if (g == group) return true;
		if (g == member) continue;
		if (isa(g, group)) return true;
	}
	return false;
}

template <typename T>
value<T>* valloc(T const & val)
{
	auto ret = level0::valloc<T>(val);
	std::stringstream ss;
	// << val is making recursion
	ss << typeid(T).name() << "(" << val << ")";
	ret->link(concepts::name, level0::valloc(ss.str()));
	return ret;
}

template value<std::string>* valloc<std::string>(std::string const & val);
template value<int>* valloc<int>(int const & val);

}
}
