
#define ___STATIC_
#define ___EXTERN_ extern
//# 1 "reference.hpp"
#pragma once

#include <memory>
#include <functional>
#include "multi-any.stable.hxx"

using index_t = long long;

class reference;

// structure idea? we'd like _both_ one__directional and two__directional references,
//                 one__directional would use wayness to get two__directional,
//                 and need to be known about.
//                 could just do two__directional i suppose.
//          -> solved by inverse link wayness.
//          	could you implement that with the other stuff?
//          	busy atm

/*
 * reference: a general class that can hold other data
 *
 * UPDATE BELOW as methods progress
 * METHOD: preprocess.bash processes METHOD lines
 * 	they are turned into a member function that accesses the method__methodname attribute and calls it.
 *	method__methodname is made into a static function that returns a reference for the method
 *	it has the provided function as data, which is used as a default implementation
 *	however, the result of kind__get can be changed (by changing the getter or changing the attribute) to alter the method behavior later
 *
 * METHODS: atm we are deliberating betwen using the method kind as the default,
 *          vs using a separate basic__methodname reference
 *          it seems nice to be able to refer to the default, and the way to
 *          identify the method generally, separately.
 */

	// -> METHOD can statically call a class method to register itself if needed
	// -> of__kind will alter kind__get to return registered methods
	// -> make a method kind static object, prefixed with method-
	// -> make the method static object be prefixed with basic-
	// -> make a member function that acts as the method

#undef ___STATIC_
#undef ___EXTERN_
#define ___CLASSNAME_ reference
#define ___CLASSNAMESTR_ "reference"
#define ___STATIC_ static
#define ___EXTERN_ static
class reference
//# 42 "reference.hpp"
{
public:
	// construction
	reference(std::any data = {});
	reference(std::initializer_list<std::any> datas);
	reference(reference const & other);

	// in case a weak reference is needed
	bool is__nonweak() const;
	void set__nonweak(bool nonweak);
	
	// general: consider also a c__style approach where no exceptions
	// are thrown.  could simplify meaning__order.

	// data access
	template <typename data__type>
		data__type & data__default(data__type default__data = {});
	template <typename data__type>
		data__type & data();

	// function call if data has a matching std::function
	template <typename... parameter__types>
		reference operator()(parameter__types... parameters);

___STATIC_ reference & reference__class() { static reference storage; return storage; }

	// METHOD call, uses kind__get to get kind, could default to basic functions
	reference operator()(reference kind, std::initializer_list<reference> parameters);

	// METHOD calls recognise__method for every defined method.  derived classes may change it.  they will need to make sure kind__get(method__kind) returns something appropriate.  things passed to this default function are added as kinds to registered__methods.
___EXTERN_ reference & recognised__methods();
	static void recognise__method(reference method__kind, reference basic__implementation, char const * classname, char const * methodname);

	// methods are indirect.  for direct access, use basic-*()(self, ...)
	// each `METHOD return_type name` makes 3 functions:
	// 	static reference method__name(); // returns the kind used to change the method
	// 	static reference basic__name(); // the default implementation
	// 	return_type name(...); // a member function that looks up the method and calls it

	// get a kinded property; returns null if nonpresent
	// this method controls all other methods by providing their default values, and can be changed
___EXTERN_ reference & basic__kind__get();
___EXTERN_ reference & method__kind__get();
	reference kind__get(reference kind);
//# 84 "reference.hpp"

	// set a kinded property, returns old value
___EXTERN_ reference & basic__kind__set();
___EXTERN_ reference & method__kind__set();
	reference kind__set(reference kind, reference value);
//# 87 "reference.hpp"

	// get all kinded property kinds, ordered
___EXTERN_ reference & basic__get__all__kinds();
___EXTERN_ reference & method__get__all__kinds();
	reference get__all__kinds();
//# 90 "reference.hpp"

	// get the count of ordered properties
___EXTERN_ reference & basic__order__count();
___EXTERN_ reference & method__order__count();
	reference order__count();
//# 93 "reference.hpp"

	// get an ordered property by index.  index is expected to have index_t data
___EXTERN_ reference & basic__order__get();
___EXTERN_ reference & method__order__get();
	reference order__get(reference index);
//# 96 "reference.hpp"

	// set an ordered property by index_t number [setting at index=count extendss]
___EXTERN_ reference & basic__order__set();
___EXTERN_ reference & method__order__set();
	reference order__set(reference index, reference value);
//# 99 "reference.hpp"

	// Warning: IMPLEMENTING OPERATORS CAN CAUSE STACK OVERFLOW IF RECURSIVELY USED

	// defaults to reseating this reference
___EXTERN_ reference & basic__operator__equals();
___EXTERN_ reference & method__operator__equals();
	reference operator__equals(reference other);
//# 104 "reference.hpp"
	reference operator=(reference other) { return operator__equals(other); }

	// no default; method__operator__brackets property must be set to not throw
___EXTERN_ reference & basic__operator__brackets();
___EXTERN_ reference & method__operator__brackets();
	reference operator__brackets(reference index);
//# 108 "reference.hpp"
	reference operator[](reference index) { return operator__brackets(index); }

	// called on destruction: dangerous as inheritance is not managed yet
	//METHOD void destruct();
	//~reference();


	// useful basic objects
	static reference const& null(); // empty reference
	static reference& kindness__mistake(); // thrown when kind mismatches
	static reference& presence__mistake(); // thrown when a null reference is used

	// kinds that might be set to alter behavior
	// TODO: set these all on some basic object to reference for default behavior

	//static reference kind__method; // event handling by instrumenting operators()?

	// for property__references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique__data could help with quick__scripting__implementation, local__variable__names.  note: doesn't help static function annotation without a way to refer to
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

	multi__any & raw__data();
};
#undef ___CLASSNAME_
#undef ___CLASSNAMESTR_
#undef ___STATIC_
#define ___STATIC_
#undef ___EXTERN_
#define ___EXTERN_ extern
//# 148 "reference.hpp"


// parameters take types and names
// if we could iterate, we could make types convert to references
// ^-- above is left over from plans for macro implementation of methods
//     we could still convert types to references when methodss are parsed.

// template implementations below

template <typename data__type>
data__type & reference::data__default(data__type default__data)
{
	return this->raw__data().get<data__type>(default__data);
}

template <typename data__type>
data__type & reference::data()
{
	data__type * pointer = this->raw__data().pointer<data__type>();
	if (pointer == nullptr) {
		throw kindness__mistake();
	}
	return *pointer;
}

// TODO: with orderedness, we can now use a reference as an arguments object
// this function could construct such an object and pass it on.
// might be nice to have a function that returns the number of arguments or a model.
template <typename... parameter__types>
reference reference::operator()(parameter__types... parameters)
{
	using returning__function = std::function<reference(parameter__types...)>;
	using void__function = std::function<void(parameter__types...)>;
	returning__function *returner;
	try {
		returner = &data<returning__function>();
	} catch (reference) {
		data<void__function>()(parameters...);
		return null();
	}
	return (*returner)(parameters...);
}

// hash implementation for containers
namespace std {
	template <>
#define ___CLASSNAME__ hash<reference>
#define ___CLASSNAMESTR__ "hash<reference>"
	struct hash<reference>
//# 195 "reference.hpp"
	{
		size_t operator()(const reference & to__hash) const;
	};
#undef ___CLASSNAME__
#undef ___CLASSNAMESTR__
//# 198 "reference.hpp"
}
