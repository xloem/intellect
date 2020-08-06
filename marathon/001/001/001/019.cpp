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
	std::unordered_map<ref,ref> refs;
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

	void set(ref what, ref value = symbols::nothing)
	{
		if (!value) { value = what; }
		refs.insert(std::pair(what, value));
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

// a c++ habit that runs a runtime habit
ref run = behavior({}, {symbols::steps}, [](ref context){
	ref steps = context->get(symbols::steps)->get(symbols::what);
	for (ref step = steps; step != symbols::nothing; step = step->get(symbols::next)) {
		ref inputs = step->get(symbols::inputs);
		ref outputs = step->get(symbols::inputs);
		ref action = step->get(symbols::what);
		ref subcontext = create({{symbols::outer, context}});
		for (ref input1 = inputs, input2 = action->get(symbols::inputs); input1->isthing() && input2->isthing(); input1 = input1->get(symbols::next), input2 = input2->get(symbols::next)) {
			ref value = input1;
			if (value->get(symbols::variable)->isthing()) {
				value = context->get(value);
			}
			subcontext->set(input2, value);
		}
		// action has no function associated here
		action->call(subcontext);
		for (ref output1 = outputs, output2 = action->get(symbols::outputs); output2->isthing() && output1->isthing(); output1 = output1->get(symbols::next), output2 = output2->get(symbols::next)) {
			ref value = output2;
			if (value->get(symbols::variable)->isthing()) {
				value = subcontext->get(value);
			}
			context->set(output1, value);
		}
	}
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

	ref steps = behavior({}, {},{
		step({}, {symbols::hello}, write),
		step({}, {symbols::space}, write),
		step({}, {symbols::world}, write),
		step({}, {symbols::endl}, write)
	});

	to_text(steps);
	steps->call(create());
}
