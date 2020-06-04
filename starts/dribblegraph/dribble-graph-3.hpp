
#include <memory>
#include <functional>
#include <experimental/any>

// these headers can go in .cpp file
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
using std::experimental::any;
using std::experimental::any_cast;
using namespace std;

class part;
class reference;

class reference : public shared_ptr<part>
{
public:
	using shared_ptr<part>::shared_ptr;

	reference read_only_access(reference key);

	template <typename data_type> operator data_type &();


	template <typename Function>
	reference(function<Function> callable)
	: reference(any(callable))
	{ }
	reference(any data = {});


	reference operator[](reference key);

	template <typename... parameter_types>
	reference operator()(parameter_types... parameters);
};

reference kindness_mistake;

#include "dribble-graph-3-part.hpp"

// MAKE SURE TO MAKE SET OPS EASYABLE

reference get([](reference self, reference attribute) {
		// incomplete type std::is_copy_constructible<reference> used in nested-name spec...
	return self.read_only_access(attribute);
});


// -> want annotation, needs annotation references
// -> want setters .. is setting done?

// it will be confusing for a generalized system to have
// two kinds of references: c++ and part.
// is better to have a part-reference that references the
// link itself.
// it would have an assignment operator that changes its destination.

reference reference::operator[](reference key)
{
	// the getter returns a reference, not an entry
	// assignment won't work this way without change
	return read_only_access(::get)(*this, key);
}

reference::reference(any data)
: shared_ptr<part>(new part(data))
{ }

reference source;
reference kind;
reference destination;



// MAP-UTILITY-FIRST

//	generalization of connections involves n-arity and n-oppositeness
//	be great if connection-access could be generalizable

// below bits access part-content

reference reference::read_only_access(reference key)
{
	return (*this)->keyed_parts[key];
}


/*
 * complex bootstrap-part.
 * let's test basic code first.
 */
