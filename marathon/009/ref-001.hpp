#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <map>
#include <unordered_set>

template <class t>
using il = std::initializer_list<t>;

using r = std::pair<ref,ref>;
using rs = il<r>;

// holds concept data
struct con;

using basic_link = std::multimap<ref,ref>::iterator;

// refers to concept data, can be copied lightweight
struct ref : public std::shared_ptr<con>
{
	ref(rs refs = {}, std::any data = {});
	ref(r refs, std::any day = {});

	ref clone();

	// TODO MAYBE: prefer using link objects to refs

	ref get(ref what) const;
	void set(ref what, ref value);

	void add(ref what, ref value);
	unsigned long count(ref what) const;
	std::vector<ref> getall(ref what) const;
	//std::vector<ref> getall(ref what) const;

	basic_link del(ref what);
	void del(ref what, ref value);

	operator bool() const;

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
};


#if defined main_unit
#define symbol(name) ref name{{}, std::string(#name)}
#else
#define symbol(name) extern ref name;
#endif

namespace sym {
	symbol(nothing);
	symbol(is);
	symbol(what);
};

// holds concept data, rarely used directly
struct con
{
	std::multimap<ref,ref> refs;
	std::any data;
};


#if defined(ref_cpp)
//#include "exception.hpp"
using exception = ref;
#define self (*this)

ref::ref (rs refs, std::any data)
: shared_ptr(new con{{refs.begin(), refs.end()}, data})
{ }

ref::ref (r refs, std::any data)
: ref({refs}, data)
{ }

ref ref::clone()
{
	ref result;
	*result = *self;
	return result;
}

/*
void ref::verify_has(il<il<ref>> refs) const
{
	std::unordered_set<unsigned long> links;
	for (std::vector<ref> assumption : refs) {
		bool found = false;
		ref found_other = sym::nothing;
		if (!assumption.size()) { continue; }
		unsigned long index = 0;
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
					{sym::actually_is, *this}
				};
			}
			if (!found_other) {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]},
					{sym::actually_is, *this}
				};
			} else {
				throw exception{
					{sym::assumed_has_a, assumption[0]},
					{sym::assumed_has_to, assumption[1]},
					{sym::actually_has_to, found_other},
					{sym::actually_is, *this}
				};
			}
		}
	}
}*/

/*
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
*/

ref::operator bool() const
{
	return self != sym::nothing;
}

bool ref::has_data()
{
	return data<std::any>().has_value();
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

ref ref::get(ref what) const
{
	auto result = self->refs.find(what);
	if (result == self->refs.end()) {
		return sym::nothing;
	} else {
		return result->second;
	}
}

// if value is nothing, 'what' is erased.  this seems to make some usage bugs have no effect
void ref::set(ref what, ref value)
{
	auto pos = del(what);
	if (value) {
		self->refs.insert(pos, std::pair<basic_ref,basic_ref>(what, value));
	}
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

std::vector<ref> ref::getall(ref what) const
{
	std::vector<ref> result;
	auto range = self->refs.equal_range(what);
	for (auto iterator = range.first; iterator != range.second; ++ iterator) {
		result.push_back(iterator->second);
	}
	return result;

}

/*
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
*/

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

