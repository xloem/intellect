
#define ___STATIC_
#define ___EXTERN_ extern
# 1 "reference.cpp"
#include <string>
#include <unordered_map>
#include <vector>

#include "reference.stable.hxx"

using namespace std;

size_t std::hash<reference>::operator()(const reference & to_hash) const
{
	return hash<shared_ptr<reference::part>>()(to_hash.pointer());
}

#undef ___STATIC_
#undef ___EXTERN_
#define ___CLASSNAME_ reference::part
#define ___CLASSNAMESTR_ "reference::part"
#define ___STATIC_ static
#define ___EXTERN_ static
class reference::part
# 15 "reference.cpp"
{
private:
	friend class reference;

	// for multiple, place a reference that has ordered content
	using kinded = unordered_map<reference, reference>;
	using ordered = vector<reference>;

	// we could just use ordered as sets.  a smidge slow to query for membership, optimize_later.

	multi_any data;
};
#undef ___CLASSNAME_
#undef ___CLASSNAMESTR_
#undef ___STATIC_
#define ___STATIC_
#undef ___EXTERN_
#define ___EXTERN_ extern
# 27 "reference.cpp"

reference::reference(any data)
: shared(new part())
{
	raw_data().set(move(data));
}
reference::reference(initializer_list<any> datas)
: shared(new part())
{
	for (auto & data : datas) {
		auto old = raw_data().set(move(data));
		if (old.type() != typeid(void)) {
			// two of the same type passed
			throw kindness_mistake();
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
	
	if (kind == null()) { throw presence_mistake(); }

	// use method_kind_get to get kind
	reference getter(null());
	try {
		getter.shared = basic_kind_get()(*this, method_kind_get()).pointer();
	} catch (reference) { }
	if (getter == null()) {
		getter.shared = basic_kind_get().pointer();
	}

	// default to basic functions
	reference method(null());
	try {
		method.shared = getter(*this, kind).pointer();
	} catch (reference) { }
	if (method == null()) {
		try {
			method.shared = getter(recognised_methods(), kind).pointer();
		} catch (reference) { }
	}
#if 0
	if (method == null()) {
		//method = kind; // <- this would simplify things, and may be a comparable hack to the below
		/*
		if (kind == method_kind_get()) {
			method = basic_kind_get();
		} else if (kind == method_kind_set()) {
			method = basic_kind_set();
		} else if (kind == method_get_all_kinds()) {
			method = basic_get_all_kinds();
		} else if (kind == method_order_count()) {
			method = basic_order_count();
		} else if (kind == method_order_get()) {
			method = basic_order_get();
		} else if (kind == method_order_set()) {
			method = basic_order_set();
		} else*/ if (kind == method_operator_equals()) {
			// default implementation can't be done with a method call because references are passed to methods by copy for now
			if (parameters.size() != 1) { throw kindness_mistake(); }
			shared = parameters.begin()->pointer();
			return *this;
		}
	}
#endif
	if (method == basic_operator_equals() || (method == null() && kind == method_operator_equals())) {
		// default implementation can't be done with a method call because references are passed to methods by copy for now
		if (parameters.size() != 1) { throw kindness_mistake(); }
		reseat(*parameters.begin());
		return *this;
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

void reference::reseat(reference const & other) {
	// this is just operator_equals ... subclasses using reference
	// want a way to adjust the c++ meaning without worrying about
	// runtime changes.  don't remember how to do that with
	// method implementation .... basic_operator_equals?
	// and then how do subclasses like unique_data change?  to manage
	// index?
	shared = other.pointer();
}

___STATIC_ reference & reference::recognised_methods() { static reference storage; return storage; }
void reference::recognise_method(reference method_kind, reference basic_implementation, char const * classname, char const * methodname)
{
	basic_kind_set()(recognised_methods(), method_kind, basic_implementation);
}

___STATIC_ reference & reference::method_kind_get() { static reference storage({string("reference::method-kind-get")}); static int registration_barrier = ((recognise_method(storage, basic_kind_get(), "reference", "kind-get")),0); (void)registration_barrier; return storage; }
reference reference::kind_get(reference kind) { return (*this)(method_kind_get(), {   kind}); }
reference & reference::basic_kind_get(){static reference storage({string("kind-get"),(std::function<reference(reference, reference kind)>)
[](reference __uncasted_self, reference kind) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 151 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 152 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
# 153 "reference.cpp"
	auto result = map.find(kind);
# 154 "reference.cpp"
	if (result == map.end()) {
# 155 "reference.cpp"
		return (reference)(__return_type)null();
# 156 "reference.cpp"
	} else {
# 157 "reference.cpp"
		return (reference)(__return_type)result->second;
# 158 "reference.cpp"
	}
# 159 "reference.cpp"
}}}); return storage;}
# 160 "reference.cpp"

___STATIC_ reference & reference::method_kind_get_or_create_empty() { static reference storage({string("reference::method-kind-get-or-create-empty")}); static int registration_barrier = ((recognise_method(storage, basic_kind_get_or_create_empty(), "reference", "kind-get-or-create-empty")),0); (void)registration_barrier; return storage; }
reference reference::kind_get_or_create_empty(reference kind) { return (*this)(method_kind_get_or_create_empty(), {   kind}); }
reference & reference::basic_kind_get_or_create_empty(){static reference storage({string("kind-get-or-create-empty"),(std::function<reference(reference, reference kind)>)
[](reference __uncasted_self, reference kind) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 163 "reference.cpp"
	reference result = self.kind_get(kind);
# 164 "reference.cpp"
	if (null() == result) {
# 165 "reference.cpp"
		result = reference();
# 166 "reference.cpp"
		self.kind_set(kind, result);
# 167 "reference.cpp"
	}
# 168 "reference.cpp"
	return (reference)(__return_type)result;
# 169 "reference.cpp"
}}}); return storage;}
# 170 "reference.cpp"

