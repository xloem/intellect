#pragma once

namespace sym
{
	symbol(text);
}

struct text : public ref
{
	text(std::string data)
	: ref({{sym::is,sym::text}}, data)
	{ }
	text(basic_ref symb0l)
	: ref(symb0l)
	{
		this->set(sym::is,sym::text);
	}
	std::string & data() { return ref::template data<std::string>(); }
};
