#pragma once

#include <experimental/any>
	namespace std { using experimental::any; using experimental::any_cast; }
#include <memory>
#include <functional>

using index_t = long long;

class reference
{
public:
	// construction
	reference(std::any data = {});
	reference(reference const & other);

	// in case a weak reference is needed
	bool is_nonweak() const;
	void set_nonweak(bool nonweak);

	// data access
	template <typename data_type>
		operator data_type &();

	// function call if data is std::function
	template <typename... parameter_types>
		reference operator()(parameter_types... parameters);

	// method call, uses kind_get to get kind, could default to basic functions
	reference operator()(reference kind, std::initializer_list<reference> parameters);

	// get an immediate property
	static reference basic_get/*(reference focus, reference kind)*/;

	// set an immediate property, returns old value
	static reference basic_set/*(reference focus, reference kind, reference value)*/;

	// get the count of immediate properties
	static reference basic_count/*(reference focus)*/;

	// get an immediate property by ordered number
	static reference basic_index/*(reference focus, reference index)*/;

	// get an indirect property
	reference get(reference kind) { return (*this)(kind_get, {kind}); }
	
	// set an indirect property, returns old value
	reference set(reference kind, reference value) { return (*this)(kind_set, {kind, value}); }
	
	// count properties indirectly
	reference count() { return (*this)(kind_count, {}); }

	// get a property by index indirectly
	reference index(reference index) { return (*this)(kind_index, {index}); }

	// defaults to reseating this reference
	reference operator=(reference other) { return (*this)(kind_operator_equals, {other}); }

	// no default; kind_operator_brackets must be set to not throw
	reference operator[](reference index) { return (*this)(kind_operator_brackets, {index}); }


	// useful basic objects
	static reference const null; // empty reference
	static reference kindness_mistake; // thrown when kind mismatches
	static reference presence_mistake; // thrown when a null reference is used

	// kinds that might be set to alter behavior
	// TODO: set these all on some basic object to reference for default behavior
	static reference kind_get; // this method controls all other methods
	static reference kind_set;
	static reference kind_count;
	static reference kind_index;
	// IMPLEMENTING OPERATORS CAN CAUSE STACK OVERFLOW IF RECURSIVELY USED
	static reference kind_operator_equals;
	static reference kind_operator_brackets;

	// for property-references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique-data could help with quick-scripting-implementation, local-variable-names.  note: doesn't help static function annotation without a way to refer to
	// but might expand subclassing (note bool and functions can still be subclasses, reference casting)

	bool operator==(reference const & other) const;

private:
	class part;
	friend class std::hash<reference>;
	char const * what() const noexcept; // unused, can move: returns possible excpetion text
	std::shared_ptr<part> pointer() const;
	reference(bool * token_for_making_null_reference);

	std::shared_ptr<part> shared;
	std::weak_ptr<part> weak;

	std::any & data();
};

// template implementations below

template <typename data_type>
reference::operator data_type &()
{
	auto & data = this->data();

	if (data.type() == typeid(void)) {
		data = data_type();
	} else if (data.type() != typeid(data_type)) {
		// kind of data was assumed wrongly
		// not-kind to treat as something other than self
		throw kindness_mistake;
	}
	return *std::any_cast<data_type>(&data);
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
		returner = &static_cast<returning_function&>(*this);
	} catch (reference) {
		static_cast<void_function&>(*this)(parameters...);
		return null;
	}
	return (*returner)(parameters...);
}