___STATIC_ reference & reference::method_kind_set() { static reference storage({string("reference::method-kind-set")}); static int registration_barrier = ((recognise_method(storage, basic_kind_set(), "reference", "kind-set")),0); (void)registration_barrier; return storage; }
reference reference::kind_set(reference kind, reference value) { return (*this)(method_kind_set(), {   kind,   value}); }
reference & reference::basic_kind_set(){static reference storage({string("kind-set"),(std::function<reference(reference, reference kind, reference value)>)
[](reference __uncasted_self, reference kind, reference value) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 173 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 174 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
# 175 "reference.cpp"
	if (value == null()) {
# 176 "reference.cpp"
		auto spot = map.find(kind);
# 177 "reference.cpp"
		if (spot == map.end()) {
# 178 "reference.cpp"
			// already unfilled
# 179 "reference.cpp"
			return (reference)(__return_type)null();
# 180 "reference.cpp"
		} else {
# 181 "reference.cpp"
			// remove value and return (reference)(__return_type)it
# 182 "reference.cpp"
			auto result = spot->second;
# 183 "reference.cpp"
			map.erase(spot);
# 184 "reference.cpp"
			return (reference)(__return_type)result;
# 185 "reference.cpp"
		}
# 186 "reference.cpp"
	}
# 187 "reference.cpp"
	auto result = map.emplace(kind, value);
# 188 "reference.cpp"
	if (result.second) {
# 189 "reference.cpp"
		// insertion happened: no old element
# 190 "reference.cpp"
		return (reference)(__return_type)null();
# 191 "reference.cpp"
	} else {
# 192 "reference.cpp"
		// kind already present
# 193 "reference.cpp"
		reference old_value = result.first->second;
# 194 "reference.cpp"
		result.first->second = value;
# 195 "reference.cpp"
		return (reference)(__return_type)old_value;
# 196 "reference.cpp"
	}
# 197 "reference.cpp"
}}}); return storage;}
# 198 "reference.cpp"

___STATIC_ reference & reference::method_get_all_kinds() { static reference storage({string("reference::method-get-all-kinds")}); static int registration_barrier = ((recognise_method(storage, basic_get_all_kinds(), "reference", "get-all-kinds")),0); (void)registration_barrier; return storage; }
reference reference::get_all_kinds() { return (*this)(method_get_all_kinds(), { }); }
reference & reference::basic_get_all_kinds(){static reference storage({string("get-all-kinds"),(std::function<reference(reference)>)
[](reference __uncasted_self) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 201 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 202 "reference.cpp"
	auto & map = self.pointer()->data.get<part::kinded>();
# 203 "reference.cpp"
	reference result;
# 204 "reference.cpp"
	for (auto & item : map) {
# 205 "reference.cpp"
		result.order_set(result.order_count(), item.first);
# 206 "reference.cpp"
	}
# 207 "reference.cpp"
	return (reference)(__return_type)result;
# 208 "reference.cpp"
}}}); return storage;}
# 209 "reference.cpp"

