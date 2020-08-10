#pragma once
#include "ref.hpp"
#include "text.hpp"
#include "var.hpp"
#include "seq.hpp"

#define self (*this)

namespace sym {
	symbol(habit);
	symbol(inputs);
	symbol(outputs);
	symbol(input);
	symbol(output);
	symbol(cxxcode);
}

class cxxhabit;

template <> il<il<ref>> assumes_has<cxxhabit> = {
	{sym::is, sym::habit},
	{sym::inputs},
	{sym::outputs}
};

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

	std::function<void(ref)> const & data()
	{
		return ref::data<std::function<void(ref)>>();
	}

	void call_with_ctx(ref context)
	{
		return data()(context);
	}

	ref ctx_from_inputs(il<ref> inputs) const
	{
		ref ctx;
		auto input = inputs.begin();
		for (ref name : self[sym::inputs].as<seq>()) {
			if (input == inputs.end()) { break; }
			ctx <= r{name, *input};
			++ input;
		}
		return ctx;
	}

	ref call_making_ctx(il<ref> inputs) {
		ref ctx = ctx_from_inputs(inputs);
		call_with_ctx(ctx);
		return ctx;
	}

	ref operator()(il<ref> inputs) {
		ref ctx = call_making_ctx(inputs);
		ref first_output_name = *self[sym::outputs].as<seq>().begin();
		return ctx[first_output_name];
	}

	ref input(unsigned long index = 0)
	{
		unsigned long current = 0;
		for (auto input : self[sym::inputs].as<seq>()) {
			if (current == index) { return input; }
			++ current;
		}
		return sym::nothing;
	}

	ref output(unsigned long index = 0)
	{
		unsigned long current = 0;
		for (auto output : self[sym::outputs].as<seq>()) {
			if (current == index) { return output; }
			++ current;
		}
		return sym::nothing;
	}
};

// BUG noted, bumped into: constructors don't verify, so typecasts don't verify
// closest solution is calling as<> in copy constructor

struct step : public ref
{
	step(il<ref> outputs, il<ref> inputs, cxxhabit what)
	: ref({
		{sym::outputs, seq(outputs)},
		{sym::inputs, seq(inputs)},
		{sym::what, what}
	})
	{ }
	step(ref label, il<ref> outputs, il<ref> inputs, cxxhabit what)
	: ref({
		{sym::text, label},
		{sym::outputs, seq(outputs)},
		{sym::inputs, seq(inputs)},
		{sym::what, what}
	})
	{ }
	step(seq outputs_then_inputs, cxxhabit what)
	{
		seq outputs({});
		seq inputs({});
		iterator output_iterator = what[sym::outputs].as<seq>().begin();
		for (ref parameter : outputs_then_inputs) {
			if (output_iterator) {
				outputs += parameter;
				++ output_iterator;
			} else {
				inputs += parameter;
			}
		}
		self <= rs{
			{sym::outputs, outputs},
			{sym::inputs, inputs},
			{sym::what, what}
		};
	}
	step(ref label, seq outputs_then_inputs, cxxhabit what)
	: step(outputs_then_inputs, what)
	{
		if (label) {
			set(sym::text, label);
		}
	}
};

namespace sym
{
	symbol(state);
	symbol(context);
	symbol(step);
	symbol(steps);
	symbol(outer);
	symbol(step_context);
}

// how to loop in stephabits
// only matters at compiletime, where things are made with constructors.
// one way is to add a link to the habit entry, to the step.
// then you can walk that link to loop to it, after predeclaring it.
// 	another way is to label each step, and go to the label, which sounds better.
// 	but how do you go to the label?  [you'd need to mutate the label into the
// 	step .. in the way you go-to.  you could have variables be lines.]
// 		=S  is fine for now for variables to be lines

namespace act
{
	// set up a habit run context
	cxxhabit steps_start = cxxhabit({sym::state}, {sym::steps, sym::context}, [](ref context)
	{
		ref habit = context[sym::steps];
		seq steps = habit[sym::what].as<seq>();
		ref contextee = context[sym::context];
		context.set(sym::state, ref({
			{sym::habit, habit},
			{sym::context, contextee},
			{sym::step, steps.begin()}
		}));
		contextee.set(sym::step_context, context);
	});

