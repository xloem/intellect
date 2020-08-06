#include "include/024.hpp"

////////////
// HABITS //
////////////

// c++-code habit
ref behavior(il<ref> outputs, il<ref> inputs, cxxcode code)
{
	return create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}}, code);
}

// create a step in a runtime habit
ref step(il<ref> outputs, il<ref> inputs, ref what)
{
	return create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)},{symbols::what, what}});
}

// c++ habits that run runtime habits
 
// set up a context to run a habit
ref start_run = behavior({symbols::state}, {symbols::steps}, [](ref context){
		// problem exists: steps must override context passed to
		// habit.
	context->set(symbols::state, create({
				{symbols::context, context},
				{symbols::step, context->get(symbols::steps)}
	}));
});

ref step_run = behavior({symbols::outputs}, {symbols::state}, [](ref outer_context){
	ref state = outer_context->get(symbols::state);
	ref context = state->get(symbols::context);
	ref step = state->get(symbols::step);

	ref inputs = step->get(symbols::inputs);
	ref outputs = step->get(symbols::outputs);
	ref action = step->get(symbols::what);
	ref subcontext = create({{symbols::outer, context}});
	for (ref input1 = inputs, input2 = action->get(symbols::inputs); input1->isthing() && input2->isthing(); input1 = input1->get(symbols::next), input2 = input2->get(symbols::next)) {
		ref value = input1;
		if (value->get(symbols::variable)->isthing()) {
			value = context->get(value);
		}
		subcontext->set(input2, value);
	}
	action->call(subcontext);
	for (ref output1 = outputs, output2 = action->get(symbols::outputs); output2->isthing() && output1->isthing(); output1 = output1->get(symbols::next), output2 = output2->get(symbols::next)) {
		ref value = output2;
		if (value->get(symbols::variable)->isthing()) {
			value = subcontext->get(value);
		}
		context->set(output1, value);
	}

	ref next = step->get(symbols::next);
	state->set(symbols::step, next);
	if (!next->isthing()) {
		outer_context->set(symbols::outputs, context);
		return;
	}
});

// run a habit all in one go
ref run = behavior({}, {symbols::steps}, [](ref context){
	ref steps = context->get(symbols::steps)->get(symbols::what);
	ref subcontext = create();
	subcontext->set(symbols::steps, steps);
	start_run->call(subcontext);
	do {
		step_run->call(subcontext);
	} while (!subcontext->get(symbols::outputs)->isthing());
});

// create a runtime, scripted habit
ref behavior(il<ref> outputs, il<ref> inputs, il<ref> steps)
{
	ref result;
	result = create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}, {symbols::what, seq(steps)}});
	result->_data = (cxxcode)[result](ref context){
		context->set(symbols::steps, result);
		run->call(context);
	};
	return result;
}

//////////
// construct chains of behavior
//////////

namespace symbols {
	sym(options);
	sym(work);
}


//////////
// TEST //
//////////
#include <iostream>
#include <list>

int main()
{
	sym(one);
	sym(two);
	sym(three);

	sym(state_remaining);
	sym(sofar);

	ref syms = seq({encapsulate(one), encapsulate(two), encapsulate(three)});

	std::list<ref> states;
	states.push_back(create({
		{symbols::options, syms},
		{symbols::work, seq({})}
	}));

	while (states.size() < 16) {
		ref state = states.front();
		states.pop_front();
		ref work = state->get(symbols::work);
		ref options = state->get(symbols::options);
		for (ref option = options; option->isthing(); option = option->get(symbols::next)) {
			work = seq_append(work, recapsulate(option));
			std::cout << to_text(work) << std::endl;
			states.push_back(create({
				{symbols::options, options},
				{symbols::work, work}
			}));
		}
	}


	var(text);
	ref write = behavior({}, {text}, [text](ref context){
		ref textval = context->get(text);
		std::cout << textval->data<::text>();
	});
	ref read = behavior({text}, {}, [text](ref context){
		::text result;
		std::cin >> result;
		context->set(text, create({}, result));
	});

	var(word);

	ref steps = behavior({}, {},{
		step({}, {symbols::hello}, write),
		step({}, {symbols::space}, write),
		step({}, {symbols::world}, write),
		step({}, {symbols::endl}, write),
		step({}, {create({}, ::text("Enter a word: \n"))}, write),
		step({word}, {}, read),
		step({}, {create({}, ::text("You entered: "))}, write),
		step({}, {word}, write),
		step({}, {symbols::endl}, write)
	});

	to_text(steps);
	steps->call(create());
}