___STATIC_ reference & reference::method_order_count() { static reference storage({string("reference::method-order-count")}); static int registration_barrier = ((recognise_method(storage, basic_order_count(), "reference", "order-count")),0); (void)registration_barrier; return storage; }
reference reference::order_count() { return (*this)(method_order_count(), { }); }
reference & reference::basic_order_count(){static reference storage({string("order-count"),(std::function<reference(reference)>)
[](reference __uncasted_self) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 212 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 213 "reference.cpp"
	return (reference)(__return_type)(any)(index_t)self.pointer()->data.get<part::ordered>().size();
# 214 "reference.cpp"
}}}); return storage;}
# 215 "reference.cpp"

___STATIC_ reference & reference::method_order_get() { static reference storage({string("reference::method-order-get")}); static int registration_barrier = ((recognise_method(storage, basic_order_get(), "reference", "order-get")),0); (void)registration_barrier; return storage; }
reference reference::order_get(reference index) { return (*this)(method_order_get(), {   index}); }
reference & reference::basic_order_get(){static reference storage({string("order-get"),(std::function<reference(reference, reference index)>)
[](reference __uncasted_self, reference index) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 218 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 219 "reference.cpp"
	index_t index_data = index.data<index_t>();
# 220 "reference.cpp"
	auto & vector = self.pointer()->data.get<part::ordered>();
# 221 "reference.cpp"
	if (index_data < 0 || index_data >= (index_t)vector.size()) {
# 222 "reference.cpp"
		throw presence_mistake();
# 223 "reference.cpp"
	}
# 224 "reference.cpp"
	return (reference)(__return_type)vector[index_data];
# 225 "reference.cpp"
}}}); return storage;}
# 226 "reference.cpp"

___STATIC_ reference & reference::method_order_get_or_create_empty() { static reference storage({string("reference::method-order-get-or-create-empty")}); static int registration_barrier = ((recognise_method(storage, basic_order_get_or_create_empty(), "reference", "order-get-or-create-empty")),0); (void)registration_barrier; return storage; }
reference reference::order_get_or_create_empty(reference index) { return (*this)(method_order_get_or_create_empty(), {   index}); }
reference & reference::basic_order_get_or_create_empty(){static reference storage({string("order-get-or-create-empty"),(std::function<reference(reference, reference index)>)
[](reference __uncasted_self, reference index) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 229 "reference.cpp"
	reference result = self.order_get(index);
# 230 "reference.cpp"
	if (null() == result) {
# 231 "reference.cpp"
		result = reference();
# 232 "reference.cpp"
		self.order_set(index, result);
# 233 "reference.cpp"
	}
# 234 "reference.cpp"
	return (reference)(__return_type)result;
# 235 "reference.cpp"
}}}); return storage;}
# 236 "reference.cpp"

___STATIC_ reference & reference::method_order_set() { static reference storage({string("reference::method-order-set")}); static int registration_barrier = ((recognise_method(storage, basic_order_set(), "reference", "order-set")),0); (void)registration_barrier; return storage; }
reference reference::order_set(reference index, reference value) { return (*this)(method_order_set(), {   index,   value}); }
reference & reference::basic_order_set(){static reference storage({string("order-set"),(std::function<reference(reference, reference index, reference value)>)
[](reference __uncasted_self, reference index, reference value) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 239 "reference.cpp"
	if (!self.pointer()) { throw presence_mistake(); }
# 240 "reference.cpp"
	auto & ordered_parts = self.pointer()->data.get<part::ordered>();
# 241 "reference.cpp"
	index_t index_data = index.data<index_t>();
# 242 "reference.cpp"
	if (value != null()) {
# 243 "reference.cpp"
		if (index_data < 0 || index_data > (index_t)ordered_parts.size()) {
# 244 "reference.cpp"
			throw presence_mistake();
# 245 "reference.cpp"
		}
# 246 "reference.cpp"
		if (index_data == (index_t)ordered_parts.size()) {
# 247 "reference.cpp"
			ordered_parts.emplace_back(value);
# 248 "reference.cpp"
			return (reference)(__return_type)null();
# 249 "reference.cpp"
		} else {
# 250 "reference.cpp"
			reference old_item = ordered_parts[index_data];
# 251 "reference.cpp"
			ordered_parts[index_data] = value;
# 252 "reference.cpp"
			return (reference)(__return_type)old_item;
# 253 "reference.cpp"
		}
# 254 "reference.cpp"
	} else {
# 255 "reference.cpp"
		if (index_data < 0 || index_data >= (index_t)ordered_parts.size()) {
# 256 "reference.cpp"
			throw presence_mistake();
# 257 "reference.cpp"
		}
# 258 "reference.cpp"
		reference old_item = ordered_parts[index_data];
# 259 "reference.cpp"
		ordered_parts.erase(ordered_parts.begin() + index_data);
# 260 "reference.cpp"
		return (reference)(__return_type)old_item;
# 261 "reference.cpp"
	}
# 262 "reference.cpp"
}}}); return storage;}
# 263 "reference.cpp"

