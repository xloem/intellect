#pragma once

#include <memory>
#include <functional>
#include "multi-any.hpp"

using index_t = long long;

class reference;

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

	// data access
	
	// general: consider also a c-style approach where no exceptions
	// are thrown.  could simplify meaning-order.

	template <typename data-type>
		data-type & data-default(data-type default-data = {});
	template <typename data-type>
		data-type & data();


	// function call if data has the std::function
	template <typename... parameter-types>
		reference operator()(parameter-types... parameters);

	// method call, uses kind-get to get kind, could default to basic functions
	reference operator()(reference kind, std::initializer_list<reference> parameters);

	// get an immediate property; returns null if nonpresent
	static reference basic-kind-get/*(reference focus, reference kind)*/();

	// set an immediate property, returns old value
	static reference basic-kind-set/*(reference focus, reference kind, reference value)*/();

	// get all immediate property kinds, ordered
	static reference basic-get-all-kinds/*(reference focus)*/();

	// get the count of immediate properties
	static reference basic-order-count/*(reference focus)*/();

	// get an immediate property by ordered number
	static reference basic-order-get/*(reference focus, reference index)*/();

	// set an immediate property by ordered number, extending if needed
	static reference basic-order-set/*(reference focus, reference index)*/();

	// get an indirect property
	reference kind-get(reference kind) { return (*this)(method-kind-get(), {kind}); }
	
	// set an indirect property, returns old value
	reference kind-set(reference kind, reference value) { return (*this)(method-kind-set(), {kind, value}); }

	// get all property kinds, ordered, indirectly
	reference get-all-kinds() { return (*this)(method-get-all-kinds(), {}); }
	
	// count properties indirectly
	reference order-count() { return (*this)(method-order-count(), {}); }

	// get a property by index indirectly
	reference order-get(reference index) { return (*this)(method-order-get(), {index}); }

	// set a property by index indirectly
	reference order-set(reference index, reference value) { return (*this)(method-order-set(), {index, value}); }

	// defaults to reseating this reference
	reference operator=(reference other) { return (*this)(method-operator-equals(), {other}); }

	// no default; kind-operator-brackets must be set to not throw
	reference operator[](reference index) { return (*this)(method-operator-brackets(), {index}); }


	// useful basic objects
	static reference const& null(); // empty reference
	static reference& kindness-mistake(); // thrown when kind mismatches
	static reference& presence-mistake(); // thrown when a null reference is used

	// kinds that might be set to alter behavior
	// TODO: set these all on some basic object to reference for default behavior
	static reference& method-kind-get(); // this method controls all other methods by providing their values
	static reference& method-kind-set();
	static reference& method-get-all-kinds();
	static reference& method-order-count();
	static reference& method-order-get();
	static reference& method-order-set();
	// IMPLEMENTING OPERATORS CAN CAUSE STACK OVERFLOW IF RECURSIVELY USED
	static reference& method-operator-equals();
	static reference& method-operator-brackets();

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

#define DEFINE_INLINE_RETURNING_METHOD(name, parameter-types-and-names, body) \
static reference name(){static reference name((function<reference>(

#define DEFINE(type, scope, name) type & scope name() { static type name(string(#name)); return name; }

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
