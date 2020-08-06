#pragma once

struct text : public ref
{
	text(std::string data)
	: ref({}, data)
	{ }
	std::string & data() { return ref::template data<std::string>(); }
};
