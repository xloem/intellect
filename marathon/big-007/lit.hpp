#pragma once

// LATER=> literal should be special case, not variable.  used in habit-*.hpp

#include "ref.hpp"
#include "text.hpp"

namespace sym
{
	symbol(literal);
}

struct lit : public wrapped
{
	lit(ref what)
	: wrapped(what)
	{
		set(sym::literal);
	}
	using wrapped::get;
};
