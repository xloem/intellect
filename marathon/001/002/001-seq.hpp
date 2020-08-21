#pragma once
#include "node.hpp"
#include "wrapped.hpp"

namespace syms {
	sym(seq);
	sym(next);
	sym(previous);
	sym(first);
	sym(last);
}

class seq : public ref
{
public:
	seq(il<ref> items)
	: ref({{syms::is, syms::seq}}, (unsigned long)0)
	{
		for (auto item : items) {
			*this += item;
		}
	}

	seq & operator+=(wrapped item)
	{
		if (!(*this)[syms::first]) {
			*(ref*)this += {
				{syms::first, item},
				{syms::last, item}
			};
		} else {
			ref preceding = (*this)[syms::last];
			preceding[syms::next] = item;
			item[syms::previous] = preceding;
			(*this)[syms::last] = item;
		}
		++ data<unsigned long>();
		return *this;
	}
};
