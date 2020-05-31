#include <string>
#include <unordered_map>
#include <vector>

#include "reference.hpp"

using namespace std;


namespace std { template <> struct hash<reference> {
	size_t operator()(const reference & to_hash) const
	{
		return hash<shared_ptr<reference::part>>()(to_hash.pointer());
	}
}; }

class reference::part
{
private:
	friend class reference;

	unordered_map<reference, reference> keyed_parts;
	vector<reference> key_order;

	any data;

	part(any &&data)
	: data(move(data))
	{ }
};

reference::reference(any data)
: shared(new part(move(data)))
{ }
reference::reference(reference const & other)
: shared(other.pointer())
{ }

reference::reference(bool * token_for_making_null_reference) {}

reference reference::operator()(reference kind, std::initializer_list<reference> parameters)
{
	// use kind_get to get kind
	reference getter = basic_get(*this, kind_get);
	if (getter == null) {
		getter = basic_get;
	}

	// default to basic functions
	reference method = getter(*this, kind);
	if (method == null) {
		if (kind == kind_get) {
			method = basic_get;
		} else if (kind == kind_set) {
			method = basic_set;
		} else if (kind == kind_count) {
			method = basic_count;
		} else if (kind == kind_index) {
			method = basic_index;
		}
	}

	//
	std::vector<reference> values(parameters);
	switch (values.size()) {
	case 0:
		return method(*this);
	case 1:
		return method(*this, values[0]);
	case 2:
		return method(*this, values[0], values[1]);
	case 3:
		return method(*this, values[0], values[1], values[2]);
	case 4:
		return method(*this, values[0], values[1], values[2], values[3]);
	case 5:
		return method(*this, values[0], values[1], values[2], values[3], values[4]);
	case 6:
		return method(*this, values[0], values[1], values[2], values[3], values[4], values[5]);
	case 7:
		return method(*this, values[0], values[1], values[2], values[3], values[4], values[5], values[6]);
	case 8:
		return method(*this, values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7]);
	default:
		throw "more than 8 method arguments, need new case line here";
	}
}

reference reference::basic_get((std::function<reference(reference,reference)>)[](reference focus, reference key) -> reference
{
	auto & map = focus.pointer()->keyed_parts;
	auto result = map.find(key);
	if (result == map.end()) {
		return null;
	} else {
		return result->second;
	}
});

reference reference::basic_set((std::function<reference(reference,reference,reference)>)[](reference focus, reference key, reference value) -> reference
{
	auto & map = focus.pointer()->keyed_parts;
	auto result = map.emplace(key, value);
	if (result.second) {
		// insertion happened: no old element
		focus.pointer()->key_order.push_back(key);
		return null;
	} else {
		// key already present
		reference old_value = result.first->second;
		result.first->second = value;
		return old_value;
	}
});

reference reference::basic_count((std::function<reference(reference)>)[](reference focus) -> reference
{
	return (any)(index_t)focus.pointer()->key_order.size();
});

reference reference::basic_index((std::function<reference(reference, reference)>)[](reference focus, reference index) -> reference
{
	return focus.pointer()->key_order[(index_t)index];
});

/*
reference reference::get((std::function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	reference getter = basic_get(focus, kind_get);
	if (getter == null) {
		getter = basic_get;
	}
	// we'll also want to have kind_set default to basic_set?
	return getter(focus, kind);
});

reference reference::indirect_set((std::function<reference(reference,reference,reference)>)[](reference focus, reference key, reference value) -> reference
{
	reference setter = basic_get(focus, indirect_setter);
	if (setter == null) {
		setter = basic_set;
	}
	return setter(focus, indirect_setter, value);
	
	auto & map = focus.pointer()->keyed_parts;
	auto result = map.emplace(key, value);
	if (result.second) {
		// insertion happened: no old element
		return null;
	} else {
		// key already present
		reference old_value = result.first->second;
		result.first->second = value;
		return old_value;
	}
});
*/

reference reference::kindness_mistake(string("kindness_mistake"));
reference const reference::null((bool *)"token_for_making_null_reference");

reference reference::kind_get(string("kind_get"));
reference reference::kind_set(string("kind_set"));
reference reference::kind_count(string("kind_count"));
reference reference::kind_index(string("kind_index"));
reference reference::kind_operator_equals(string("kind_operator_equals"));
reference reference::kind_operator_brackets(string("kind_operator_brackets"));


/*
reference reference::operator[](reference key)
{
	return get(*this, key);
	// the getter returns a reference, not an entry
	// assignment won't work this way without change
	return this->get(::get)(*this, key);
}
*/

bool reference::is_nonweak() const
{
	return (bool)shared;
}

void reference::set_nonweak(bool nonweak)
{
	if (nonweak) {
		shared == pointer();
		weak.reset();
	} else {
		weak = pointer();
		shared.reset();
	}
}

bool reference::operator==(reference const & other) const
{
	return other.pointer() == pointer();
}

shared_ptr<reference::part> reference::pointer() const
{
	return is_nonweak() ? shared : weak.lock();
}

any & reference::data()
{
	return pointer()->data;
}
