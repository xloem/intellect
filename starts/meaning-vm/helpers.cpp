#include "helpers.hpp"

#include "meaning.hpp"
#include "memorystore.hpp"

#include <unordered_map>

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

ref ref::operator=(ref that)
{
	decllnks(anonymous, is, name);
	declrefs(link, source, type, target, unknown);
	lnks(link-target, link-source, link-target);
	if (this->isa(link) && ptr->get(link-target) == unknown) {
		// we are a link missing a target: our assignment is making the link happen
		ptr->unlink(link-target, unknown);
		ptr->link(link-target, that);
		ref src = ptr->get(link-source);
		if (ptr->get(link-type) != unknown && src != unknown) {
			src->link(ptr->get(link-type), ptr->get(link-target));
			dealloc(ptr);
			return src;
		} else {
			throw std::logic_error("not sure what to do with incomplete link assignment");
		}
	} else if (isa(link-type)) {
		// assignment to a link-type is likely inside a [type1=target1,type2=target2] expression
		return (*this) << that;
	} else if (that->linked(anonymous, true) && !ptr->linked(anonymous, true)) {
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
		if (!donealready) {
			ptr->links.insert(that->links.begin(), that->links.end());
		}
		that->link(name, nam);
		dealloc(that);
		dealloc(nam);
		return *this;
	}
	throw std::logic_error("unexpected use of assignment");
}

ref ref::operator<<(ref target)
{
	// prep a link
	ref ret = alloc();
	ret->link(*this, target);
	return ret;
}

ref ref::operator[](ref that) {
	declrefs(link, type, unknown);
	if (that.isa(link-type)) {
		return ::link(*this, that, unknown);
	} else {
		ptr->links.insert(that->links.begin(), that->links.end());
		dealloc(that);
		return *this;
	}
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

template <>
vref<std::string>::vref(std::string const & s)
: ptr(valloc(s).ptr)
{
	ptr->link(::name, ptr);
}
name_t::name_t()
: ref(alloc())
{
	vref nam(std::string("name"));
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
		vref<std::string> nam(s);
		conceptsByName.emplace(nam, con.ptr);
		con->link(::name, nam);
		ptr = con.ptr;
	}
}

value<std::string> & ref::name() const
{
	try {
		return *ptr->vget<std::string>(::name, true).ptr;
	} catch (std::out_of_range) {
		declrefs(UNNAMED);
		return UNNAMED.name();
	}
}

ref::operator const char *() const {
	return name().c_str();
}

ref a(ref what)
{
	static unsigned long long gid = 0;
	decllnks(is, anonymous);
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
		if (group == *this) continue;
		if (group.isa(what)) return true;
	}
	return false;
}

bool ref::isan(ref what) const
{
	return isa(what);
}
