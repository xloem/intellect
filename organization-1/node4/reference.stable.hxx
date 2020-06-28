
#define ___STATIC_
#define ___EXTERN_ extern
//# 1 "reference.hpp"
#pragma once

#include <memory>
#include <functional>
#include "multi-any.stable.hxx"

using index_t = long long;

class reference;

// structure idea? we'd like _both_ one_directional and two_directional references,
//                 one_directional would use wayness to get two_directional,
//                 and need to be known about.
//                 could just do two_directional i suppose.
//          -> solved by inverse link wayness.
//          	could you implement that with the other stuff?
//          	busy atm

/*
 * reference: a general class that can hold other data
 *
 * UPDATE BELOW as methods progress
 * METHOD: preprocess.bash processes METHOD lines
 * 	they are turned into a member function that accesses the method_methodname attribute and calls it.
 *	method_methodname is made into a static function that returns a reference for the method
 *	it has the provided function as data, which is used as a default implementation
 *	however, the result of kind_get can be changed (by changing the getter or changing the attribute) to alter the method behavior later
 *
 * METHODS: atm we are deliberating betwen using the method kind as the default,
 *          vs using a separate basic_methodname reference
 *          it seems nice to be able to refer to the default, and the way to
 *          identify the method generally, separately.
 */

	// -> METHOD can statically call a class method to register itself if needed
	// -> of_kind will alter kind_get to return registered methods
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
	bool is_nonweak() const;
	void set_nonweak(bool nonweak);
	
	// general: consider also a c_style approach where no exceptions
	// are thrown.  could simplify meaning_order.

	// data access
	template <typename data_type>
		data_type & data_default(data_type default_data = {});
	template <typename data_type>
		data_type & data();

	// function call if data has a matching std::function
	template <typename... parameter_types>
		reference operator()(parameter_types... parameters);

___STATIC_ reference & reference_class() { static reference storage; return storage; }

	// METHOD call, uses kind_get to get kind, could default to basic functions
	reference operator()(reference kind, std::initializer_list<reference> parameters);

	// METHOD calls recognise_method for every defined method.  derived classes may change it.  they will need to make sure kind_get(method_kind) returns something appropriate.  things passed to this default function are added as kinds to registered_methods.
___EXTERN_ reference & recognised_methods();
	static void recognise_method(reference method_kind, reference basic_implementation, char const * classname, char const * methodname);

	// methods are indirect.  for direct access, use basic-*()(self, ...)
	// each `METHOD return_type name` makes 3 functions:
	// 	static reference method_name(); // returns the kind used to change the method
	// 	static reference basic_name(); // the default implementation
	// 	return_type name(...); // a member function that looks up the method and calls it

	// get a kinded property; returns null if nonpresent
	// this method controls all other methods by providing their default values, and can be changed
___EXTERN_ reference & basic_kind_get();
___EXTERN_ reference & method_kind_get();
	reference kind_get(reference kind);
//# 84 "reference.hpp"

	// set a kinded property, returns old value
___EXTERN_ reference & basic_kind_set();
___EXTERN_ reference & method_kind_set();
	reference kind_set(reference kind, reference value);
//# 87 "reference.hpp"

	// get all kinded property kinds, ordered
___EXTERN_ reference & basic_get_all_kinds();
___EXTERN_ reference & method_get_all_kinds();
	reference get_all_kinds();
//# 90 "reference.hpp"

	// get the count of ordered properties
___EXTERN_ reference & basic_order_count();
___EXTERN_ reference & method_order_count();
	reference order_count();
//# 93 "reference.hpp"

	// get an ordered property by index.  index is expected to have index_t data
___EXTERN_ reference & basic_order_get();
___EXTERN_ reference & method_order_get();
	reference order_get(reference index);
//# 96 "reference.hpp"

	// set an ordered property by index_t number [setting at index=count extendss]
___EXTERN_ reference & basic_order_set();
___EXTERN_ reference & method_order_set();
	reference order_set(reference index, reference value);
//# 99 "reference.hpp"

	// Warning: IMPLEMENTING OPERATORS CAN CAUSE STACK OVERFLOW IF RECURSIVELY USED

	// defaults to reseating this reference
___EXTERN_ reference & basic_operator_equals();
___EXTERN_ reference & method_operator_equals();
	reference operator_equals(reference other);
//# 104 "reference.hpp"
	reference operator=(reference other) { return operator_equals(other); }

	// no default; method_operator_brackets property must be set to not throw
___EXTERN_ reference & basic_operator_brackets();
___EXTERN_ reference & method_operator_brackets();
	reference operator_brackets(reference index);
//# 108 "reference.hpp"
	reference operator[](reference index) { return operator_brackets(index); }

	// called on destruction: dangerous as inheritance is not managed yet
	//METHOD void destruct();
	//~reference();


	// useful basic objects
	static reference const& null(); // empty reference
	static reference& kindness_mistake(); // thrown when kind mismatches
	static reference& presence_mistake(); // thrown when a null reference is used

	// kinds that might be set to alter behavior
	// TODO: set these all on some basic object to reference for default behavior

	//static reference kind_method; // event handling by instrumenting operators()?

	// for property_references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique_data could help with quick_scripting_implementation, local_variable_names.  note: doesn't help static function annotation without a way to refer to
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

	multi_any & raw_data();
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

template <typename data_type>
data_type & reference::data_default(data_type default_data)
{
	return this->raw_data().get<data_type>(default_data);
}

template <typename data_type>
data_type & reference::data()
{
	data_type * pointer = this->raw_data().pointer<data_type>();
	if (pointer == nullptr) {
		throw kindness_mistake();
	}
	return *pointer;
}

// TODO: with orderedness, we can now use a reference as an arguments object
// this function could construct such an object and pass it on.
// might be nice to have a function that returns the number of arguments or a model.
template <typename... parameter_types>
reference reference::operator()(parameter_types... parameters)
{
	using returning_function = std::function<reference(parameter_types...)>;
	using void_function = std::function<void(parameter_types...)>;
	returning_function *returner;
	try {
		returner = &data<returning_function>();
	} catch (reference) {
		data<void_function>()(parameters...);
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
		size_t operator()(const reference & to_hash) const;
	};
#undef ___CLASSNAME__
#undef ___CLASSNAMESTR__
//# 198 "reference.hpp"
}
