#pragma once

// Provides for syntax sugar easing hardcoding.
// 1. concept names
// 	use declrefs(apple, is, fruit, water, has)
// 	to quickly define the named ref variables,
// 	referring always to the same named concept.
// 	the ref constructor provides for using
// 	'true' and 'false' as refs.
// 2. links
// 	links may be quickly made with
// 	apple[is = fruit, has = water]
// 	which links 'apple' by 'is' to 'fruit'
// 	and by 'has' to 'water' in one line
// 3. hyphenation
// 	refs may be hyphenated (subtracted) to
// 	produce a new ref with the conglomerate name
// 	apple-has-water.name(): "apple-has-water"
// 	this may be expanded upon.
// 4. anonymous concept creation
// 	an unnamed instance of a concept may be created with the a() or an() equivalent functions:
// 	  ref anongreenapple = an(apple)[color = green];
// 	it will be given a name such as apple-3
// 5. quick group checking
// 	the obverse of the a()/an() functions is provided
// 	by ref::isa() and ref::isan() which quickly check
// 	if there is an "is" link to the passed ref.

#include "concept.hpp"
#include "memorystore.hpp"

#include <sstream>
#include <functional>

inline std::string operator+(vref<std::string> a, char const * b) { return std::string(a) + b; }
inline std::string operator+(vref<std::string> a, std::string b) { return std::string(a) + b; }
inline std::string operator+(char const * a, vref<std::string> b) { return a + std::string(b); }
inline std::string operator+(std::string a, vref<std::string> b) { return a + std::string(b); }

// reasons to use special struct:
//   the reason not to use ref directly is because we want to pass a ref to the function handler,
//   and it is easier to use if it is a bare copy.  the bare copy triggers destruction and eval.
// 	an alternate would be to add an evaluation flag to ref, but this might make it big.
// reasons to not use special struct:
//   the operator, takes a ref type and returns a ref type
struct statementcallref
{
	statementcallref(ref type, std::function<void(ref)> func);
	statementcallref(ref const & that);

	// handles evaluating refs as statements
	~statementcallref();
	statementcallref(statementcallref & that);
	statementcallref(statementcallref && that) noexcept;

	operator ref();
	
	void destatement();
	
	ref r;
};

template <>
vref<std::string>::vref(std::string const &);

template <typename T>
vref<T>::vref(T const & v)
: ptr(valloc(v).ptr)
#include "memorystore.hpp"
{
	ptr->link(ref("name"), vref(std::to_string(v)));
}


template <typename... T>
void __helper_init_ref_names(std::string names, T &... refrefs)
{
	std::stringstream ss(names);
	ref* refptrs[] = {&refrefs...};
	for (std::size_t i = 0; i < sizeof...(refrefs); ++ i) {
		std::string name;
		ss >> name;
		if (name[name.size() - 1] == ',') {
			name = name.substr(0, name.size() - 1);
		}
		refptrs[i]->ptr = ref(name).ptr;
	}
}

#define declrefs(...) \
	ref __VA_ARGS__; \
	__helper_init_ref_names(#__VA_ARGS__, __VA_ARGS__)

template <typename... T>
void lnks(T ... passedrefs)
{
	std::initializer_list<ref> refs = { passedrefs... };
	declrefs(link, type, is);
	for (ref r : refs) {
		if (!r->linked(is, link-type)) {
			r->link(is, link-type);
		}
	}
}

#define decllnks(...) \
	declrefs(__VA_ARGS__); \
	lnks(__VA_ARGS__)

statementcallref operator,(statementcallref a, statementcallref b);
ref operator-(ref a, ref b);

ref a(ref what);
ref an(ref what);
ref a(ref what, ref name);
ref an(ref what, ref name);
