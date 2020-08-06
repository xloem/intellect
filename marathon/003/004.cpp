#include "include/025.hpp"
#include "include/habits-001.hpp"
#include "include/extra.hpp"

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

	ref syms = seq({one, two, three});

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
		for (ref option = options->get(symbols::first); option->isthing(); option = option->get(symbols::next)) {
			// note: cloning only clones the top bit !
			ref next_work = seq_append(seq_clone(work), decapsulate(option));
			std::cout << seq_to_detail_text(next_work) << std::endl;
			states.push_back(create({
				{symbols::options, options},
				{symbols::work, next_work}
			}));
		}
	}

	var(text);
	ref write = habit({}, {text}, [text](ref context){
		ref textval = context->get(text);
		std::cout << textval->data<::text>();
	});
	ref read = habit({text}, {}, [text](ref context){
		::text result;
		std::cin >> result;
		context->set(text, create({}, result));
	});

	var(word);

	ref steps = habit({}, {},{
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

	std::cout << to_text(steps) << std::endl;
	steps->call(create());
}
