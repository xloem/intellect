#pragma once

// LATER=> literal should be special case, not variable.  used in habit-*.hpp

#include "ref.hpp"
#include "text.hpp"

namespace sym
{
	symbol(variable);
}

struct variable : public text
{
	variable(std::string data)
	: text(data, false)
	{
		set(sym::variable);
	}
	variable(ref symb0l)
	: text(symb0l)
	{
		if (!get(sym::variable)) {
			this->set(sym::variable);
		}
	}
	std::string & data() { return ref::template data<std::string>(); }
	std::string const & data() const { return ref::template data<std::string>(); }
};
