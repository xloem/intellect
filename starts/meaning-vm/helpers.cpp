#include "helpers.hpp"

#include "memorystore.hpp"

#include <unordered_map>

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

concept ref::operator=(ref other)
{
	// if this is not anonymous, and other is, then we are naming it
	/*declrefs(anonymous);
	if (other->linked(anonymous, true) && !ptr->linked(anonymous, true)) {
		return;
	}*/
	// if this is link-type, make new concept [not checked, might want to assume]
	concept ret;
	ret.link(*this, other);
	return ret;
}

ref ref::operator[](concept links) {
	ptr->links.insert(links.links.begin(), links.links.end());
	return *this;
}

concept operator,(concept a, concept b)
{
	concept ret;
	ret.links.insert(a.links.begin(), a.links.end());
	ret.links.insert(b.links.begin(), b.links.end());
	return ret;
}

// concept names are for bootstrapping convenience,
// to make hardcoding structures easier.
// hence there is just one single list of them
std::unordered_map<vref<std::string>,ref,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;

struct name_t : public ref
{
	name_t();
} name;
name_t::name_t()
: ref(alloc())
{
	auto nam = valloc(std::string("name"));
	ptr->link(::name, nam);
	conceptsByName.emplace(nam, *this);
}

ref::ref(std::string const & s)
{
	value<std::string> str(s);
	auto res = conceptsByName.find(&str);
	if (res != conceptsByName.end()) {
		ptr = res->second.ptr;
	} else {
		ref con = alloc();
		auto nam = valloc<std::string>(s);
		conceptsByName.emplace(nam, con);
		con->link(::name, nam);
		ptr = con.ptr;
	}
}

value<std::string> & ref::name() const
{
	return *ptr->vget<std::string>(::name).ptr;
}

ref::operator const char *() const {
	return name().c_str();
}

ref a(ref what)
{
	static unsigned long long gid = 0;
	declrefs(is, anonymous);
	return ref(what.name() + "-" + std::to_string(gid++))[is = what, anonymous = true];
}
ref an(ref what)
{
	return a(what);
}

bool ref::isa(ref what) const
{
	declrefs(is);
	for (auto group : ptr->getAll(is)) {
		if (group == what) return true;
		if (group.isa(what)) return true;
	}
	return false;
}

bool ref::isan(ref what) const
{
	return isa(what);
}
