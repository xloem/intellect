#pragma once

// LATER=> literal should be special case, not variable.  used in habit-*.hpp

#include "ref.hpp"
#include "text.hpp"

namespace sym
{
	symbol(variable);
}

struct variable;
template <> il<il<ref>> assumes_has<variable> = {
	{sym::is, sym::variable}
};

struct variable : public text
{
	// TODO: might make more sense to reference unique strings than to be new copies (makes anonytmous variables fit in, prevents pollution of symbols)
	variable(std::string data)
	: text(data, false)
	{
		set(sym::variable);
	}
	variable(ref symb0l)
	: text(symb0l)//symb0l.as<variable>())
	{
		if (!ref::get(sym::variable)) {
			this->set(sym::variable);
		}
	}
	__attribute__((deprecated))
	std::string & data() { return ref::template data<std::string>(); }
	__attribute__((deprecated))
	std::string const & data() const { return ref::template data<std::string>(); }
};
