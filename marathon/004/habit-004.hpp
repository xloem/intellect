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

	void call_with_ctx(ref context)
	{
		ref::data<std::function<void(ref)>>()(context);
	}

	ref operator()(il<ref> inputs) {
		ref context;
		auto input = inputs.begin();
		for (ref name : seq(get(sym::inputs))) {
			if (input == inputs.end()) { break; }
			context.set(name, *input);
			++ input;
		}
		call_with_ctx(context);
		return *seq(get(sym::outputs)).begin();
	}
};

struct step : public ref
{
	step(il<ref> outputs, il<ref> inputs, ref what)
	: ref({
		{sym::outputs, seq(outputs)},
		{sym::inputs, seq(inputs)},
		{sym::what, seq(what)}
	})
	{ }
};

namespace sym
{
	symbol(state);
	symbol(context);
	symbol(step);
	symbol(steps);
}

namespace act
{
	// set up a habit run context
	ref steps_start = cxxhabit({sym::state}, {sym::steps}, [](ref context)
	{
		seq steps = context[sym::steps][sym::what].as<seq>();
		context.set(sym::state, ref({
			{sym::context, context},
			{sym::step, steps.begin()}
		}));
	});

	// run a single step in a set-up context
	/*
	ref steps_next = cxxhabit({sym::outputs}, {sym::state}, [](ref outer_context) {
		ref state = outer_context[sym::state];
		ref ctx = state[sym::context];
		iterator step_entry = state[sym::step].as<iterator>();
		ref step = *step_entry;

		seq inputs = step[sym::inputs].as<seq>();
		seq outputs = step[sym::outputs].as<seq>();
		cxxhabit action = step[sym::what].as<cxxhabit>();
		ref subcontext({{sym::outer, context}});
		
		seq habit_inputs = action[sym::inputs].as<seq>();
		auto habit_input = habit_inputs.begin();
		for (ref input : inputs) {
			if (habit_input == habit_inputs.end()) { break; }
			ref value = input
			if (value[sym::variable]) {
				value = context[value];
			}
			subcontext.set(*habit_input, value);
			++ habit_input;
		}

		action.call_with_context(subcontext);

		ref habit_outputs = action[sym::outputs].as<seq>();
		ref habit_output = habit_outputs.begin();
		for (ref output : outputs) {
			if (habit_output == habit_outputs.end()) { break; }
			ref value = *habit_output;
			if (value[sym::variable]) {
				value = subcontext[value];
			}
			context.set(output, value);
			++ habit_output;
		}

		ref next = step_entry[sym::next];
	});
	*/
}

struct stephabit : public cxxhabit
{
	stephabit(il<text> outputs, il<text> inputs, il<step> steps = {})
	: cxxhabit(outputs, inputs, [](ref context){/*STUB*/})
	{
		set(sym::what, seq((il<ref>&)steps));
	}
};
