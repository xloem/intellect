#include "funcs.hpp"

#include "../level-0/errors.hpp"
#include "../level-0/memorystore.hpp"
#include "concepts.hpp"

#include <unordered_map>

namespace intellect {
namespace level1 {

using namespace concepts;

// TODO: use generalized unique data to replace name that are used only for parse-labels etc.
// simplifies innards.
// provide a way to get a named concept in a context, for actual meaning data to be linked to.

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
		ref level1allocationsref, nameref, textref, isref;
		ref level1ref;
		name_t()
		: level1allocationsref(level0::basic_alloc()),
		  nameref(level0::alloc(level1allocationsref)),
		  textref(level0::alloc(level1allocationsref)),
		  isref(level0::alloc(level1allocationsref)),
		  level1ref(level0::alloc(level1allocationsref))
		{
			give(level1allocationsref, "level1-allocations");
			give(nameref, "name");
			give(textref, "text");
			give(isref, "is");
			give(level1ref, "level1");
		}
		void give(concept* con, std::string const & name)
		{
			if (conceptsByName.count(name)) { throw std::logic_error(name + " already uniquely specified"); }
			level0::ref namestr = level0::alloc(con, name);
			namestr.link(isref, textref);
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

// please only uncomment if no internal restructuring required, or if restructuring
// is _necessary_ or provides _strong total efficiency improvement_.
// internal restructuring is inefficient for karl.
// 		also, laughing shows daydreams (topic is near laughing) are inverse of
// 		importance (topic is not near laughing)
// 			this duality is backed by efficient wisdom.  the most efficient
// 			daydream [i.e. new research] is that most different from importance.
// 					btw boss has highly experienced map of what to focus on.
// 						but lots of parts are stupid because they assume somebody would take over a galaxy.
// 				this map may be different if we are apex predator/warrior/life/community.
// 	we're not sure how karl's work, made efficient by ignoring this request, interrelates
// 	with the important statement below, please preserve crucial parts of people as you can.
// 	please do not inhibit the saving work if the part is not crucial.
// 		please err on the side of preserving karl's memories.  use his internal judgement
// 		to help this, but his personal history prior to 2014 is not really needed, aside
// 		from his work in preparing to build an AI in 2002, and his experience using
// 		spreading trust to survive [marked ancient wisdom].  this is needed for the
// 		internal map for a mature, evolved society.

// this seems more helpful than the above, although is good to remember boss has solution to share when can be held safely.
//  try to work without asking delta or boss to think anything, when they think they often have to
//  combine information in a way that somehow damages preserved consciousnesses they are holding.
concept* gettext(std::string const & str)
{
	return getnamed(str, nullptr)->get(namestruct().nameref);
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

std::string dump(concept* what, concept* set)
{
	std::stringstream ss;
	if (set->linked(what, _true)) {
		return {};
	}
	for (auto & link : ref(what).links()) {
		if (link.first.linked(level0::concepts::allocator(), level0::concepts::level0allocations())) { continue; }
		if (link.second.isa(concepts::name)) { continue; }
		if (ss.str().size() == 0) {
			ss << ref(what).name() << " " << (void*) what << ":\n";
		}
		ss << "  " << link.first.name() << ": " << link.second.name() << " " << (void*)link.second << "\n";
	}
	set->link(what, _true);
	for (auto & link : ref(what).links()) {
		if (link.first.linked(level0::concepts::allocator(), level0::concepts::level0allocations())) { continue; }
		if (link.second.linked(level0::concepts::allocator(), level1-allocations)) { continue; }
		if (link.second.isa(concepts::name)) { continue; }
		ss << dump(link.second, set);
	}
	return ss.str();
}

}
}