/*
reference reference::get((function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	reference getter = basic_get(focus, kind_get);
	if (getter == null()) {
		getter = basic_get;
	}
	// we'll also want to have kind_set default to basic_set?
	return getter(focus, kind);
});

reference reference::indirect_set((function<reference(reference,reference,reference)>)[](reference focus, reference kind, reference value) -> reference
{
	reference setter = basic_get(focus, indirect_setter);
	if (setter == null()) {
		setter = basic_set;
	}
	return setter(focus, indirect_setter, value);
	
	auto & map = focus.pointer()->data.get<part::kinded>();
	auto result = map.emplace(kind, value);
	if (result.second) {
		// insertion happened: no old element
		return null();
	} else {
		// kind already present
		reference old_value = result.first->second;
		result.first->second = value;
		return old_value;
	}
});
*/

reference const& reference::null() { static reference null((bool *****)"token_for_making_null_reference"); return null; }
___STATIC_ reference & agreement_mistake() { static reference storage; return storage; }
___STATIC_ reference & kindness_mistake() { static reference storage; return storage; }
___STATIC_ reference & presence_mistake() { static reference storage; return storage; }

___STATIC_ reference & reference::method_operator_equals() { static reference storage({string("reference::method-operator-equals")}); static int registration_barrier = ((recognise_method(storage, basic_operator_equals(), "reference", "operator-equals")),0); (void)registration_barrier; return storage; }
reference reference::operator_equals(reference other) { return (*this)(method_operator_equals(), {   other}); }
reference & reference::basic_operator_equals(){static reference storage({string("operator-equals"),(std::function<reference(reference, reference other)>)
[](reference __uncasted_self, reference other) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 304 "reference.cpp"
	throw presence_mistake;
# 305 "reference.cpp"
	return (reference)(__return_type){};
# 306 "reference.cpp"
}}}); return storage;}
# 307 "reference.cpp"
___STATIC_ reference & reference::method_operator_brackets() { static reference storage({string("reference::method-operator-brackets")}); static int registration_barrier = ((recognise_method(storage, basic_operator_brackets(), "reference", "operator-brackets")),0); (void)registration_barrier; return storage; }
reference reference::operator_brackets(reference other) { return (*this)(method_operator_brackets(), {   other}); }
reference & reference::basic_operator_brackets(){static reference storage({string("operator-brackets"),(std::function<reference(reference, reference other)>)
[](reference __uncasted_self, reference other) -> reference { using __return_type = reference; reference self = __uncasted_self;{
# 309 "reference.cpp"
	throw presence_mistake;
# 310 "reference.cpp"
	return (reference)(__return_type){};
# 311 "reference.cpp"
}}}); return storage;}
# 312 "reference.cpp"

/*
___STATIC_ reference & reference::method_destruct() { static reference storage({string("reference::method-destruct")}); static int registration_barrier = ((recognise_method(storage, basic_destruct(), "reference", "destruct")),0); (void)registration_barrier; return storage; }
void reference::destruct() {  (*this)(method_destruct(), { }); }
reference & reference::basic_destruct(){static reference storage({string("destruct"),(std::function<void(reference)>)
[](reference __uncasted_self) -> void {  reference self = __uncasted_self;{
# 316 "reference.cpp"
}}}); return storage;}
# 317 "reference.cpp"
reference::~reference()
{
	if (is_nonweak() && shared.use_count() == 1) {
		destruct();
	}
}
*/


/*
reference reference::operator[](reference kind)
{
	return get(*this, kind);
	// the getter returns a reference, not an entry
	// assignment won't work this way without change
	return this->get(::get)(*this, kind);
}
*/

bool reference::is_nonweak() const
{
	return (bool)shared;
}

void reference::set_nonweak(bool nonweak)
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
	auto string_pointer = pointing->data.pointer<string>();
	if (string_pointer == nullptr) {
		return "reference not a string";
	}
	return string_pointer->c_str();
}

shared_ptr<reference::part> reference::pointer() const
{
	return is_nonweak() ? shared : weak.lock();
}

multi_any & reference::raw_data()
{
	auto pointing = pointer();
	if (!pointing) { throw presence_mistake(); }
	return pointing->data;
}

multi_any const & reference::raw_data() const
{
	auto pointing = pointer();
	if (!pointing) { throw presence_mistake(); }
	return pointing->data;
}
