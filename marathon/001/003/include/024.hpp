#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template <typename t>
using il = std::initializer_list<t>;

using ref = std::shared_ptr<concept>;
using cxxcode = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

#define sym(name) ref name = create({},::text(#name))

// make a variable
#define var(name) ref name = create({{symbols::variable,symbols::variable}},::text(#name))

namespace symbols
{
	sym(nothing);

	sym(what);
	sym(next);
	sym(previous);
	sym(first);
	sym(last);

	sym(context);
	sym(outer);

	sym(state);
	sym(step);
	sym(steps);
	sym(variable);
	sym(inputs);
	sym(outputs);

	sym(hello);
	sym(world);
	ref space = create({}, text(" "));
	ref endl = create({}, text("\n"));
}

struct concept
{
	std::unordered_multimap<ref,ref> refs;
	std::any _data;

	template <typename t>
	t & data()
	{
		t * result = std::any_cast<t>(&_data);
		if (result == 0) { throw symbols::nothing; }
		return *result;
	}

	void call(ref context)
	{
		data<cxxcode>()(context);
	}

	ref get(ref what)
	{
		auto result = refs.find(what);
		if (result == refs.end()) {
			return symbols::nothing;
		} else {
			return result->second;
		}
	}

	void add(ref what, ref value)
	{
		if (!what) { throw what; }
		if (!value) { throw value; }
		refs.insert(std::pair(what, value));
	}

	decltype(refs)::iterator wipe(ref what)
	{
		auto range = refs.equal_range(what);
		return refs.erase(range.first, range.second);
	}

	void set(ref what, ref value = symbols::nothing)
	{
		if (!value->isthing()) { value = what; }
		refs.insert(wipe(what), std::pair(what, value));
	}

	ref clone()
	{
		ref result = create();
		result->_data = _data;
		result->refs = refs;
		return result;
	}

	bool isthing()
	{
		return &*symbols::nothing != this;
	}
};

#include "extra.hpp"

// Create a new concept
ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->_data = data;
	result->refs.insert(refs.begin(), refs.end());
	verify(result);
	return result;
}

// Link concepts in a list with symbolic refs
ref seq(il<ref> items)
{
	ref first = symbols::nothing;
	ref next;
	for (auto & item : items)
	{
		ref last = next;
		next = item;
		if (!last) {
			first = next;
		} else {
			last->set(symbols::next, next);
			next->set(symbols::previous, last);
		}
	}
	if (next) {
		first->set(symbols::first, first);
		first->set(symbols::last, next);
	}
	return first;
}

ref seq_append(ref sequence, ref what)
{
	if (!sequence->isthing()) {
		return seq({what});
	}
	ref last = sequence->get(symbols::last);
	sequence->set(symbols::last, what);
	sequence->set(symbols::first, what);
	last->set(symbols::next, what);
	what->set(symbols::previous, last);
	sequence->set(symbols::last, what);
	return sequence;
}

ref encapsulate(ref what)
{
	return create({{symbols::what, what}});
}

ref decapsulate(ref what)
{
	return what->get(symbols::what);
}

ref recapsulate(ref what)
{
	return encapsulate(decapsulate(what));
}

