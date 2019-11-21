#include "concept.hpp"

// Makes string values interchangeable with the concepts
//   they name.
template<>
struct value<std::string> : public concept, public std::string
{
	//using std::string::basic_string;
	value(std::string);
	value(cid);
	operator cid();
	static value& of(cid c)
	{
		return *static_cast<value*>(c);
	}
};

using str = value<std::string>;

cid operator"" _c(const char* str, std::size_t len)
{
	return value<std::string>({str,len});
}

#include <unordered_map>

concept namesByConcept;
std::unordered_map<value<std::string>,concept,std::hash<std::string>> conceptsByName;

value<std::string>::value(std::string s)
: std::string(s)
{ }

value<std::string>::value(cid c)
: std::string(of(namesByConcept.get(c)))
{ }

value<std::string>::operator cid()
{
	try {
		return &conceptsByName.at(*this);
	} catch (std::out_of_range) {
		auto insertion = conceptsByName.emplace(*this, concept());
		cid con = &insertion.first->second;
		cid nam = const_cast<cid>((const cid)&insertion.first->first);
		namesByConcept.link(con, nam);
		return con;
	}
}
