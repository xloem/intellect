#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>

// so maybe a function to remove a link from one node, and place it into another

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
	
	// this is a multimap instead of an unordered_multimap to ease the possibility of
	// link referencing.  iterators are not invalidated when a normal multimap is mutated.
	std::multimap<basic_ref,basic_ref> refs;
	std::any data;
};
using basic_link = decltype(basic_concept::refs)::iterator;

class ref;

using r = std::pair<ref,ref>;
using rs = il<r>;

template <class t>
il<il<ref>> assumes_has = {};

class ref : public basic_ref
{
public:
	ref (rs refs = {}, std::any data = {});
	ref (r refs, std::any data = {});
	ref(basic_ref const & basic);

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

	ref & operator+=(rs refs);
	ref & operator+=(r refs);
	ref & operator-=(il<ref> refs);
	ref & operator-=(ref refs);

	ref operator+(rs refs);
	ref operator+(r refs);

	ref operator-(il<ref> refs);
	ref operator-(ref refs);

	// sets only the passed refs
	ref & operator<=(il<std::pair<ref,ref>> refs);
	ref & operator<=(std::pair<ref,ref> refs);

	bool has_data();

	template <typename t>
	void data(t const & value)
	{
		if (has_data()) { throw "oops"; }
		(**this).data = value;
	}

	template <typename t>
	t & data()
	{
		t * result = std::any_cast<t>(&data<std::any>());
		if (result == 0) { throw result; }
		return *result;
	}

	template <typename t>
	t const & data() const
	{
		t const * result = std::any_cast<t const>(&data<std::any>());
		if (result == 0) { throw result; }
		return *result;
	}

	operator bool() const;

	// TODO MAYBE: prefer using link objects to refs
	unsigned long count(ref what) const;
	ref const get(ref what) const;
	ref take(ref what);
	//std::unordered_set<ref> getall(ref what) const;

	void add(ref what, ref value);

	void set(ref what) { set(what, what); }

	void set(ref what, ref value);

	basic_link wipe(ref what);

	ref const operator[](ref what) const;

	/*
	for operator[] see ../medium-001/assignable*.hpp

	or use below implementation which fills empty properties in with 'nothing' which sticks
	around forever.  fine for small systems =)
		
	*/
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


#if defined(ref_cpp)
#include "exception.hpp"
#define self (*this)

basic_ref create(il<std::pair<basic_ref,basic_ref>> refs, std::any data)
{
	return ref((il<std::pair<ref,ref>> &)refs, data);
}

ref::ref (rs refs, std::any data)
: basic_ref(new basic_concept{{refs.begin(), refs.end()}, data})
{ }

ref::ref (r refs, std::any data)
: ref({refs}, data)
{ }

ref::ref(basic_ref const & basic)
: basic_ref(basic)
{ }

void ref::verify_has(il<il<ref>> refs) const
{
	std::unordered_set<unsigned long> links;
	/* // FOR UNORDERED_MAP
	unsigned long bucket_count = (**this).refs.bucket_count();
	*/
	for (std::vector<ref> assumption : refs) {
		bool found = false;
		ref found_other = sym::nothing;
		if (!assumption.size()) { continue; }
		unsigned long index = 0;
		/* // FOR UNORDERED_MAP
		unsigned long bucket = (**this).refs.bucket(assumption[0]);
		for (auto link = (**this).refs.begin(bucket); link != (**this).refs.end(bucket); ++ link) {
			index += bucket_count;
			if (link->first != assumption[0]) {
				continue;
			}
			if (assumption.size() > 1 && link->second != assumption[1]) {
				found_other = link->second;
				continue;
			}
			unsigned long hash = index + bucket;
			if (links.count(hash)) {
				continue;
			}
			links.insert(hash);
			found = true;
		}
		*/
		auto range = self->refs.equal_range(assumption[0]);
		unsigned long hash = std::hash<basic_ref>()(assumption[0]) * self->refs.size();
		for (auto link = range.first; link != range.second; ++ link) {
			index ++;
			if (assumption.size() > 1 && link->second != assumption[1]) {
				found_other = link->second;
				continue;
			}
			if (links.count(hash + index)) {
				continue;
			}
			links.insert(hash + index);
			found = true;
		}
		if (!found) {
			if (assumption.size() == 1) {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
				};
			}
			if (!found_other) {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]}
				};
			} else {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]},
					{sym::actually_has_to, found_other}
				};
			}
		}
	}
}

ref & ref::operator+=(rs refs)
{
	self->refs.insert(refs.begin(), refs.end());
	return self;
}

ref & ref::operator+=(r refs)
{
	add(refs.first, refs.second);
	return self;
}

ref & ref::operator-=(il<ref> refs)
{
	for (auto item : refs)
	{
		self -= item;
	}
	return self;
}
ref & ref::operator-=(ref refs)
{
	self->refs.erase(refs);
	return self;
}

ref ref::operator+(rs refs)
{
	ref result(refs);
	result->refs.insert(self->refs.begin(), self->refs.end());
	return result;
}
ref ref::operator+(r refs)
{
	return self + rs{refs};
}

ref ref::operator-(il<ref> refs)
{
	ref result = self + rs{};
	result -= refs;
	return result;
}
ref ref::operator-(ref refs)
{
	return self - il<ref>{refs};
}

// sets only the passed refs
ref & ref::operator<=(il<std::pair<ref,ref>> refs)
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
ref & ref::operator<=(std::pair<ref,ref> refs)
{
	return self <= il<std::pair<ref,ref>>{refs};
}

template <>
std::any & ref::data<std::any>()
{
	return self->data;
}

template <>
std::any const & ref::data<std::any>() const
{
	return self->data;
}

bool ref::has_data()
{
	return data<std::any>().has_value();
}

ref::operator bool() const
{
	return self != sym::nothing;
}

unsigned long ref::count(ref what) const
{
	unsigned long result = 0;
	auto range = self->refs.equal_range(what);
	for (auto iterator = range.first; iterator != range.second; ++ iterator) {
		++ result;
	}
	return result;
}

ref const ref::get(ref what) const
{
	auto result = self->refs.find(what);
	if (result == self->refs.end()) {
		return sym::nothing;
	} else {
		return result->second;
	}
}

ref ref::take(ref what)
{
	auto iterator = self->refs.find(what);
	if (iterator == self->refs.end()) {
		return sym::nothing;
	} else {
		ref result = iterator->second;
		self->refs.erase(iterator);
		return result;
	}
}

/*
std::unordered_set<ref> ref::getall(ref what) const
{
	std::unordered_set<ref> result;
	auto range = self->refs.equal_range(what);
	for (auto item : range) {
		
	}
	if (result == (**this).refs.end()) {
		return sym::nothing;
	} else {
		return result->second;
	}
}
*/

void ref::add(ref what, ref value)
{
	// if(!what) { throw }
	// if(!value) { throw }
	self->refs.insert(std::pair<basic_ref,basic_ref>(what, value));
}

void ref::set(ref what, ref value)
{
	auto pos = wipe(what);
	if (value) {
		self->refs.insert(pos, std::pair<basic_ref,basic_ref>(what, value));
	} else {
		// setting to nothing erased but did not produce success
		// been found to be a common use to erase
		// note that get() returns nothing for erased members
	}
}

basic_link ref::wipe(ref what)
{
	auto range = self->refs.equal_range(what);
	return self->refs.erase(range.first, range.second);
}

ref const ref::operator[](ref what) const
{
	return self.get(what);
}

#undef self
#endif // defined(main_unit)

