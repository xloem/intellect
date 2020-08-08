#pragma once

// LATER=> literal should be special case, not variable.  used in habit-*.hpp

#include "ref.hpp"
#include "text.hpp"

namespace sym
{
	symbol(variable);
}

struct var : public text
{
	var(std::string data)
	: text(data)
	{
		set(sym::variable);
	}
	var(basic_ref symb0l)
	: text(symb0l)
	{
		if (!get(sym::variable)) {
			this->set(sym::variable);
		}
	}
	std::string & data() { return ref::template data<std::string>(); }
	std::string const & data() const { return ref::template data<std::string>(); }
};
