#pragma once

#include "ref.hpp"
#include "data.hpp"

namespace sym
{
	synonym(text,::data<std::string>::type());
}

struct text : public data<std::string>
{
	text(std::string data, bool unique = true)
	: data<std::string>(data, unique)
	{ }
	text(ref symb0l)
	: data<std::string>(symb0l)
	{ }

	__attribute__((deprecated))
	std::string & data() { return this->get(); }
	__attribute__((deprecated))
	std::string const & data() const { return this->get(); }
};
