#pragma once

#include "ref.hpp"
#include "text.hpp"

namespace sym {
	symbol(todo);
	symbol(during);
	symbol(within);
	symbol(assumed_present);
	symbol(assumed_absent);
	symbol(assumed_is);
	symbol(assumed_has_a);
	symbol(assumed_has_to);
	symbol(actually_present);
	symbol(actually_absent);
	symbol(actually_is);
	symbol(actually_has_to);
}

char const * dump(ref what);

#include <stdexcept>
class exception : public ref, public std::runtime_error
{
public:
	// solutions to good dumps:
	//  could add operator- to ref to remove 'what'
	//  could add depth attribute to dumping
	exception(il<std::pair<ref,ref>> refs)
	: ref(refs), runtime_error(dump(as<ref>()))
	{
		ref::data<std::string>(runtime_error::what());
	}

	template <class t>
	exception & operator +=(t const &)
	{
		throw exception({{sym::what, text("atm exception generates its message during construction")}});
		return *this;
	}
};
