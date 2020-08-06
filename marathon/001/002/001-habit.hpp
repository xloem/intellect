#pragma once
#include "text.hpp"

namespace syms {
	sym(inputs);
	sym(outputs);
	sym(cxxcode);
}

void* anon="no function templates for now, likely takes too long: maybe compare to other struts.  goal: reach habit-combination";
	// note: the time judgement relates
	// to a long term prediction.
	// we need _more_ attempts
class cxxhabit : public ref
{
public:
	cxxhabit(il<text> outputs, il<text> inputs, std::function<void(ref)> function)
	: ref({
		{sym::is, sym::habit},
		{sym::inputs, inputs},
		{sym::outputs, outputs}
	}, function)
	{ }
};
