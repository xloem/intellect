#pragma once

#include <memory>
#include <functional>
#include "multi-any.hpp"

using index_t = long long;

class reference;

// structure idea? we'd like _both_ one-directional and two-directional references,
//                 one-directional would use wayness to get two-directional,
//                 and need to be known about.
//                 could just do two-directional i suppose.
//          -> solved by inverse link wayness.
//          	could you implement that with the other stuff?
//          	busy atm

/*
 * reference: a general class that can hold other data
 *
 * UPDATE BELOW as methods progress
 * METHOD: preprocess.bash processes METHOD lines
 * 	they are turned into a member function that accesses the method-methodname attribute and calls it.
 *	method-methodname is made into a static function that returns a reference for the method
 *	it has the provided function as data, which is used as a default implementation
 *	however, the result of kind-get can be changed (by changing the getter or changing the attribute) to alter the method behavior later
 *
 * METHODS: atm we are deliberating betwen using the method kind as the default,
 *          vs using a separate basic-methodname reference
 *          it seems nice to be able to refer to the default, and the way to
 *          identify the method generally, separately.
 */

	// -> METHOD can statically call a class method to register itself if needed
	// -> of-kind will alter kind-get to return registered methods
	// -> make a method kind static object, prefixed with method-
	// -> make the method static object be prefixed with basic-
	// -> make a member function that acts as the method

class reference
{
public:
	// construction
	reference(std::any data = {});
	reference(std::initializer_list<std::any> datas);
	reference(reference const & other);

	// in case a weak reference is needed
	bool is-nonweak() const;
	void set-nonweak(bool nonweak);
	
	// general: consider also a c-style approach where no exceptions
	// are thrown.  could simplify meaning-order.

	// data access
	template <typename data-type>
		data-type & data-default(data-type default-data = {});
	template <typename data-type>
		data-type & data();

	// function call if data has a matching std::function
	template <typename... parameter-types>
		reference operator()(parameter-types... parameters);

	DEFINE reference reference-class;

	// METHOD call, uses kind-get to get kind, could default to basic functions
	reference operator()(reference kind, std::initializer_list<reference> parameters);

	// METHOD calls recognise-method for every defined method.  derived classes may change it.  they will need to make sure kind-get(method-kind) returns something appropriate.  things passed to this default function are added as kinds to registered-methods.
	DECLARE reference recognised-methods;
	static void recognise-method(reference method-kind, reference basic-implementation, char const * classname, char const * methodname);

	// methods are indirect.  for direct access, use basic-*()(self, ...)
	// each `METHOD return_type name` makes 3 functions:
	// 	static reference method-name(); // returns the kind used to change the method
	// 	static reference basic-name(); // the default implementation
	// 	return_type name(...); // a member function that looks up the method and calls it

	// get a kinded property; returns null if nonpresent
	// this method controls all other methods by providing their default values, and can be changed
	METHOD reference kind-get(reference kind);

	// set a kinded property, returns old value
	METHOD reference kind-set(reference kind, reference value);

	// get all kinded property kinds, ordered
	METHOD reference get-all-kinds();

	// get the count of ordered properties
	METHOD reference order-count();

	// get an ordered property by index.  index is expected to have index_t data
	METHOD reference order-get(reference index);

	// set an ordered property by index_t number [setting at index=count extendss]
	METHOD reference order-set(reference index, reference value);

	// Warning: IMPLEMENTING OPERATORS CAN CAUSE STACK OVERFLOW IF RECURSIVELY USED

	// defaults to reseating this reference
	METHOD reference operator-equals(reference other);
	reference operator=(reference other) { return operator-equals(other); }

	// no default; method-operator-brackets property must be set to not throw
	METHOD reference operator-brackets(reference index);
	reference operator[](reference index) { return operator-brackets(index); }

	// called on destruction: dangerous as inheritance is not managed yet
	//METHOD void destruct();
	//~reference();


	// useful basic objects
	static reference const& null(); // empty reference
	static reference& kindness-mistake(); // thrown when kind mismatches
	static reference& presence-mistake(); // thrown when a null reference is used

	// kinds that might be set to alter behavior
	// TODO: set these all on some basic object to reference for default behavior

	//static reference kind-method; // event handling by instrumenting operators()?

	// for property-references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique-data could help with quick-scripting-implementation, local-variable-names.  note: doesn't help static function annotation without a way to refer to
	// but might expand subclassing (note bool and functions can still be subclasses, reference casting)

	bool operator==(reference const & other) const;
	bool operator!=(reference const & other) const;

private:
	class part;
	friend class std::hash<reference>;
	char const * what() const noexcept; // unused, can move: returns possible excpetion text
	std::shared_ptr<part> pointer() const;
	reference(bool ***** token_for_making_null_reference);

	std::shared_ptr<part> shared;
	std::weak_ptr<part> weak;

	multi-any & raw-data();
};


// parameters take types and names
// if we could iterate, we could make types convert to references
// ^-- above is left over from plans for macro implementation of methods
//     we could still convert types to references when methodss are parsed.

// template implementations below

template <typename data-type>
data-type & reference::data-default(data-type default-data)
{
	return this->raw-data().get<data-type>(default-data);
}

template <typename data-type>
data-type & reference::data()
{
	data-type * pointer = this->raw-data().pointer<data-type>();
	if (pointer == nullptr) {
		throw kindness-mistake();
	}
	return *pointer;
}

// TODO: with orderedness, we can now use a reference as an arguments object
// this function could construct such an object and pass it on.
// might be nice to have a function that returns the number of arguments or a model.
template <typename... parameter-types>
reference reference::operator()(parameter-types... parameters)
{
	using returning-function = std::function<reference(parameter-types...)>;
	using void-function = std::function<void(parameter-types...)>;
	returning-function *returner;
	try {
		returner = &data<returning-function>();
	} catch (reference) {
		data<void-function>()(parameters...);
		return null();
	}
	return (*returner)(parameters...);
}

// hash implementation for containers
namespace std {
	template <>
	struct hash<reference>
	{
		size_t operator()(const reference & to-hash) const;
	};
}
