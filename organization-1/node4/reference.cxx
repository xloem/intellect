
#define ___STATIC_
#define ___EXTERN_ extern
//# 1 "reference.cpp"
#include <string>
#include <unordered_map>
#include <vector>

#include "reference.hxx"

using namespace std;

size_t std::hash<reference>::operator()(const reference & to__hash) const
{
	return hash<shared_ptr<reference::part>>()(to__hash.pointer());
}

#undef ___STATIC_
#undef ___EXTERN_
#define ___CLASSNAME_ reference::part
#define ___CLASSNAMESTR_ "reference::part"
#define ___STATIC_ static
#define ___EXTERN_ static
class reference::part
//# 15 "reference.cpp"
{
private:
	friend class reference;

	using kinded = unordered_map<reference, reference>;
	using ordered = vector<reference>;

	multi__any data;
};
#undef ___CLASSNAME_
#undef ___CLASSNAMESTR_
#undef ___STATIC_
#define ___STATIC_
#undef ___EXTERN_
#define ___EXTERN_ extern
//# 24 "reference.cpp"

reference::reference(any data)
: shared(new part())
{
	raw__data().set(move(data));
}
reference::reference(initializer_list<any> datas)
: shared(new part())
{
	for (auto & data : datas) {
		auto old = raw__data().set(move(data));
		if (old.type() != typeid(void)) {
			// two of the same type passed
			throw kindness__mistake();
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
	// is possibility for stack overflow.  so it assigns with .shared .
	
	if (kind == null()) { throw presence__mistake(); }

	// use method__kind__get to get kind
	reference getter(null());
	try {
		getter.shared = basic__kind__get()(*this, method__kind__get()).pointer();
	} catch (reference) { }
	if (getter == null()) {
		getter.shared = basic__kind__get().pointer();
	}

	// default to basic functions
	reference method(null());
	try {
		method.shared = getter(*this, kind).pointer();
	} catch (reference) { }
	if (method == null()) {
		try {
			method.shared = getter(registered__methods(), kind).pointer();
		} catch (reference) { }
	}
	if (method == null()) {
		//method = kind; // <- this would simplify things, and may be a comparable hack to the below
		/*
		if (kind == method__kind__get()) {
			method = basic__kind__get();
		} else if (kind == method__kind__set()) {
			method = basic__kind__set();
		} else if (kind == method__get__all__kinds()) {
			method = basic__get__all__kinds();
		} else if (kind == method__order__count()) {
			method = basic__order__count();
		} else if (kind == method__order__get()) {
			method = basic__order__get();
		} else if (kind == method__order__set()) {
			method = basic__order__set();
		} else*/ if (kind == method__operator__equals()) {
			// default implementation can't be done with a method call because references are passed to methods by copy for now
			if (parameters.size() != 1) { throw kindness__mistake(); }
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

___STATIC_ reference & reference::registered__methods() { static reference storage; return storage; }
void reference::register__method(reference method__kind, reference basic__implementation, char const * classname, char const * methodname)
{
	basic__kind__set()(registered__methods(), method__kind, basic__implementation);
}

___STATIC_ reference & reference::method__kind__get() { static reference storage({string("reference::method-kind-get")}); static int registration__barrier = ((register__method(storage, basic__kind__get(), "reference", "kind-get")),0); (void)registration__barrier; return storage; }
reference reference::kind__get(reference kind) { return (*this)(method__kind__get(), {   kind}); }
reference & reference::basic__kind__get(){static reference storage({string("kind-get"),(function<reference(reference, reference kind)>)
[](reference __uncasted_self, reference kind) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 130 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 131 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
//# 132 "reference.cpp"
	auto result = map.find(kind);
//# 133 "reference.cpp"
	if (result == map.end()) {
//# 134 "reference.cpp"
		return (reference)(__return__type)null();
//# 135 "reference.cpp"
	} else {
//# 136 "reference.cpp"
		return (reference)(__return__type)result->second;
//# 137 "reference.cpp"
	}
//# 138 "reference.cpp"
}}}); return storage;}
//# 139 "reference.cpp"

___STATIC_ reference & reference::method__kind__set() { static reference storage({string("reference::method-kind-set")}); static int registration__barrier = ((register__method(storage, basic__kind__set(), "reference", "kind-set")),0); (void)registration__barrier; return storage; }
reference reference::kind__set(reference kind, reference value) { return (*this)(method__kind__set(), {   kind,   value}); }
reference & reference::basic__kind__set(){static reference storage({string("kind-set"),(function<reference(reference, reference kind, reference value)>)
[](reference __uncasted_self, reference kind, reference value) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 142 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 143 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
//# 144 "reference.cpp"
	auto result = map.emplace(kind, value);
//# 145 "reference.cpp"
	if (result.second) {
//# 146 "reference.cpp"
		// insertion happened: no old element
//# 147 "reference.cpp"
		return (reference)(__return__type)null();
//# 148 "reference.cpp"
	} else {
//# 149 "reference.cpp"
		// kind already present
//# 150 "reference.cpp"
		reference old__value = result.first->second;
//# 151 "reference.cpp"
		result.first->second = value;
//# 152 "reference.cpp"
		return (reference)(__return__type)old__value;
//# 153 "reference.cpp"
	}
//# 154 "reference.cpp"
}}}); return storage;}
//# 155 "reference.cpp"

___STATIC_ reference & reference::method__get__all__kinds() { static reference storage({string("reference::method-get-all-kinds")}); static int registration__barrier = ((register__method(storage, basic__get__all__kinds(), "reference", "get-all-kinds")),0); (void)registration__barrier; return storage; }
reference reference::get__all__kinds() { return (*this)(method__get__all__kinds(), { }); }
reference & reference::basic__get__all__kinds(){static reference storage({string("get-all-kinds"),(function<reference(reference)>)
[](reference __uncasted_self) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 158 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 159 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
//# 160 "reference.cpp"
	reference result;
//# 161 "reference.cpp"
	for (auto & item : map) {
//# 162 "reference.cpp"
		result.order__set(result.order__count(), item.first);
//# 163 "reference.cpp"
	}
//# 164 "reference.cpp"
	return (reference)(__return__type)result;
//# 165 "reference.cpp"
}}}); return storage;}
//# 166 "reference.cpp"

