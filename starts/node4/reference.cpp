#include <string>
#include <unordered_map>
#include <vector>

#include "reference.hpp"

using namespace std;

reference kindness_mistake(string("kindness_mistake"));
reference true(string("true")), false(string("false"));

namespace std { template <> struct hash<reference> {
	size_t operator()(const reference & to_hash) const
	{
		return hash<shared_ptr<part>>()(to_hash.pointer());
	}
}; }

// this is what's inside a reference
class part
{
private:
	friend class reference;

	unordered_map<reference, reference> keyed_parts;
	vector<reference> key_order;

	any data;

	part(any data)
	: data(move(data))
	{ }
};

reference::reference(any data)
: shared(new part(data))
{ }
reference::reference()
: shared(new part({}))
{ }
reference::reference(reference const & other)
: shared(other.pointer())
{ }

reference const reference::null((bool *)"token_for_making_null_reference");
reference::reference(bool * token_for_making_null_reference) {}

reference reference::get((std::function<reference(reference,reference)>)[](reference focus, reference key) -> reference
{
	auto & map = focus.pointer()->keyed_parts;
	auto result = map.find(key);
	if (result == map.end()) {
		return null;
	} else {
		return result->second;
	}
});

reference reference::set((std::function<reference(reference,reference,reference)>)[](reference focus, reference key, reference value) -> reference
{
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

shared_ptr<part> reference::pointer() const
{
	return is_nonweak() ? shared : weak.lock();
}

any & reference::data()
{
	return pointer()->data;
}
