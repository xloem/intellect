#include "helpers.hpp"

#include <unordered_map>

// these concept names are for bootstrapping convenience,
// to make hardcoding structures easier.
// hence there is just one single list of them

concept namesByConcept;
std::unordered_map<value<std::string>,concept,std::hash<std::string>> conceptsByName;

ref::ref(std::string s)
{
	try {
		ptr = &conceptsByName.at(value<std::string>(s));
	} catch (std::out_of_range) {
		auto insertion = conceptsByName.emplace(value<std::string>(s), concept());
		ref con = &insertion.first->second;
		ref nam = const_cast<concept *>((concept const *)&insertion.first->first);
		namesByConcept.link(con, nam);
		ptr = con;
	}
}

ref::operator std::string()
{
	return value<std::string>::of(namesByConcept.get(*this));
}