___STATIC_ reference & reference::method__order__count() { static reference storage({string("reference::method-order-count")}); static int registration__barrier = ((register__method(storage, basic__order__count(), "reference", "order-count")),0); (void)registration__barrier; return storage; }
reference reference::order__count() { return (*this)(method__order__count(), { }); }
reference & reference::basic__order__count(){static reference storage({string("order-count"),(function<reference(reference)>)
[](reference __uncasted_self) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 169 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 170 "reference.cpp"
	return (reference)(__return__type)(any)(index_t)self.pointer()->data.get<part::ordered>().size();
//# 171 "reference.cpp"
}}}); return storage;}
//# 172 "reference.cpp"

___STATIC_ reference & reference::method__order__get() { static reference storage({string("reference::method-order-get")}); static int registration__barrier = ((register__method(storage, basic__order__get(), "reference", "order-get")),0); (void)registration__barrier; return storage; }
reference reference::order__get(reference index) { return (*this)(method__order__get(), {   index}); }
reference & reference::basic__order__get(){static reference storage({string("order-get"),(function<reference(reference, reference index)>)
[](reference __uncasted_self, reference index) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 175 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 176 "reference.cpp"
	index_t index__data = index.data<index_t>();
//# 177 "reference.cpp"
	auto & vector = self.pointer()->data.get<part::ordered>();
//# 178 "reference.cpp"
	if (index__data < 0 || index__data >= (index_t)vector.size()) {
//# 179 "reference.cpp"
		throw presence__mistake();
//# 180 "reference.cpp"
	}
//# 181 "reference.cpp"
	return (reference)(__return__type)vector[index__data];
//# 182 "reference.cpp"
}}}); return storage;}
//# 183 "reference.cpp"

