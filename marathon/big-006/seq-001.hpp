// no-more-invest-code-kinds-until-used-for-stuff,
// need-to-normalize-these-wonderful-investments

#pragma once
#include "exception.hpp"
#include "ref.hpp"
#include "wrapped.hpp"

namespace sym {
	symbol(seq);
	symbol(next);
	symbol(previous);
	symbol(first);
	symbol(last);
}

class iterator : public ref {
public:
	using ref::ref;

	iterator & operator++()
	{
		return *this = ref::get(sym::next);
	}

	ref operator*()
	{
		return get(sym::what);
	}
};

class seq; template <>
il<il<ref>> assumes_has<seq> = {
	{sym::is, sym::seq}
};

class seq : public ref
{
public:
	seq(ref other)
	: ref(other)
	{ }

	seq(il<ref> items)
	: ref({{sym::is, sym::seq}}, (unsigned long)0)
	{
		for (auto item : items) {
			*this += item;
		}
	}

	seq clone()
	{
		seq result({});
		for (auto item : *this) {
			result += item;
		}
		return result;
	}

	seq & operator+=(wrapped item)
	{
		push_back(item);
		return *this;
	}

	void push_front(/*cut-short-in-middle error*/ref item)
	{
		if (!this->get(sym::first)) {
			*(ref*)this += {
				{sym::first, item},
				{sym::last, item}
			};
		} else {
			ref following = this->get(sym::first);
			following.set(sym::previous, item);
			item.ref::set(sym::next, following);
			this->set(sym::first, item);
		}
		++ data<unsigned long>();
	}

	void push_back(/*cut-short-in-middle error*/ref item)
	{
		if (!this->get(sym::first)) {
			*(ref*)this += {
				{sym::first, item},
				{sym::last, item}
			};
		} else {
			ref preceding = this->get(sym::last);
			preceding.set(sym::next, item);
			item.ref::set(sym::previous, preceding);
			this->set(sym::last, item);
		}
		++ data<unsigned long>();
	}

	// we asked for some guidance in the 'pop/shift' journey
	// but we still ran into issues.  we may need others to learn
	// for us when our learning is at capacity, or to take more
	// time to consider that it could be harder than it seems.
	ref pop_front()
	{
		if (!(*this)[sym::first]) {
			throw exception({
				{sym::assumed_present, sym::first},
				{sym::actually_absent, sym::first}
			});
		}
		wrapped result = (*this)[sym::first].as<wrapped>();
		wrapped following = result[sym::next].as<wrapped>();
		if (!following) {
			(*this).wipe(sym::first);
			(*this).wipe(sym::last);
		} else {
			(*this).set(sym::first, following);
		}
		result.wipe(sym::next);
		following.wipe(sym::previous);
		// returned wrong of two values, fixed later
		// many other small pattern-inversions made
		// errors in these two functions
		return result.get();
	}

	ref pop_back()
	{
		if (!(*this)[sym::last]) {
			throw exception({
				{sym::assumed_present, sym::last},
				{sym::actually_absent, sym::last}
			});
		}
		wrapped result = (*this)[sym::last].as<wrapped>();
		wrapped preceding = result[sym::previous].as<wrapped>();
		if (!preceding) {
			(*this).wipe(sym::last);
			(*this).wipe(sym::first);
		} else {
			(*this).set(sym::last, preceding);
		}
		result.wipe(sym::previous);
		preceding.wipe(sym::next);
		return result.get();
	}

	unsigned long size() const
	{
		return data<unsigned long>();
	}

	iterator begin() const // this is const because iterator returns a copy
	{
		return this->get(sym::first);
	}

	iterator end() const // const because iterator returns a copy
	{
		return iterator(sym::nothing);
	}
};
