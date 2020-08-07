#pragma once

#include <any>
#include <functional>
#include <memory>
#include <unordered_map>

template <class t>
using il = std::initializer_list<t>;

using basic_ref = std::shared_ptr<struct basic_concept>;
basic_ref create(il<std::pair<basic_ref,basic_ref>> refs = {}, std::any data = {});

#define symbol(name) basic_ref name = create({}, std::string(#name));

namespace sym {
	symbol(nothing);
	symbol(is);
	symbol(what);
};

// note: link objects could be temporary data refs where data is an iterator
struct basic_concept {
	// instead of a multimap, i think a map
	// of unordered sets would be more clear
	std::unordered_multimap<basic_ref,basic_ref> refs;
	std::any data;
};
using basic_link = decltype(basic_concept::refs)::iterator;

class ref : public basic_ref
{
public:
	ref (il<std::pair<ref,ref>> refs = {}, std::any data = {})
	: basic_ref(new basic_concept{{refs.begin(), refs.end()}, data})
	{ }

	ref(basic_ref const & basic)
	: basic_ref(basic)
	{ }

	virtual void assuming_is() {}

	template <class t>
	t & as() // this template annotation could be moved to the whole class
	{        // for use-is-more-frequent-than-design.  then every
	         // subclass would inherit ref's copy-constructor,
		 // and this code would move to ref constructor.
		t result = static_cast<t&>(*this);
		result.assuming_is();
		return result;
	}

	template <class t>
	t const & as() const
	{
		return static_cast<t const&>(*this);
	}

	ref & operator+=(il<std::pair<ref,ref>> refs)
	{
		(**this).refs.insert(refs.begin(), refs.end());
		return *this;
	}

	ref & operator-=(il<ref> refs)
	{
		for (auto item : refs)
		{
			(**this).refs.erase(item);
		}
		return *this;
	}

	template <typename t>
	t & data()
	{
		return *std::any_cast<t>(&(**this).data);
	}

	template <typename t>
	t const & data() const
	{
		return *std::any_cast<t const>(&(**this).data);
	}

	operator bool() const
	{
		return *this != sym::nothing;
	}

	ref const get(ref what) const
	{
		auto result = (**this).refs.find(what);
		if (result == (**this).refs.end()) {
			return sym::nothing;
		} else {
			return result->second;
		}
	}

	void add(ref what, ref value)
	{
		// if(!what) { throw }
		// if(!value) { throw }
		(**this).refs.insert(std::pair<basic_ref,basic_ref>(what, value));
	}

	void set(ref what, ref value = sym::nothing)
	{
		if (!value) { value = what; }
		(**this).refs.insert(wipe(what), std::pair<basic_ref,basic_ref>(what, value));
	}

	basic_link wipe(ref what)
	{
		auto range = (**this).refs.equal_range(what);
		return (**this).refs.erase(range.first, range.second);
	}

	ref const operator[](ref what) const
	{
		return get(what);
	}

	/*

	ref & operator[](ref what)
	{
		auto result = (**this).refs.find(what);
		if (result == (**this).refs.end()) {
			result = (**this).refs.insert({what, sym::nothing});
		}
		return *(ref*)&result->second;
	}
	*/
};

basic_ref create(il<std::pair<basic_ref,basic_ref>> refs, std::any data)
{
	return ref((il<std::pair<ref,ref>> &)refs, data);
}
