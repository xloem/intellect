#include "helpers.hpp"

#include "meaning.hpp"
#include "memorystore.hpp"

#include <unordered_map>

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

statementevaluable ref::operator=(ref that)
{
	// for single-line evaluation, we'll need to return a special object
	declrefs(assign, expression);
	decllnks(value, to);
	return {
		a(assign-expression)[
			to = *this,
			value = that
		],
		[](ref expr) {
			declrefs(link, target, unknown, source, type);
			decllnks(value, to);
			lnks(link-target, link-type, link-source);
			decllnks(name, is);
			ref lhs = expr.get(to);
			ref rhs = expr.get(value);
			if (lhs.isa(link) && lhs->get(link-target) == unknown) {
				// we are a link missing a target: our assignment is making the link happen
				lhs->unlink(link-target, unknown);
				lhs->link(link-target, rhs);
				ref src = lhs->get(link-source);
				if (lhs->get(link-type) != unknown && src != unknown) {
					src->link(lhs->get(link-type), rhs);
					dealloc(lhs);
					dealloc(expr);
				} else {
					throw std::logic_error("not sure what to do with incomplete link assignment");
				}
			} else if (rhs->linked(anonymous, true) && !lhs->linked(anonymous, true)) {
				// this is assignment of anonymous content to empty named concept
				bool donealready = false;
				for (auto & l : lhs->links) {
					if (ref(l.first) == name) { continue; }
					if (ref(l.first) == is && ref(l.second) == link-type) { continue; }
					donealready = true;
				}
				if (donealready) {
					// if we have links, and that has links we do not have, an error has been made
					for (auto & link : rhs->links) {
						if (ref(link.first) == anonymous) { continue; }
						if (ref(link.first) == name) { continue; }
						if (!lhs->linked(ref(link.first), ref(link.second))) {
							throw std::logic_error(lhs.name() + " already defined otherwise from " + rhs->get(is).name());
						}
					}
					donealready = true;
				}
				rhs->unlink(anonymous, true);
				auto nam = rhs->get(name);
				rhs->unlink(name, nam);
				if (!donealready) {
					ptr->links.insert(rhs->links.begin(), rhs->links.end());
				}
				rhs->link(name, nam);
				dealloc(rhs);
				dealloc(nam);
			} else {
				throw std::logic_error("unexpected bare assignment");
			}
		}
	}
/*
	decllnks(anonymous, is, name);
	declrefs(link, source, type, target, unknown);
	lnks(link-target, link-source, link-target);
	if (this->isa(link) && ptr->get(link-target) == unknown) {
	} else if (that->linked(anonymous, true) && !ptr->linked(anonymous, true)) {
		// TODO TODO: when left hand side is a link, and right hand side is anonymous,
		// 	especially when left hand side is link without other content,
		// 	there is no way to determine if user is providing content for link,
		//	or is making a link on another ref[a = b].
		//	instead, return just assignment information, and process in outer context.
		//	will want to make a ref subclass and assign in destructor unless has been used elsewhere.

	} else if (isa(link-type)) {
		// assignment to a link-type is likely inside a [type1=target1,type2=target2] expression
		// this happens after checking for name assignment because when name assignment
		// 	is redundant, the left-hand value could be a link
		return (*this) << that;
	}
	throw std::logic_error("unexpected use of assignment");
*/
}

ref ref::operator<<(ref target)
{
	// prep a link
	ref ret = alloc();
	ret->link(*this, target);
	return ret;
}

ref ref::operator[](ref expr) {
	declrefs(assign, expression);
	decllnks(value, to);
	declrefs(link, type, unknown);

	if (expr.isa(assign-expression)) {
	}
	expr.take();
	ref lhs = expr.get(to);
	ref rhs = expr.get(value);
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

vref<std::string> ref::name() const
{
	try {
		return ptr->vget<std::string>(::name, true);
	} catch (std::out_of_range) {
		declrefs(UNNAMED);
		return UNNAMED.name();
	}
}

ref::operator const char *() const {
	return name()->data.c_str();
}

ref a(ref what)
{
	static unsigned long long gid = 0;
	decllnks(is, anonymous);
	return ref(what.name() + "-" + std::to_string(gid++))[is = what, anonymous = true];
}
ref a(ref what, ref name)
{
	if (!name.isa(what)) {
		decllnks(is);
		name[is = what];
	}
	return name;
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
