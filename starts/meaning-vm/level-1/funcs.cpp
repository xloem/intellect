#include "funcs.hpp"

#include "../level-0/errors.hpp"
#include "../level-0/memorystore.hpp"
#include "concepts.hpp"

#include <unordered_map>

namespace intellect {
namespace level1 {

using namespace concepts;

// for generalizing unique data references.  not used yet, could replace conceptsByName,
// but note this doesn't use a type link, and conceptsByName does.
template <typename T>
ref conceptByData(T const& data, concept* con = nullptr, concept* allocator = nullptr)
{
	static std::map<T, ref> conceptsByData; // std::map works for typeid data
	auto res = conceptsByData.find(data);
	if (res != conceptsByData.end()) {
		if (con != nullptr) { throw std::logic_error("unique data concept already specified"); }
		return res->second;
	} else {
		if (con == nullptr) {
			if (allocator == nullptr) { allocator = level0::concepts::allocations(); }
			con = level0::alloc(allocator);
		}
		conceptsByData.emplace(data, con);
		return con;
	}
}

// ensure name link and backing structure are created prior to first use
static auto & namestruct()
{
	static struct name_t
	{
		std::unordered_map<std::string,ref,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;
		ref level1allocationsref, nameref, isref;
		ref level1ref;
		name_t()
		: level1allocationsref(level0::basic_alloc()),
		  nameref(level0::alloc(level1allocationsref)),
		  isref(level0::alloc(level1allocationsref)),
		  level1ref(level0::alloc(level1allocationsref))
		{
			give(level1allocationsref, "level1-allocations");
			give(nameref, "name");
			give(isref, "is");
			give(level1ref, "level1");
		}
		void give(concept* con, std::string const & name)
		{
			if (conceptsByName.count(name)) { throw std::logic_error(name + " already uniquely specified"); }
			level0::ref namestr = level0::alloc(con, name);
			namestr.link(isref, nameref);
			conceptsByName.emplace(namestr.val<std::string>(), con);
			con->set(nameref, namestr);
		}
	} namestruct;
	return namestruct;
}

void givename(concept* con, std::string const & name)
{
	auto & ns = namestruct();
	ns.give(con, name);
}

concept* getnamed(std::string const & name, concept* allocator)
{
	auto & ns = namestruct();
	auto res = ns.conceptsByName.find(name);
	if (res != ns.conceptsByName.end()) {
		return res->second;
	} else {
		if (allocator == nullptr) { allocator = level0::concepts::allocations(); }
		level1::ref con = level0::alloc(allocator);
		givename(con, name);
		return con.ptr();
	}
}

std::string nameforany(concept* c)
{
	using namespace std;
	stringstream ss;
	static int fn_counter = 0;
	if (false);
#define t(T) \
		else if (c->data.type() == typeid(T)) { \
			ss << #T "(" << std::any_cast<T>(c->data) << ")"; \
		}
	t(uint8_t) t(int8_t) t(uint16_t) t(int16_t)
	t(uint32_t) t(int32_t) t(uint64_t) t(int64_t)
	t(bool) t(float) t(double) t(string) t(char const *)
#undef t
#define t(T) \
		else if (c->data.type() == typeid(T)) { \
			ss << #T "(" << (fn_counter++) << ")"; \
		}
	t(function<void(void)>)
	t(function<level0::ref(level0::ref)>)
	t(function<level1::ref(level1::ref)>)
	t(function<level2::ref(level2::ref)>)
	t(function<level3::ref(level3::ref)>)
	t(function<level4::ref(level4::ref)>)
	t(function<level5::ref(level5::ref)>)
	t(function<level6::ref(level6::ref)>)
	t(function<level7::ref(level7::ref)>)
	t(function<level8::ref(level8::ref)>)
	t(function<level9::ref(level9::ref)>)
#undef t
	else { ss << "?"; }
	return ss.str();
}

std::string getname(concept* r)
{
	try {
		return r->vget<std::string>(namestruct().nameref);
	} catch(level0::no_such_link_type&) {
		if (r->data.has_value()) { return nameforany(r); }
		return "UNNAMED";
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

concept* alloc(concept* allocator, std::any val)
{
	ref ret = level0::alloc(allocator, val);
	ref namestr = level0::alloc(ret, nameforany(ret));
	namestr.link(concepts::is, concepts::name);
	ret.link(concepts::name, level0::alloc(ret, nameforany(ret)));
	return ret;
}

concept* hyphenate(concept* a, concept* b)
{
	return getnamed(getname(a) + "-" + getname(b));
}

std::string dump(concept* what, concept* skipmarkertype, concept* skipmarkertarget)
{
	std::stringstream ss;
	if (what->linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	for (auto & link : ref(what).links()) {
		if (link.first.linked(level0::concepts::allocator(), level0::concepts::level0allocations())) { continue; }
		if (link.second.isa(concepts::name)) { continue; }
		if (ss.str().size() == 0) {
			ss << ref(what).name() << " " << (void*) what << ":\n";
		}
		ss << "  " << link.first.name() << ": " << link.second.name() << "\n";
	}
	what->link(skipmarkertype, skipmarkertarget);
	for (auto & link : ref(what).links()) {
		if (link.first.linked(allocator, level0-allocations)) { continue; }
		if (link.second.linked(allocator, level1-allocations)) { continue; }
		if (link.first.ptr() == skipmarkertype && link.second.ptr() == skipmarkertarget) {
			continue;
		}
		if (link.second.isa(concepts::name)) { continue; }
		ss << dump(link.second, skipmarkertype, skipmarkertarget);
	}
	return ss.str();
}

}
}
