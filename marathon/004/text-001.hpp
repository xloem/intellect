#pragma once

struct text : public ref
{
	text(std::string data)
	: ref({}, data)
	{ }
	text(basic_ref symb0l)
	: ref(symb0l)
	{ }
	std::string & data() { return ref::template data<std::string>(); }
};
