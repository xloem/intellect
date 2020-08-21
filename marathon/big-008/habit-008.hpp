#pragma once
#include "ref.hpp"
#include "text.hpp"
#include "var.hpp"
#include "seq.hpp"
#include "exception.hpp"

namespace sym {
	symbol(habit);
	symbol(inputs);
	symbol(outputs);
	symbol(input);
	symbol(output);
	symbol(cxxcode);
	symbol(self);

	symbol(state);
	symbol(context);
	symbol(step);
	symbol(steps);
	symbol(outer);
	symbol(from);
	symbol(to);
	symbol(step_context);
}
ref sym_self = sym::self;

#define self (*this)

class cxxhabit;

template <> il<il<ref>> assumes_has<cxxhabit> = {
	{sym::is, sym::habit},
	{sym::inputs},
	{sym::outputs}
};

#include <functional>
class cxxhabit : public ref
{
public:
	using cxxctxfunction = std::function<void(ref)>;

	cxxhabit(il<text> outputs, il<text> inputs, cxxctxfunction function)
	: ref({
		{sym::is, sym::habit},
		{sym::inputs, seq(*(il<ref>*)&inputs)},
		{sym::outputs, seq(*(il<ref>*)&outputs)},
		{sym::habit, ref({},function)}
	})
	{ }

	cxxhabit(std::string name, il<text> outputs, il<text> inputs, cxxctxfunction function)
	: ref({
		{sym::is, sym::habit},
		{sym::inputs, seq(*(il<ref>*)&inputs)},
		{sym::outputs, seq(*(il<ref>*)&outputs)},
		{sym::habit, ref({},function)}
	}, name.size() ? std::any(name) : std::any())
	{ }

	template <class outputs_t, class... input_ts>
	cxxhabit(std::string name, il<text> outputs, outputs_t(*function)(input_ts...), il<text> inputs)
	: ref({
		{sym::is, sym::habit},
		{sym::inputs, seq(*(il<ref>*)&inputs)},
		{sym::outputs, seq(*(il<ref>*)&outputs)},
		{sym::habit, ref({},static_cast<outputs_t(*)(decltype(((input_ts*)0)->template as<ref>())...)>(function))}
	}, name)
	{ }

	cxxctxfunction const & ctxdata()
	{
		return get(sym::habit).data<cxxctxfunction>();
	}

	bool cxx_takes_context() const
	{
		auto & data = get(sym::habit).data<std::any>();
		return data.type() == typeid(cxxctxfunction);
	}

	static ref where_from(ref what)
	{
		// TODO BUG: incorrect if multithreaded.  could use thread id from ref.
		return what.back(sym::from);
	}

	// making these inline may ease the gdb stack strace
	inline void call_with_ctx(ref context)
	{
		context.set(sym_self, self);

#if flow_enabled // should be runtime
		static thread_local ref last_call = sym::nothing;

		seq inputs({});
		seq outputs({});

		ref outer_ctx = context[sym::outer];
		if (outer_ctx) { outer_ctx = outer_ctx.clone(); }
		ref call{{
			{sym::previous, last_call},
			{sym::habit, *this},
			{sym::inputs, inputs},
			{sym::outputs, outputs},
			{sym::outer, outer_ctx},
		}};
		for (ref inputname : self[sym::inputs].as<seq>()) {
			// TODO BUG: if values are mutated, this won't be correct; deep cloning them (before passing to function) would fix
			ref input = context[inputname];
			ref from = where_from(input);
			inputs.push_back(ref({
				{sym::what, input},
				{sym::from, from}
			}));
			from.add(sym::to, call);
		}
		if (last_call) {
			last_call.set(sym::next, call);
		}
		last_call = call;
#endif

		ctxdata()(context);
#if flow_enabled
		for (ref outputname : self[sym::outputs].as<seq>()) {
			ref output = context[outputname];
			output.add(sym::from, call);
			outputs.push_back(output);
		}
#endif
	}

	inline ref ctx_from_inputs(il<ref> inputs) const
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

	inline ref call_making_ctx(il<ref> inputs)
	{
		ref ctx = ctx_from_inputs(inputs);
		call_with_ctx(ctx);
		return ctx;
	}

	inline ref operator()(il<ref> inputs)
	{
		ref ctx = call_making_ctx(inputs);
		ref first_output_name = *self[sym::outputs].as<seq>().begin();
		return ctx[first_output_name];
	}