	// run a single step in a set-up context
	cxxhabit steps_next = cxxhabit({sym::outputs, sym::step, sym::next}, {sym::state}, [](ref outer_ctx) {
		ref state = outer_ctx[sym::state];
		ref ctx = state[sym::context];
		iterator<ref> step_entry = state[sym::step].as<iterator<ref>>();
		outer_ctx.set(sym::step, step_entry);
		outer_ctx.set(sym::next, step_entry[sym::next]);
		ref step = *step_entry;

		seq inputs = step[sym::inputs].as<seq>();
		seq outputs = step[sym::outputs].as<seq>();
		cxxhabit action = step[sym::what].as<cxxhabit>();
		ref subcontext({{sym::outer, ctx}});
		
		// provide inputs
		seq habit_inputs = action[sym::inputs].as<seq>();
		auto habit_input = habit_inputs.begin();
		for (ref input : inputs) {
			if (habit_input == habit_inputs.end()) { break; }
			ref value = input;
			if (value[sym::variable]) {
				value = ctx[value];
			}
			subcontext <= r{*habit_input, value};
			++ habit_input;
		}

		// call action
		action.call_with_ctx(subcontext);

		// extract outputs
		seq habit_outputs = action[sym::outputs].as<seq>();
		auto habit_output = habit_outputs.begin();
		for (ref output : outputs) {
			if (habit_output == habit_outputs.end()) { break; }
			ref value = *habit_output;
			if (value[sym::variable]) {
				value = subcontext[value];
			}
			ctx <= r{output, value};
			++ habit_output;
		}

		// advance to next step.  a special symbol in the context could do this.
		// in general, this would be a stepshabit, and have a context the other
		// could reference.  and it does.  step_entry is in outer_ctx.
		// 	we'll want to copy it into outer_ctx or such.
		// 		how is outer_ctx related to the stepee's context?
		// 		note: outer_ctx is the same context shared with start
		// 		it contains a reference to the stepee's context,
		// 		but not vice versa yet
		ref next = outer_ctx[sym::next];
		if (next){
			state.set(sym::step, next);
		} else {
			state.wipe(sym::step);
			outer_ctx <= r{sym::outputs, ctx};
		}
	});

	// run a habit to completion
	cxxhabit steps_run = cxxhabit({}, {sym::steps}, [](ref context) {
		ref steps = context[sym::steps];
		ref subcontext({
			{sym::steps, steps},
			{sym::context, context},
		});
		steps_start.call_with_ctx(subcontext);
		do {
			steps_next.call_with_ctx(subcontext);
		} while (!subcontext[sym::outputs]);
	});
}


class stephabit;
template <> il<il<ref>> assumes_has<stephabit> = {
	{sym::is, sym::habit},
	{sym::what}
};

struct stephabit : public cxxhabit
{
	stephabit(il<text> outputs, il<text> inputs, il<step> steps = {})
	: stephabit(outputs, inputs, seq((il<ref>&)steps))
	{ }
	stephabit(il<text> outputs, il<text> inputs, seq steps = {{}})
	: cxxhabit(outputs, inputs, [this](ref ctx){
		if (ctx[sym::steps]) {
			// a better solution here is to pass ctx to steps_run inside another
			// object.
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
		set(sym::what, steps);

		// below code mutates step labels to the steps they label,
		// in inputs to function calls.
		ref labels;
		for (auto step = steps.begin(); step != steps.end(); ++ step) {
			auto label = (*step)[sym::text];
			if (label) {
				labels.set(label, step);
			}
		}
		for (auto step : steps) {
			seq inputs = step[sym::inputs].as<seq>();
			for (auto input = inputs.begin(); input != inputs.end(); ++ input) {
				auto destination = labels[*input];
				if (destination) {
					input.set(sym::what, destination);
				}
			}
		}
	}
};
