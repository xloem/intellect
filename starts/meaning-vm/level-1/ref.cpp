#include "ref.hpp"
#include "../level-0/errors.hpp"
#include "../level-0/memorystore.hpp"
#include "../level-0/vref.hpp"
#include "concepts.hpp"
#include "vref.hpp"

#include <unordered_map>

using namespace intellect;
using namespace level1;
using namespace concepts;

// ensure name link and backing structure are created prior to first use
static auto & namestruct()
{
	static struct name_t
	{
		std::unordered_map<level0::vref<std::string>,level0::ref,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;
		ref nameref;
		name_t()
		: nameref(level0::alloc())
		{
			level0::vref namestr(std::string("name"));
			nameref.set(nameref, (level0::ref)namestr);
			conceptsByName.emplace(namestr, nameref.l0());
		}
	} namestruct;
	return namestruct;
}

level0::ref getnamed(std::string const & name)
{
	auto & ns = namestruct();
	auto res = ns.conceptsByName.find(name);
	if (res != ns.conceptsByName.end()) {
		return res->second;
	} else {
		level0::ref con = level0::alloc();
		level0::vref namestr(name);
		ns.conceptsByName.emplace(namestr, con);
		con.set(ns.nameref.l0(), namestr);
		return con;
	}
}

ref::ref(std::string const & name)
: baseref(getnamed(name).ptr())
{ }

vref<std::string> ref::name() const
{
	try {
		return vget<std::string>(namestruct().nameref);
	} catch(level0::no_such_link_type&) {
		return ref("UNNAMED").name();
	}
}

bool ref::isa(ref group) const
{
	for (auto & g : getAll(is)) {
		if (g == group) return true;
		if (g == self) continue;
		if (g.isa(group)) return true;
	}
	return false;
}

std::string ref::dump(ref skipmarkertype, ref skipmarkertarget)
{
	if (linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	std::string ret;
	for (auto & link : links()) {
		if (link.first == namestruct().nameref) { continue; }
		if (ret.size() == 0) {
			ret = name().val() + ":\n";
		}
		ret += "  " + link.first.name().val() + ": " + link.second.name().val() + "\n";
	}
	link(skipmarkertype, skipmarkertarget);
	for (auto & link : links()) {
		if (link.first == skipmarkertype && link.second == skipmarkertarget) {
			continue;
		}
		ret += link.second.dump(skipmarkertype, skipmarkertarget);
	}
	return ret;
}
