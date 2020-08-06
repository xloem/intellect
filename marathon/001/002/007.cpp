#include "node-001-022.hpp"
#include <iostream>

ref clone(ref old)
{
	ref next = create();
	next->_data = old->_data;
	next->refs = old->refs;
	return next;
}

// changes to 021/022:
// 	- clone function
// 	- push to list
// 	later

ref encapsulate(ref what)
{
	return create({{symbols::what, what}});
}

ref decapsulate(ref what)
{
	return what->get(symbols::what);
}

ref recapsulate(ref what)
{
	return encapsulate(decapsulate(what));
}

ref prepend(ref what, ref sequence)
{
	if (sequence->isthing()) {
		what->set(symbols::next, sequence);
	}
	return what;
}

namespace symbols
{
	sym(options);
	sym(work);
}

text better_to_text(ref what, bool contents = true)
{
	static thread_local std::unordered_set<concept*> found;
	if (found.count(&*what) || !what->refs.size()) { contents = false; }
	text result;
       	if (contents) { result += "["; }
	if (what->_data.has_value()) {
		if (what->_data.type() == typeid(cxxcode)) {
			result += "<cxxcode>";
		} else if (what->_data.type() == typeid(text)) {
			result += what->data<text>();
		} else {
			result += text("<") + what->_data.type().name() + ">";
		}
		if (contents) { result += ":"; }
	}
	ref next = symbols::nothing;
	found.insert(&*what);
	if (contents) {
		for (auto refs : what->refs) {
			if (refs.first == symbols::next) {
				next = refs.second;
				continue;
			}
			result += better_to_text(refs.first, false) + "=" + better_to_text(refs.second);
		}
	}
	if (contents) { result += "]"; }
	if (next->isthing()) {
		result += ",";
		result += better_to_text(next, contents);
	}
	found.erase(&*what);
	return result;
}

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
			work = prepend(recapsulate(option), work);
			std::cout << better_to_text(work) << std::endl;
			states.push_back(create({
				{symbols::options, options},
				{symbols::work, work}
			}));
		}
	}
}