___STATIC_ reference & reference::method__order__set() { static reference storage({string("reference::method-order-set")}); static int registration__barrier = ((register__method(storage, basic__order__set(), "reference", "order-set")),0); (void)registration__barrier; return storage; }
reference reference::order__set(reference index, reference value) { return (*this)(method__order__set(), {   index,   value}); }
reference & reference::basic__order__set(){static reference storage({string("order-set"),(function<reference(reference, reference index, reference value)>)
[](reference __uncasted_self, reference index, reference value) -> reference { using __return__type = reference; reference self = __uncasted_self;{
//# 186 "reference.cpp"
	if (!self.pointer()) { throw presence__mistake(); }
//# 187 "reference.cpp"
	auto & ordered__parts = self.pointer()->data.get<part::ordered>();
//# 188 "reference.cpp"
	index_t index__data = index.data<index_t>();
//# 189 "reference.cpp"
	if (value != null()) {
//# 190 "reference.cpp"
		if (index__data < 0 || index__data > (index_t)ordered__parts.size()) {
//# 191 "reference.cpp"
			throw presence__mistake();
//# 192 "reference.cpp"
		}
//# 193 "reference.cpp"
		if (index__data == (index_t)ordered__parts.size()) {
//# 194 "reference.cpp"
			ordered__parts.emplace_back(value);
//# 195 "reference.cpp"
			return (reference)(__return__type)null();
//# 196 "reference.cpp"
		} else {
//# 197 "reference.cpp"
			reference old__item = ordered__parts[index__data];
//# 198 "reference.cpp"
			ordered__parts[index__data] = value;
//# 199 "reference.cpp"
			return (reference)(__return__type)old__item;
//# 200 "reference.cpp"
		}
//# 201 "reference.cpp"
	} else {
//# 202 "reference.cpp"
		if (index__data < 0 || index__data >= (index_t)ordered__parts.size()) {
//# 203 "reference.cpp"
			throw presence__mistake();
//# 204 "reference.cpp"
		}
//# 205 "reference.cpp"
		reference old__item = ordered__parts[index__data];
//# 206 "reference.cpp"
		ordered__parts.erase(ordered__parts.begin() + index__data);
//# 207 "reference.cpp"
		return (reference)(__return__type)old__item;
//# 208 "reference.cpp"
	}
//# 209 "reference.cpp"
}}}); return storage;}
//# 210 "reference.cpp"

/*
reference reference::get((function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	reference getter = basic__get(focus, kind__get);
	if (getter == null()) {
		getter = basic__get;
	}
	// we'll also want to have kind__set default to basic__set?
	return getter(focus, kind);
});

reference reference::indirect__set((function<reference(reference,reference,reference)>)[](reference focus, reference kind, reference value) -> reference
{
	reference setter = basic__get(focus, indirect__setter);
	if (setter == null()) {
		setter = basic__set;
	}
	return setter(focus, indirect__setter, value);
	
	auto & map = focus.pointer()->data.get<part::kinded>();
	auto result = map.emplace(kind, value);
	if (result.second) {
		// insertion happened: no old element
		return null();
	} else {
		// kind already present
		reference old__value = result.first->second;
		result.first->second = value;
		return old__value;
	}
});
*/

reference const& reference::null() { static reference null((bool *****)"token_for_making_null_reference"); return null; }
___STATIC_ reference & reference::kindness__mistake() { static reference storage; return storage; }
___STATIC_ reference & reference::presence__mistake() { static reference storage; return storage; }

___STATIC_ reference & reference::method__operator__equals() { static reference storage; return storage; }
___STATIC_ reference & reference::method__operator__brackets() { static reference storage; return storage; }


/*
reference reference::operator[](reference kind)
{
	return get(*this, kind);
	// the getter returns a reference, not an entry
	// assignment won't work this way without change
	return this->get(::get)(*this, kind);
}
*/

bool reference::is__nonweak() const
{
	return (bool)shared;
}

void reference::set__nonweak(bool nonweak)
{
	if (nonweak) {
		shared = pointer();
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
	auto string__pointer = pointing->data.pointer<string>();
	if (string__pointer == nullptr) {
		return "reference not a string";
	}
	return string__pointer->c_str();
}

shared_ptr<reference::part> reference::pointer() const
{
	return is__nonweak() ? shared : weak.lock();
}

multi__any & reference::raw__data()
{
	auto pointing = pointer();
	if (!pointing) { throw presence__mistake(); }
	return pointing->data;
}
