#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template <typename t>
using il = std::initializer_list<t>;

using ref = std::shared_ptr<concept>;
using cxxcode = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

#define symbol(name) ref name = create({},::text(#name))

namespace symbols
{
	symbol(nothing);

	symbol(what);
	symbol(next);

	symbol(context);
	symbol(outer);

	symbol(state);
	symbol(step);
	symbol(steps);
	symbol(variable);
	symbol(inputs);
	symbol(outputs);

	symbol(hello);
	symbol(world);
	ref space = create({}, text(" "));
	ref endl = create({}, text("\n"));
}

struct concept
{
	std::unordered_multimap<ref,ref> refs;
	std::any _data;

	template <typename t>
	t & data()
	{
		t * result = std::any_cast<t>(&_data);
		if (result == 0) { throw symbols::nothing; }
		return *result;
	}

	void call(ref context)
	{
		data<cxxcode>()(context);
	}

	ref get(ref what)
	{
		auto result = refs.find(what);
		if (result == refs.end()) {
			return symbols::nothing;
		} else {
			return result->second;
		}
	}

	void add(ref what, ref value)
	{
		refs.insert(std::pair(what, value));
	}

	decltype(refs)::iterator wipe(ref what)
	{
		auto range = refs.equal_range(what);
		return refs.erase(range.first, range.second);
	}

	void set(ref what, ref value = symbols::nothing)
	{
		if (!value) { value = what; }
		refs.insert(wipe(what), std::pair(what, value));
	}

	bool isthing()
	{
		return &*symbols::nothing != this;
	}
};

#include "extra.hpp"

// Create a new concept
ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->_data = data;
	result->refs.insert(refs.begin(), refs.end());
	verify(result);
	return result;
}

// Link concepts in a list, by symbols::next ref
ref seq(il<ref> items)
{
	ref first = symbols::nothing;
	ref next;
	for (auto & item : items)
	{
		ref last = next;
		next = item;
		if (!last) {
			first = next;
		} else {
			last->set(symbols::next, next);
		}
	}
	return first;
}

////////////
// HABITS //
////////////

// c++-code habit
ref behavior(il<ref> outputs, il<ref> inputs, cxxcode code)
{
	return create({{symbols::outputs, seq(outputs)}, {symbols::inputs, seq(inputs)}}, code);
}

// make a variable
#define var(name) ref name = create({{symbols::variable,symbols::variable}},::text(#name))

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


//////////
// TEST //
//////////
#include <iostream>
int main()
{
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
