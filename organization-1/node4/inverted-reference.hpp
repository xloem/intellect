#pragma once

#include "reference.hpp"

DECLARE inverted-reference inverse;
DECLARE reference self;

class inverted-reference : public reference
{
public:
	inverted-reference(reference inverse)
	{
		if (self() == inverse) {
			kind-set(::inverse(), *this);
		} else {
			inverse.kind-set(::inverse(), *this);
			kind-set(::inverse(), inverse);
		}
	};
};
