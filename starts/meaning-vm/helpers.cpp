#include "helpers.hpp"

#include "memorystore.hpp"

#include <unordered_map>

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

ref ref::operator=(ref that)
{
	// if this is not anonymous, and that is, then we are naming it
	declrefs(anonymous, name, is);
	if (that->linked(anonymous, true) && !ptr->linked(anonymous, true)) {
		// this is assignment of anonymous content to empty named concept
		bool donealready = false;
		if (ptr->links.size() != 1) {
			// if we have links, and that has links we do not have, an error has been made
			for (auto & link : that->links) {
				if (ref(link.first) == anonymous) { continue; }
				if (ref(link.first) == name) { continue; }
				if (!ptr->linked(ref(link.first), ref(link.second))) {
					throw std::logic_error(this->name() + " already defined otherwise from " + that->get(is).name());
				}
			}
			donealready = true;
		}
		that->unlink(anonymous, true);
		auto nam = that->get(name);
		that->unlink(name, nam);
		dealloc(nam);
		if (!donealready) {
			ptr->links.insert(that->links.begin(), that->links.end());
		}
		dealloc(that);
		return *this;
	}

	// if this is link-type, make new concept [not checked, might want to assume]
	ref ret = alloc();
	ret->link(*this, that);
	return ret;
}

ref ref::operator[](ref links) {
	ptr->links.insert(links->links.begin(), links->links.end());
	dealloc(links);
	return *this;
}

ref operator,(ref a, ref b)
{
	a->links.insert(b->links.begin(), b->links.end());
	dealloc(b);
	return a;
}

// concept names are for bootstrapping convenience,
// to make hardcoding structures easier.
// hence there is just one single list of them
std::unordered_map<std::string,concept*,std::hash<std::string>,std::equal_to<std::string>> conceptsByName;

struct name_t : public ref
{
	name_t();
} name;
name_t::name_t()
: ref(alloc())
{
	auto nam = valloc(std::string("name"));
	ptr->link(::name, nam);
	conceptsByName.emplace(nam, ptr);
}

ref::ref(std::string const & s)
{
	auto res = conceptsByName.find(s);
	if (res != conceptsByName.end()) {
		ptr = res->second;
	} else {
		ref con = alloc();
		auto nam = valloc<std::string>(s);
		conceptsByName.emplace(nam, con.ptr);
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
ref a(ref what, ref name)
{
	return name = a(what);
}
ref an(ref what)
{
	return a(what);
}
ref an(ref what, ref name)
{
	return a(what, name);
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
