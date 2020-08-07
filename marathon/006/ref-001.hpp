#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

template <class t>
using il = std::initializer_list<t>;

using basic_ref = std::shared_ptr<struct basic_concept>;
basic_ref create(il<std::pair<basic_ref,basic_ref>> refs = {}, std::any data = {});

#if defined main_unit
#define symbol(name) basic_ref name = create({}, std::string(#name));
#else
#define symbol(name) extern basic_ref name;
#endif

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

class ref;

template <class t>
il<il<ref>> assumes_has = {};

class ref : public basic_ref
{
public:
	ref (il<std::pair<ref,ref>> refs = {}, std::any data = {})
	: basic_ref(new basic_concept{{refs.begin(), refs.end()}, data})
	{ }

	ref(basic_ref const & basic)
	: basic_ref(basic)
	{ }

	template <class t>
	t as() // this template annotation could be moved to a single subclass
	{        // for use-is-more-frequent-than-design.  then every
	         // subclass would inherit ref's copy-constructor,
		 // and this code would move to ref constructor.
		return (t)((ref const *)this)->as<t>();
	}

	void verify_has(il<il<ref>> refs) const;

	template <class t>
	t const as() const
	         // this template annotation could be moved to a single subclass
	{        // for use-is-more-frequent-than-design.  then every
	         // subclass would inherit ref's copy-constructor,
		 // and this code would move to ref constructor.

		t const & result = static_cast<t const&>(*this);

		verify_has(assumes_has<t>);

		return result;
	}

	ref & operator+=(il<std::pair<ref,ref>> refs)
	{
		(**this).refs.insert(refs.begin(), refs.end());
		return *this;
	}

	ref & operator+=(std::pair<ref,ref> refs)
	{
		add(refs.first, refs.second);
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

	ref operator+(il<std::pair<ref,ref>> refs)
	{
		ref result(refs);
		result->refs.insert((**this).refs.begin(), (**this).refs.end());
		return result;
	}

	ref operator-(il<ref> refs)
	{
		ref result = (*this) + il<std::pair<ref,ref>>{};
		result -= refs;
		return result;
	}

	// note: this doesn't wipe unmentioned items
	ref & operator=(il<std::pair<ref,ref>> refs)
	{
		std::unordered_set<basic_ref> found;
		for (auto item : refs)
		{
			if (!found.count(item.first)) {
				set(item.first, item.second);
				found.insert(item.first);
			} else {
				add(item.first, item.second);
			}
		}
		return *this;
	}

	bool has_data()
	{
		return ((**this).data.has_value());
	}

	template <typename t>
	void data(t const & value)
	{
		if (has_data()) { throw "oops"; }
		(**this).data = value;
	}

	template <typename t>
	t & data()
	{
		t * result = std::any_cast<t>(&(**this).data);
		if (result == 0) { throw result; }
		return *result;
	}

	template <typename t>
	t const & data() const
	{
		t const * result = std::any_cast<t const>(&(**this).data);
		if (result == 0) { throw result; }
		return *result;
	}

	operator bool() const;

	ref const get(ref what) const;
	//std::unordered_set<ref> getall(ref what) const;

	void add(ref what, ref value)
	{
		// if(!what) { throw }
		// if(!value) { throw }
		(**this).refs.insert(std::pair<basic_ref,basic_ref>(what, value));
	}

	void set(ref what)
	{
		set(what, what);
	}

	void set(ref what, ref value)
	{
		auto pos = wipe(what);
		if (value) {
			(**this).refs.insert(pos, std::pair<basic_ref,basic_ref>(what, value));
		} else {
			// setting to nothing erased but did not produce success
			// been found to be a common use to erase
			// note that get() returns nothing for erased members
		}
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
