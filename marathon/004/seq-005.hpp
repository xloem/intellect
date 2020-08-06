#pragma once
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

class seq : public ref
{
public:
	seq(il<ref> items)
	: ref({{sym::is, sym::seq}}, (unsigned long)0)
	{
		for (auto item : items) {
			*this += item;
		}
	}

	seq & operator+=(wrapped item)
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
		return *this;
	}

	iterator begin()
	{
		return this->get(sym::first);
	}

	iterator end()
	{
		return iterator(sym::nothing);
	}

};
