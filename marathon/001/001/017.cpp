#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template<typename t>
using il = std::initializer_list<t>;

using ref = std::shared_ptr<concept>;
using habit = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

#define symbol(name) ref name = create({},::text(#name))

symbol(nothing);
symbol(context);
symbol(next);

struct concept
{
	std::unordered_map<ref,ref> refs;
	std::any data;

	void go(ref context)
	{
		std::any_cast<habit>(data)(context);
	}

	ref at(ref where)
	{
		auto result = refs.find(where);
		if (result == refs.end()) {
			return nothing;
		} else {
			return result->second;
		}
	}

	void add(ref where, ref what)
	{
		refs.insert(std::pair(where, what));
	}

	void verify()
	{
		for (auto refs : this->refs) {
			if (!refs.first || !refs.second) {
				throw refs;
			}
		}
	}
};

text to_text(ref what, bool contents = true)
{
	static thread_local std::unordered_set<concept*> found;
	if (found.count(&*what) || !what->refs.size()) { contents = false; }
	text result;
       	if (contents) { result += "["; }
	if (what->data.has_value()) {
		if (what->data.type() == typeid(habit)) {
			result += "<habit>";
		} else if (what->data.type() == typeid(text)) {
			result += std::any_cast<text>(what->data);
		} else {
			result += text("<") + what->data.type().name() + ">";
		}
		if (contents) { result += ":"; }
	}
	if (contents) {
		found.insert(&*what);
		for (auto refs : what->refs) {
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
		found.erase(&*what);
	}
	if (contents) { result += "]"; }
	return result;
}

ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->data = data;
	result->refs.insert(refs.begin(), refs.end());
	result->verify();
	return result;
}

ref seq(il<ref> items)
{
	ref first, next;
	for (auto & item : items)
	{
		ref last = next;
		next = item;
		if (!last) {
			first = next;
		} else {
			last->refs.insert(std::pair(::next, next));
		}
	}
	return first;
}

template <typename type>
ref dataseq(il<type> items)
{
	ref first, next;
	for (auto & item : items)
	{
		ref last = next;
		next = create({}, item);
		if (!last) {
			first = next;
		} else {
			last->refs.insert(std::pair(::next, next));
		}
	}
	return first;
}

namespace symbols {
	symbol(steps);
	symbol(text);
	symbol(where);
}

namespace behavior {
	symbol(result);
	symbol(variable);
	symbol(literal);
	symbol(reftype);
	symbol(value);
	symbol(inputs);
	symbol(map);
	symbol(outer);
	ref output = result;

	ref habit(il<ref> inputs, ::habit what) {
		return create({{behavior::inputs, seq(inputs)}}, what);
	}

	ref var(ref which)
	{
		return create({{reftype, variable},{value, which}});
	}
	ref lit(ref which)
	{
		return create({{reftype, literal},{value, which}});
	}

	ref step(il<ref> parameters, ref what)
	{
		return create({{inputs,seq(parameters)},{value, what}});
	}

	ref steps(il<ref> steps)
	{
		return seq(steps);
	}

	ref run = habit({symbols::steps},[](ref context){
		ref steps = context->at(symbols::steps);
		for (ref step = steps; step != nothing; step = step->at(next)) {
			ref inputs = step->at(behavior::inputs);
			ref action = step->at(value);
			ref subcontext = create({{outer, context}});
			for (ref input1 = inputs, input2 = action->at(behavior::inputs); input1 != nothing && input2 != nothing; input1 = input1->at(next), input2 = input2->at(next)) {
				ref type = input1->at(reftype);
				ref value = input1->at(behavior::value);
				if (type == variable) {
					value = context->at(value);
				}
				subcontext->add(input2, value);
			}
			action->go(subcontext);
		}
	});
};

#include <iostream>
int main()
{
	using namespace behavior;

	ref write = behavior::habit({symbols::text}, [](ref context){
		ref text = context->at(symbols::text);
		std::cout << std::any_cast<::text>(text->data);
	});

	symbol(hello);
	ref space = create({}, text(" "));
	symbol(world);
	ref endl = create({}, text("\n"));

	ref steps = behavior::steps({
			step({lit(hello)}, write),
			step({lit(space)}, write),
			step({lit(world)}, write),
			step({lit(endl)}, write)
	});

	behavior::run->go(create({{symbols::steps,steps}}));
}
