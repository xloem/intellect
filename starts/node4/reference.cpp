#include <string>
#include <unordered_map>
#include <vector>

#include "reference.hpp"

using namespace std;

size_t std::hash<reference>::operator()(const reference & to-hash) const
{
	return hash<shared_ptr<reference::part>>()(to-hash.pointer());
}

class reference::part
{
private:
	friend class reference;

	using kinded = unordered_map<reference, reference>;
	using ordered = vector<reference>;

	multi-any data;
};

reference::reference(any data)
: shared(new part())
{
	raw-data().set(move(data));
}
reference::reference(initializer_list<any> datas)
: shared(new part())
{
	for (auto & data : datas) {
		auto old = raw-data().set(move(data));
		if (old.type() != typeid(void)) {
			// two of the same type passed
			throw kindness-mistake();
		}
	}
}
reference::reference(reference const & other)
: shared(other.pointer())
{ }

reference::reference(bool ***** token_for_making_null_reference) {}

reference reference::operator()(reference kind, initializer_list<reference> parameters)
{
	// this is called by operators, so when it uses operators there
	// is possibility for stack overflow.
	if (kind == null()) { throw presence-mistake(); }

	// use method-kind-get to get kind
	reference getter(null());
	try {
		// direct .shared access used to avoid operator= method usage in the method-call handler
		getter.shared = basic-kind-get()(*this, method-kind-get()).pointer();
	} catch (reference) { }
	if (getter == null()) {
		getter.shared = basic-kind-get().pointer();
	}

	// default to basic functions
	reference method(null());
	try {
		method.shared = getter(*this, kind).pointer();
	} catch (reference) { }
	if (method == null()) {
		// here default methods are built in for things the getter doesn't handle
		if (kind == method-kind-get()) {
			method = basic-kind-get();
		} else if (kind == method-kind-set()) {
			method = basic-kind-set();
		} else if (kind == method-get-all-kinds()) {
			method = basic-get-all-kinds();
		} else if (kind == method-order-count()) {
			method = basic-order-count();
		} else if (kind == method-order-get()) {
			method = basic-order-get();
		} else if (kind == method-order-set()) {
			method = basic-order-set();
		} else if (kind == method-operator-equals()) {
			// default implementation can't be done with a method call because references are passed to methods by copy for now
			if (parameters.size() != 1) { throw kindness-mistake(); }
			shared = parameters.begin()->pointer();
			return *this;
		}
	}

	//
	vector<reference> values(parameters);
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

reference reference::basic-kind-get(){static reference basic-kind-get((function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	auto & map = focus.pointer()->data.get<part::kinded>();
	auto result = map.find(kind);
	if (result == map.end()) {
		return null();
	} else {
		return result->second;
	}
}); return basic-kind-get;}

reference reference::basic-kind-set(){static reference basic-kind-set((function<reference(reference,reference,reference)>)[](reference focus, reference kind, reference value) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	auto & map = focus.pointer()->data.get<part::kinded>();
	auto result = map.emplace(kind, value);
	if (result.second) {
		// insertion happened: no old element
		return null();
	} else {
		// kind already present
		reference old-value = result.first->second;
		result.first->second = value;
		return old-value;
	}
}); return basic-kind-set;}

reference reference::basic-get-all-kinds(){static reference basic-get-all-kinds((function<reference(reference)>)[](reference focus) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	auto & map = focus.pointer()->data.get<part::kinded>();
	reference result;
	for (auto & item : map) {
		result.order-set(result.order-count(), item.first);
	}
	return result;
}); return basic-get-all-kinds;}

reference reference::basic-order-count(){static reference basic-order-count((function<reference(reference)>)[](reference focus) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	return (any)(index_t)focus.pointer()->data.get<part::ordered>().size();
}); return basic-order-count;}

reference reference::basic-order-get(){static reference basic-order-get((function<reference(reference, reference)>)[](reference focus, reference index) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	index_t index-data = index.data<index_t>();
	auto & vector = focus.pointer()->data.get<part::ordered>();
	if (index-data < 0 || index-data >= (index_t)vector.size()) {
		throw presence-mistake();
	}
	return vector[index-data];
}); return basic-order-get;}

// this appears the same as the in-class method except it doesn't have static
//  in front of it, and does have a classspace scope specifier.
reference reference::basic-order-set(){static reference basic-order-set((function<reference(reference, reference, reference)>)[](reference focus, reference index, reference value) -> reference
{
	if (!focus.pointer()) { throw presence-mistake(); }
	auto & ordered-parts = focus.pointer()->data.get<part::ordered>();
	index_t index-data = index.data<index_t>();
	if (value != null()) {
		if (index-data < 0 || index-data > (index_t)ordered-parts.size()) {
			throw presence-mistake();
		}
		if (index-data == (index_t)ordered-parts.size()) {
			ordered-parts.emplace_back(value);
			return null();
		} else {
			reference old-item = ordered-parts[index-data];
			ordered-parts[index-data] = value;
			return old-item;
		}
	} else {
		if (index-data < 0 || index-data >= (index_t)ordered-parts.size()) {
			throw presence-mistake();
		}
		reference old-item = ordered-parts[index-data];
		ordered-parts.erase(ordered-parts.begin() + index-data);
		return old-item;
	}
}); return basic-order-set;}

/*
reference reference::get((function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	reference getter = basic-get(focus, kind-get);
	if (getter == null()) {
		getter = basic-get;
	}
	// we'll also want to have kind-set default to basic-set?
	return getter(focus, kind);
});

reference reference::indirect-set((function<reference(reference,reference,reference)>)[](reference focus, reference kind, reference value) -> reference
{
	reference setter = basic-get(focus, indirect-setter);
	if (setter == null()) {
		setter = basic-set;
	}
	return setter(focus, indirect-setter, value);
	
	auto & map = focus.pointer()->data.get<part::kinded>();
	auto result = map.emplace(kind, value);
	if (result.second) {
		// insertion happened: no old element
		return null();
	} else {
		// kind already present
		reference old-value = result.first->second;
		result.first->second = value;
		return old-value;
	}
});
*/

reference const& reference::null() { static reference null((bool *****)"token_for_making_null_reference"); return null; }
DEFINE(reference, reference::, kindness-mistake)
DEFINE(reference, reference::, presence-mistake)

DEFINE(reference, reference::, method-kind-get)
DEFINE(reference, reference::, method-kind-set)
DEFINE(reference, reference::, method-get-all-kinds)
DEFINE(reference, reference::, method-order-count)
DEFINE(reference, reference::, method-order-get)
DEFINE(reference, reference::, method-order-set)
DEFINE(reference, reference::, method-operator-equals)
DEFINE(reference, reference::, method-operator-brackets)


/*
reference reference::operator[](reference kind)
{
	return get(*this, kind);
	// the getter returns a reference, not an entry
	// assignment won't work this way without change
	return this->get(::get)(*this, kind);
}
*/

bool reference::is-nonweak() const
{
	return (bool)shared;
}

void reference::set-nonweak(bool nonweak)
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

bool reference::operator!=(reference const & other) const
{
	return !(*this == other);
}

char const * reference::what() const noexcept
{
	auto pointing = pointer();
	if (!pointing) {
		return "null reference";
	}
	auto string-pointer = pointing->data.pointer<string>();
	if (string-pointer == nullptr) {
		return "reference not a string";
	}
	return string-pointer->c_str();
}

shared_ptr<reference::part> reference::pointer() const
{
	return is-nonweak() ? shared : weak.lock();
}

multi-any & reference::raw-data()
{
	auto pointing = pointer();
	if (!pointing) { throw presence-mistake(); }
	return pointing->data;
}
