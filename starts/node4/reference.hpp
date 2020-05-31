#pragma once

#include <experimental/any>
	namespace std { using experimental::any; using experimental::any_cast; }
#include <memory>
#include <functional>

class part;
class reference;

#define true true_
#define false false_

extern reference kindness_mistake; // thrown when kind mismatches
extern reference true, false;

// a general reference to an index of pointers
class reference
{
public:
	reference(std::any data);
	reference();
	reference(reference const & other);
	template <typename data_type>
		operator data_type &();
	// throws kindness_mistak if this isn't a std::function taking and returning references
	template <typename... parameter_types>
		reference operator()(parameter_types... parameters);

	static reference const null;

	// callable: get an immediate property
	static reference get/*(reference focus, reference key)*/;

	// callable: set an immediate property, returns old value
	static reference set/*(reference focus, reference key, reference value)*/;

	// we might like to copy one property to another
	// but we see it relates to maps, which are more general
		// this fucntion is set(focus2, key2, get(focus1, key1));
		// so you see we could make a fucntional programming language
		// by including call objects inside of call objects.
	static reference copy/*(reference focus1, reference key1, reference focus2, reference key2)*/;

	// for calling members, structural subclasses could override?
		// we could separate connectedness from immediateness
		// using different operators, too?
	//reference operator[](reference key);

	// nonweak means reference-counting.  all references start nonweak.
	// see how these functions are used to see if that should be changed.
	bool is_nonweak() const;
	void set_nonweak(bool nonweak);

	// for property-references, must treat properties as connection objects owned by this object (connectedness) [when implementing connectedness, if typedness is being implemented, we'll want gettersetterness]
		// ^-- nonweakness relation: users of connection objects that are nonweak may be surprised if the source object for the connection is thrown out.  some way to propagate events could be relevent.
	
	// unaddressed: graph models.  probably doing with placeholders and maps.
	
	// unique-data could help with quick-scripting-implementation, local-variable-names.  note: doesn't help static function annotation without a way to refer to
	// but might expand subclassing (note bool and functions can still be subclasses, reference casting)

	bool operator==(reference const & other) const;

private:
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
