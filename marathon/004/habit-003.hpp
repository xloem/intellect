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

	ref operator()(il<ref> inputs) {
		ref context;
		auto input = inputs.begin();
		for (ref name : seq(get(sym::inputs))) {
			if (input == inputs.end()) { break; }
			context.set(name, *input);
			++ input;
		}
		ref::data<std::function<void(ref)>>()(context);
		return *seq(get(sym::outputs)).begin();
	}
};
