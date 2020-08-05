#pragma once

#include "025.hpp"
#include "contain-001.hpp"

////////////
// HABITS //
////////////

// c++-code habit
ref habit(il<ref> outputs, il<ref> inputs, cxxcode code)
{
	return create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}}, code);
}

// create a step in a runtime habit
ref step(il<ref> outputs, il<ref> inputs, ref what)
{
	return create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}, {symbols::what, what}});
}

// c++ habits that run runtime habits

// set up a context to run a habit
ref start_run = habit({symbols::state}, {symbols::steps}, [](ref context){
		// problem exists: steps must override context passed to habit
	context->set(symbols::state, create({
				{symbols::context, context},
				{symbols::step, context->get(symbols::steps)->get(symbols::first)}
	}));
});

// run a single step in set-up context
ref step_run = habit({symbols::outputs}, {symbols::state}, [](ref outer_context) {
	ref state = outer_context->get(symbols::state);
	ref context = state->get(symbols::context);
	ref step_entry = state->get(symbols::step);
	ref step = decapsulate(step_entry);

	ref inputs = step->get(symbols::inputs);
	ref outputs = step->get(symbols::outputs);
	ref action = step->get(symbols::what);
	ref subcontext = create({{symbols::outer, context}});
	for (ref input1 = inputs->get(symbols::first),
	         input2 = action->get(symbols::inputs)->get(symbols::first);
	     input1->isthing() && input2->isthing();
	     input1 = input1->get(symbols::next),
	     input2 = input2->get(symbols::next))
	{
		ref value = decapsulate(input1);
		if (value->get(symbols::variable)->isthing()) {
			value = context->get(value);
		}
		subcontext->set(decapsulate(input2), value);
	}
	action->call(subcontext);
	for (ref output1 = outputs->get(symbols::first),
	         output2 = action->get(symbols::outputs)->get(symbols::first);
	     output2->isthing() && output1->isthing();
	     output1 = output1->get(symbols::next),
	     output2 = output2->get(symbols::next))
	{
		ref value = decapsulate(output2);
		if (value->get(symbols::variable)->isthing()) {
			value = subcontext->get(value);
		}
		context->set(decapsulate(output1), value);
	}

	ref next = step_entry->get(symbols::next);
	state->set(symbols::step, next);
	if (!next->isthing()) {
		outer_context->set(symbols::outputs, context);
	}
});

// run a habit all in one go
ref run = habit({}, {symbols::steps}, [](ref context){
	ref steps = context->get(symbols::steps)->get(symbols::what);
	ref subcontext = create();
	subcontext->set(symbols::steps, steps);
	start_run->call(subcontext);
	do {
		step_run->call(subcontext);
	} while (!subcontext->get(symbols::outputs)->isthing());
});

// create a runtime, scripted habit
ref habit(il<ref> outputs, il<ref> inputs, il<ref> steps)
{
	ref result;
	result = create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}, {symbols::what, seq(steps)}});
	result->_data = (cxxcode)[result](ref context){
		context->set(symbols::steps, result);
		run->call(context);
	};
	return result;
}
