
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

class reference
{
public:

	reference get(reference key);
	void change(reference key, reference value);
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
	reference(reference const & other);

	// bool whether content is in this reference
	// TODO: a new reference is the same as an empty reference atm
	// reference exists();

	void set_strong(reference strong_bool);
	reference get_strong();


	reference operator[](reference key);

	template <typename... parameter_types>
	reference operator()(parameter_types... parameters);

private:
	shared_ptr<part> pointer();

	shared_ptr<part> shared;
	weak_ptr<part> weak;
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
: shared(new part(data)) // can't make a weak new allocation
{ }
reference::reference(reference const & other)
: shared(other.pointer())
{ }

shared_ptr<part> reference::pointer()
{
	// we return a shared_ptr to make sure the same shared_ptr
	// information is used for every reference to the data
	return shared ? shared : weak.lock();
}

void reference::set_strong(reference bool_strong)
{
	if (bool_strong == true) {
		shared = pointer();
	} else if (bool_strong == false) {
		weak = pointer();
		shared.reset();
	} else {
		throw kindness_mistake;
	}
}

reference source;
reference kind;
reference destination;



// MAP-UTILITY-FIRST

//	generalization of connections involves n-arity and n-oppositeness
//	be great if connection-access could be generalizable


// below bits access part-content

/*
 * TODO: we don't provide a way to make empty references yet
reference reference::exists()
{
	return ((bool)(shared_ptr<part>)(*this));
}
*/

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
