#pragma once
#include "ref.hpp"
#include "text.hpp"

namespace sym {
	symbol(habit);
	symbol(inputs);
	symbol(outputs);
	symbol(cxxcode);
}

class cxxhabit : public ref
{
public:
	cxxhabit(il<text> outputs, il<text> inputs, std::function<void(ref)> function)
	: ref({
		{sym::is, sym::habit},
		{sym::inputs, seq(*(il<ref>*)&inputs)},
		{sym::outputs, seq(*(il<ref>*)&outputs)}
	}, function)
	{ }

	/*
	ref operator()(il<ref> inputs_il) {
		seq names = (*this)[sym::inputs];
		auto input_il = inputs_il.begin();
		ref name = names[sym::first];
		for (; name && input_il != inputs_il.end(); name = name[sym::next], ++input_il) {
		}
		// stub
		return sym::nothing;
	}
	*/
};
