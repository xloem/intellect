#include "helpers.hpp"

#include <unordered_map>

ref operator-(ref a, ref b)
{
	return ref(a.name() + "-" + b.name());
}

concept ref::operator=(ref other)
{
	// if this is link-type, make new concept
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
concept namesByConcept;
std::unordered_map<value<std::string>,concept,std::hash<std::string>> conceptsByName;

ref::ref(std::string const & s)
{
	try {
		ptr = &conceptsByName.at(value<std::string>(s));
	} catch (std::out_of_range) {
		auto insertion = conceptsByName.emplace(value<std::string>(s), concept());
		ref con = &insertion.first->second;
		ref nam = const_cast<concept *>((concept const *)&insertion.first->first);
		namesByConcept.link(con, nam);
		ptr = con.ptr;
	}
}

value<std::string> & ref::name() const
{
	return value<std::string>::of(namesByConcept.get(*this));
}

ref::operator const char *() const {
	return name().c_str();
}

ref a(ref what)
{
	static unsigned long long gid = 0;
	declrefs(is);
	return ref(what.name() + "-" + std::to_string(gid++))[is = what];
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
