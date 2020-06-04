
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

	reference exists(); // bool
	reference get(reference key);
	reference change(reference key, reference value);
	// it seems like it is nice to have a reference to a property.
	// this roughly a reference to a connection.
		// we could add n-ary connections.
		// we could make properties that connect to connection
		// objects, and maintain the similarity.  that
		// would be a structure agreement.
		// structure agreements probably more valuable.
		// but maps most important.
		// maps quickly done with keys to connection objects.
		// so connection objects have operator=
		// 	[idea of passing off operators to destination property]
		// and normal objects have operator[]

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

#define true true_
#define false false_

reference kindness_mistake;
reference true, false;

#include "dribble-graph-3-part.hpp"

// MAKE SURE TO MAKE SET OPS EASYABLE

reference get([](reference self, reference attribute) {
		// incomplete type std::is_copy_constructible<reference> used in nested-name spec...
	return self.get(attribute);
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
	return this->get(::get)(*this, key);
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

reference reference::exists()
{
	return ((bool)(shared_ptr<part>)(*this)) ? 
}

reference reference::get(reference key)
{
	auto result = keyed_parts.find(key);
	return result == keyed_parts.end() ? {} : *result;
}

void reference::set(reference key, reference value)
{
	(*this)->keyed_parts[key] = value;
}


/*
 * complex bootstrap-part.
 * let's test basic code first.
 */
