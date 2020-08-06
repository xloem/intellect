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
		if (!(*this)[sym::first]) {
			*(ref*)this += {
				{sym::first, item},
				{sym::last, item}
			};
		} else {
			ref preceding = (*this)[sym::last];
			preceding[sym::next] = item;
			item[sym::previous] = preceding;
			(*this)[sym::last] = item;
		}
		++ data<unsigned long>();
		return *this;
	}
};
