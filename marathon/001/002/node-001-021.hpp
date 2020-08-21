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

#define symbol(name) ref name = create({},::text(#name))

namespace symbols
{
	symbol(nothing);

	symbol(what);
	symbol(next);

	symbol(context);
	symbol(outer);

	symbol(state);
	symbol(step);
	symbol(steps);
	symbol(variable);
	symbol(inputs);
	symbol(outputs);

	symbol(hello);
	symbol(world);
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
		refs.insert(std::pair(what, value));
	}

	decltype(refs)::iterator wipe(ref what)
	{
		auto range = refs.equal_range(what);
		return refs.erase(range.first, range.second);
	}

	void set(ref what, ref value = symbols::nothing)
	{
		if (!value) { value = what; }
		refs.insert(wipe(what), std::pair(what, value));
	}

	bool isthing()
	{
		return &*symbols::nothing != this;
	}
};

#include "../001/extra.hpp"

// Create a new concept
ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->_data = data;
	result->refs.insert(refs.begin(), refs.end());
	verify(result);
	return result;
}

// Link concepts in a list, by symbols::next ref
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
		}
	}
	return first;
}
