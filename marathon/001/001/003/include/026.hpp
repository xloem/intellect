#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template <typename t>
using il = std::initializer_list<t>;

using ref = std::shared_ptr<concept>;
/*
struct ref : public std::shared_ptr<concept>
{
	ref(il<std::pair<ref,ref>> refs = {}, std::any data = {});

	operator bool() const;
	ref & operator[](ref);
	ref const & operator[](ref) const;
};
*/
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
	sym(is);
	sym(next);
	sym(previous);
	sym(first);
	sym(last);
	sym(detail);

	sym(context);
	sym(outer);

	sym(state);
	sym(states);
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

void verify(ref what)
{
	for (auto refs : what->refs) {
		if (!refs.first || !refs.second) {
			throw refs;
		}
	}
}

// Create a new concept
ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->_data = data;
	result->refs.insert(refs.begin(), refs.end());
	verify(result);
	return result;
}
