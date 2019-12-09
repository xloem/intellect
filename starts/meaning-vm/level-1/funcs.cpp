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
			level0::ref namestr = alloc((std::string)("name"));
			nameref.set(nameref, namestr);
			conceptsByName.emplace(namestr.val<std::string>(), nameref);
		}
	} namestruct;
	return namestruct;
}

void givename(concept* con, std::string const & name)
{
	auto & ns = namestruct();
	level0::ref namestr = level0::alloc(name);
	ns.conceptsByName.emplace(namestr.val<std::string>(), con);
	con.set(ns.nameref, namestr);
}

concept* getnamed(std::string const & name)
{
	auto & ns = namestruct();
	auto res = ns.conceptsByName.find(name);
	if (res != ns.conceptsByName.end()) {
		return res->second;
	} else {
		level1::ref con = level0::alloc();
		level0::ref namestr = level0::alloc(name);
		ns.conceptsByName.emplace(namestr.val<std::string>(), con);
		con.set(ns.nameref, namestr);
		return con.ptr();
	}
}

std::string& getname(concept* r)
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

concept* alloc(std::any val)
{

	ref ret = level0::alloc(val);
	std::stringstream ss;
	ss << val.type().name() << "(";
	if (false);
#define t(T) \
		else if (val.type() == typeid(T)) { \
			ss << ret.val<T>(); \
		}
	t(uint8_t) t(int8_t) t(uint16_t) t(int16_t)
	t(uint32_t) t(int32_t) t(uint64_t) t(int64_t)
	t(bool) t(float) t(double) t(std::string) t(char const *)
#undef t
	else { ss << "?"; }
	ss << ")";
	ret.link(concepts::name, level0::alloc(ss.str()));
	return ret;
}

concept* hyphenate(concept* a, concept* b)
{
	return getnamed(getname(a) + "-" + getname(b));
}

std::string dump(concept* what, concept* skipmarkertype, concept* skipmarkertarget)
{
	if (what->linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	std::string ret;
	for (auto & link : ref(what).links()) {
		if (link.first == concepts::name) { continue; }
		if (ret.size() == 0) {
			ret = ref(what).name() + ":\n";
		}
		ret += "  " + link.first.name() + ": " + link.second.name() + "\n";
	}
	what->link(skipmarkertype, skipmarkertarget);
	for (auto & link : ref(what).links()) {
		if (link.first.ptr() == skipmarkertype && link.second.ptr() == skipmarkertarget) {
			continue;
		}
		ret += dump(link.second, skipmarkertype, skipmarkertarget);
	}
	return ret;
}

}
}
