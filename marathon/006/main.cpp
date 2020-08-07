#define main_unit
#include "ref.hpp"
#include "seq.hpp"
#include "habit.hpp"
#include "dump.hpp"
#include "gen.hpp"

namespace sym {
	symbol(options);
	symbol(work);
}

gen seq_gen({{sym::context},{sym::seq},[](ref ctx)
	{
		// setup from seq
		seq list = ctx[sym::seq].as<seq>();
		ctx.set(sym::context, ref({{sym::state, list.begin()}}));

	}},{{sym::what},{sym::state},[](ref ctx)
	{
		// get next from seq
		iterator element = ctx[sym::state].as<iterator>();
		ctx.set(sym::what, *element);
		++ element;
		ctx.set(sym::state, element);
	}});

namespace enumeration {
	symbol(states);
	
	cxxhabit start({sym::state},{sym::gen,sym::options},[](ref ctx){
		gen generator = ctx[sym::gen].as<gen>();
		ref options = ctx[sym::options];
		ctx.set(sym::state, ref({
			{sym::gen, generator},
			{sym::options, options},
			{enumeration::states, seq({
				ref({
		//			{sym::state, generator.setup(options)},
					{sym::work, seq({})}
				})
			})}
		}));
	});

	cxxhabit pop_and_append({sym::work},{sym::state},[](ref ctx){
		ref ctxstate = ctx[sym::state];
		seq states = ctxstate[enumeration::states].as<seq>();

		ref state = states.pop_front();
		seq work = state[sym::work].as<seq>();
		//ref options = ctxstate[sym::options]();

		// gens could be iterable
		gen generator = ctxstate[sym::gen].as<gen>();
		ref genstate = generator.start({ctxstate[sym::options]});

		ref option;
		while ( (option = generator.next(genstate)) ) {
			seq next_work = work.clone();
			next_work += option;
			states += ref({
				{sym::work, next_work}
			});
		}

		/*for (ref option : options) {
			seq next_work = work.clone();
			next_work += option;
			states += ref({
				{sym::options, options},
				{sym::work, next_work}
			});
		}*/
		ctx.set(sym::work, work);
	});

}

#include <iostream>
int main()
{
	symbol(one);
	symbol(two);
	symbol(three);
	seq list({one,two,three});

	cxxhabit printlist({},{sym::seq},[](ref context)
	{
		bool first = true;
		for (ref item : seq(context.get(sym::seq))) {
			if (first) {
				first = false;
			} else {
				std::cout << ",";
			}
			std::cout << item.data<std::string>();
		}
		std::cout << std::endl;
	});

	printlist({list});

	ref state = enumeration::start({seq_gen, list});

	//while (state[enumeration::states].as<seq>().size() < 64) {//states.size() < 16) {
	for (unsigned long i = 0; i < 16; ++ i) {
		ref work = enumeration::pop_and_append({state});
		printlist({work});
	}

	std::cout << dump(list) << std::endl;

	cxxhabit write({},{sym::text},[](ref context)
	{
		std::cout << context[sym::text].as<text>().data();
	});

	// default-to-literal inefficient
	cxxhabit read({var(sym::text)},{},[](ref context)
	{
		std::string word;
		std::cin >> word;
		context.set(sym::text, text(word));
	});

	text space(" ");
	text endl("\n");

	seq words({
		space,
		endl,
		text("hello"),
		text("world"),
		text("enter a word"),
		text("you entered:")
	});

	seq actions({
		write,
		read
	});


	symbol(word);
	seq variables({
		var(word)
	});

	// each step is made of an action,
	// and then each input or output is made of either a word or a variable
	
	// the combinations of the above are what we want to pick among
	
	/*
	ref action_enumeration = enumeration::start({actions});

	while (true) {
		ref action = action_enumeration.
	}
	*/

	stephabit helloworld({},{},{
		{{},{text("hello")}, write},
		{{},{space}, write},
		{{},{text("world")}, write},
		{{},{endl}, write}
	});

	stephabit readwrite({},{},{
		{{},{text("enter a word\n")}, write},
		{{word},{}, read},
		{{},{text{"you entered: "}}, write},
		{{},{word}, write},
		{{},{endl}, write}
	});

	helloworld({});
	readwrite({});

}
