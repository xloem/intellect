#pragma once

#include "025.hpp"
#include "contain-002.hpp"

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
ref habit(il<ref> outputs, il<ref> inputs, il<ref> steps = {})
{
	ref result;
	result = create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}, {symbols::what, seq(steps)}});
	result->_data = (cxxcode)[result](ref context){
		context->set(symbols::steps, result);
		run->call(context);
	};
	return result;
}

//"this call shows solution to function templates problem<==="
//"it is only similar: is the other end"
//"it shows container solution: can pass il<ref> parameters or equivalent to function?"
//"we could pass a std::map =S.  but context object already works as std::map =)"
//	"update function templates: with [] operator context is easy to use?"
//		"not-needed you say.  still.  hmm.  any information on time-investment-meaning?"
//			"we already have working code; time investment should be small and scattered, not big chunks that continue.  ==>function tempaltes had one big chunk of time investment inside another, showing a habit of distraction was growing<==
//				"process depth: ==> nested large investment builds distraction <=="
//					may have just triggered that here, new
//						some of this is higher priority because
//						it is investment in how to invest well,
//		there was other indication that habit was growing, too.
//	thank you for wonderful update.  unfortunately may need it again, later.
ref call(ref habit, il<ref> parameters)
{
	ref context = create();
	ref inputs = habit->get(symbols::inputs);
	auto parameter = parameters.begin();
	for (ref input = inputs->get(symbols::first); input->isthing() && parameter != parameters.end(); input = input->get(symbols::next), ++ parameter) {
		context->set(decapsulate(input), *parameter);
	}
	habit->call(context);
	ref outputs = habit->get(symbols::outputs);
	ref output = outputs->get(symbols::first);
	return output->isthing() ? context->get(decapsulate(output)) : symbols::nothing;
}
