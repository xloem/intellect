#pragma once
#include "ref.hpp"
#include "text.hpp"
#include "var.hpp"

namespace sym {
	symbol(habit);
	symbol(inputs);
	symbol(outputs);
	symbol(input);
	symbol(output);
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

	void assuming_is() const
	{
		ref::assuming_is(); // maybe
		if (get(sym::is) != sym::habit) {
			throw exception({
				{sym::assumed_is, sym::habit},
				{sym::actually_is, get(sym::is)}
			});
		}
	}

	void call_with_ctx(ref context)
	{
		ref::data<std::function<void(ref)>>()(context);
	}

	ref operator()(il<ref> inputs) {
		ref ctx;
		auto input = inputs.begin();
		for (ref name : seq(get(sym::inputs))) {
			if (input == inputs.end()) { break; }
			ctx.set(name, *input);
			++ input;
		}
		call_with_ctx(ctx);
		ref first_output_name = *seq(get(sym::outputs)).begin();
		return ctx[first_output_name];
	}

	ref input(unsigned long index = 0)
	{
		unsigned long current = 0;
		for (auto input : (*this)[sym::inputs].as<seq>()) {
			if (current == index) { return input; }
			++ current;
		}
		return sym::nothing;
	}

	ref output(unsigned long index = 0)
	{
		unsigned long current = 0;
		for (auto output : (*this)[sym::outputs].as<seq>()) {
			if (current == index) { return output; }
			++ current;
		}
		return sym::nothing;
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
	symbol(outer);
}

namespace act
{
	// set up a habit run context
	cxxhabit steps_start = cxxhabit({sym::state}, {sym::steps}, [](ref context)
	{
		seq steps = context[sym::steps].as<seq>();
		context.set(sym::state, ref({
			{sym::context, context},
			{sym::step, steps.begin()}
		}));
	});

	// run a single step in a set-up context
	cxxhabit steps_next = cxxhabit({sym::outputs}, {sym::state}, [](ref outer_context) {
		ref state = outer_context[sym::state];
		ref ctx = state[sym::context];
		iterator step_entry = state[sym::step].as<iterator>();
		ref step = *step_entry;

		seq inputs = step[sym::inputs].as<seq>();
		seq outputs = step[sym::outputs].as<seq>();
		cxxhabit action = step[sym::what].as<cxxhabit>();
		ref subcontext({{sym::outer, ctx}});
		
		seq habit_inputs = action[sym::inputs].as<seq>();
		auto habit_input = habit_inputs.begin();
		for (ref input : inputs) {
			if (habit_input == habit_inputs.end()) { break; }
			ref value = input;
			if (value[sym::variable]) {
				value = ctx[value];
			}
			subcontext.set(*habit_input, value);
			++ habit_input;
		}

		action.call_with_ctx(subcontext);

		seq habit_outputs = action[sym::outputs].as<seq>();
		auto habit_output = habit_outputs.begin();
		for (ref output : outputs) {
			if (habit_output == habit_outputs.end()) { break; }
			ref value = *habit_output;
			if (value[sym::variable]) {
				value = subcontext[value];
			}
			ctx.set(output, value);
			++ habit_output;
		}

		ref next = step_entry[sym::next];
		if (next){
			state.set(sym::step, next);
		} else {
			state.wipe(sym::step);
			outer_context.set(sym::outputs, ctx);
		}
	});

	// run a habit to completion
	cxxhabit steps_run = cxxhabit({}, {sym::steps}, [](ref context) {
		ref steps = context[sym::steps][sym::what];
		ref subcontext({
			{sym::steps, steps}
		});
		steps_start.call_with_ctx(subcontext);
		do {
			steps_next.call_with_ctx(subcontext);
		} while (!subcontext[sym::outputs]);
	});
}

struct stephabit : public cxxhabit
{
	stephabit(il<text> outputs, il<text> inputs, il<step> steps = {})
	: cxxhabit(outputs, inputs, [this](ref ctx){
		if (ctx[sym::steps]) {
			throw exception({
				//{sym::during, sym::stephabit},
				//{sym::within, ctx},
				{sym::assumed_absent, sym::steps},
				{sym::actually_present, sym::steps}
			});
		}
		ctx.set(sym::steps, *this);
		act::steps_run.call_with_ctx(ctx);
	})
	{
		set(sym::what, seq((il<ref>&)steps));
	}
};