	template <class... t>
	inline ref operator()(t... inputs)
	{
		return (*this)(il<ref>{inputs...});
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
	void steps_start_func(ref context);
	cxxhabit steps_start = cxxhabit({sym::state}, {sym::steps, sym::context}, steps_start_func);
	void steps_start_func(ref context)
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
	}

	// run a single step in a set-up context
	void steps_next_func(ref context);
	cxxhabit steps_next = cxxhabit({sym::outputs, sym::step, sym::next}, {sym::state}, steps_next_func);
	void steps_next_func(ref outer_ctx)
	{
		ref state = outer_ctx[sym::state];
		ref ctx = state[sym::context];
		if (!state.get(sym::step)) {
			outer_ctx <= r{sym::outputs, ctx};
			return;
		}
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
		if (habit_inputs.size() != inputs.size()) {
			// might be too much checking, unsure
			throw exception({
				{sym::assumed_has_a, sym::inputs},
				{sym::assumed_has_to, habit_inputs},
				{sym::actually_has_to, inputs}
			});
		}
		for (ref input : inputs) {
			if (habit_input == habit_inputs.end()) {
				break;
			}
			ref value = input;
			if (value[sym::variable]) {
				value = ctx[value];
			}
			subcontext <= r{*habit_input, value};
			++ habit_input;
		}
		/*
		ref call{{
			{sym::state, state},
			{sym::inputs, subcontext.clone()},
			{sym::step, step_entry}
		}};
		*/

		// call action
		action.call_with_ctx(subcontext);

		// extract outputs
		seq habit_outputs = action[sym::outputs].as<seq>();
		auto habit_output = habit_outputs.begin();
		if (habit_outputs.size() != outputs.size()) {
			// might be too much checking, unsure, but these are all that are copied out
			throw exception({
				{sym::assumed_has_a, sym::inputs},
				{sym::assumed_has_to, habit_inputs},
				{sym::actually_has_to, inputs}
			});
		}
		for (ref output : outputs) {
			if (habit_output == habit_outputs.end()) {
				break;
			}
			ref value = *habit_output;
			if (value[sym::variable]) {
				value = subcontext[value];
			}
			// element-in-context
			ctx <= r{output, value};
			/*
			value.add(sym::from, //ref representing calling this function from this step with its inputs
				rs{
					{sym::habit, call},
					{sym::output, habit_output}
				}
			);
			*/
			++ habit_output;
		}
		//call.set(sym::outputs, subcontext);
			// this tagged output values with sym::from, how they were made

		// advance to next step.  a special symbol in the context could do this.
		// in general, this would be a stepshabit, and have a context the other
		// could reference.  and it does.  step_entry is in outer_ctx.
		// 	we'll want to copy it into outer_ctx or such.
		// 		how is outer_ctx related to the stepee's context?
		// 		note: outer_ctx is the same context shared with start
		// 		it contains a reference to the stepee's context,
		// 		but not vice versa yet
		ref next = outer_ctx[sym::next];
		state.set(sym::step, next);
	}

	// run a habit to completion
	void steps_run_func(ref context);
	cxxhabit steps_run = cxxhabit({}, {sym::steps}, steps_run_func);
	void steps_run_func(ref context)
	{
		ref steps = context[sym::steps];
		ref subcontext({
			{sym::steps, steps},
			{sym::context, context},
		});
		steps_start.call_with_ctx(subcontext);
		while (!subcontext[sym::outputs]) {
			steps_next.call_with_ctx(subcontext);
		}
	}
}


class stephabit;
template <> il<il<ref>> assumes_has<stephabit> = {
	{sym::is, sym::habit},
	{sym::what}
};

class stephabit : public cxxhabit
{
public:
	stephabit(il<text> outputs, il<text> inputs, il<step> steps = {})
	: stephabit({}, outputs, inputs, seq((il<ref>&)steps))
	{ }
	stephabit(il<text> outputs, il<text> inputs, seq steps = {{}})
	: stephabit({}, outputs, inputs, steps)
	{ }
	stephabit(std::string name, il<text> outputs, il<text> inputs, il<step> steps = {})
	: stephabit(name, outputs, inputs, seq((il<ref>&)steps))
	{ }
	stephabit(std::string name, il<text> outputs, il<text> inputs, seq steps = {{}})
	: cxxhabit(name, outputs, inputs, stephabit::run)
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

private:
	static void run(ref ctx)
	{
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
		ctx.set(sym::steps, ctx[sym_self]);
		act::steps_run.call_with_ctx(ctx);
	}
};

#undef self
