#pragma once

#include <experimental/any>
	namespace std { using experimental::any; using experimental::any_cast; }
#include <memory>
#include <functional>

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

	// get an immediate property
	static reference basic_get/*(reference focus, reference key)*/;

	// set an immediate property, returns old value
	static reference basic_set/*(reference focus, reference key, reference value)*/;

	// get an indirect property
	static reference indirect_get/*(reference focus, reference key)*/;

	// set an indirect property, returns old value
	static reference indirect_set/*(reference focus, reference key, reference value)*/;

	// useful basic objects
	static reference kindness_mistake; // thrown when kind mismatches
	static reference const null; // empty reference

	// kinds that can be used to store alternatives to the basic functions
	static reference indirect_getter;
	static reference indirect_setter;

	// kinds that could be used to store operators in c++
	static reference operator_equals;
	static reference operator_brackets;

	// for property-references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique-data could help with quick-scripting-implementation, local-variable-names.  note: doesn't help static function annotation without a way to refer to
	// but might expand subclassing (note bool and functions can still be subclasses, reference casting)

	bool operator==(reference const & other) const;

private:
	class part;
	friend class std::hash<reference>;
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
