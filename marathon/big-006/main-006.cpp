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

gen seq_gen({{sym::state},{sym::seq},[](ref ctx)
	{
		// setup from seq
		seq list = ctx[sym::seq].as<seq>();
		ctx <= r{sym::state, list.begin()};

	}},{{sym::what},{sym::state},[](ref ctx)
	{
		// get next from seq
		iterator<ref> element = ctx[sym::state].as<iterator<ref>>();

		ctx.set(sym::what, *element);

		++ element;

		ctx.set(sym::state, element);
	}});

// might be sensical to have gen setup list its outputs and
// preserve those, passing all to next context

// generates sequences of items from the provided sequence of gen_uses
gen finite_combinations_gen({{sym::state, sym::work},{sym::seq},[](ref ctx)
	{
		seq genuses = ctx[sym::seq].as<seq>();
		seq result({});
		for (ref refitem : genuses) {
			gen_use item = refitem.as<gen_use>();
			item.reset();
			result += item.next();
		}
		ctx <= rs{
			{sym::state, genuses},
			{sym::work, result}
		};
	}},{{sym::seq},{sym::state, sym::work},[](ref ctx)
	{
		ref work = ctx[sym::work];
		if (!work) {
			ctx <= r{sym::seq, sym::nothing};
			return;
		}
		seq spot = work.as<seq>();
		seq result = spot.clone();
		seq genuses = ctx[sym::state].as<seq>();
		
		bool done = true;
		iterator<ref> workitem = spot.begin();
		for (ref refgenerator : genuses) {
			gen_use generator = refgenerator.as<gen_use>();
			ref item = generator.next();
			if (item) {
				workitem.set(sym::what, item);
				done = false;
				break;
			} else {
				generator.reset();
				item = generator.next();
				workitem.set(sym::what, item);
			}
		}

		if (done) {
			ctx.set(sym::work, sym::nothing);
		}

		ctx <= r{sym::seq, result};
	}});

// generates infinite combinations from a single sequence, producing longer
// and longer sequences
gen forever_seq_gen({{sym::gen_use, sym::work},{sym::gen_use},[](ref ctx)
	{
		gen_use gen = ctx[sym::gen_use].as<gen_use>();
		ctx <= rs{
			{sym::gen_use, gen},
			{sym::work, seq({
				ref({
					{sym::work, seq({})}
				})
			})}
		};
	}},{{sym::seq},{sym::gen_use, sym::work},[](ref ctx)
	{
		seq states = ctx[sym::work].as<seq>();

		ref state = states.pop_front();
		seq work = state[sym::work].as<seq>();

		gen_use generator = ctx[sym::gen_use].as<gen_use>();
		generator.reset();

		ref option;
		while ( (option = generator.next()) ) {
			seq next_work = work.clone();
			next_work += option;
			states += ref({
				{sym::work, next_work}
			});
		}
		ctx.set(sym::seq, work);
	}});

namespace sym {
	symbol(habit_gen_use);
	symbol(parameter_gen_use);
}

// makes steps by combining parameters and habits
// 	make a finite_combination_gen for the outputs and inputs
gen step_gen({{sym::habit_gen_use,sym::parameter_gen_use},{sym::habit_gen_use, sym::parameter_gen_use},[](ref ctx){
		gen_use habitgen = ctx[sym::habit_gen_use].as<gen_use>();
		ref firsthabit = habitgen.next();
		ctx <= rs{
			{sym::habit_gen_use, ctx[sym::habit_gen_use]},
			{sym::parameter_gen_use, ctx[sym::parameter_gen_use]},
			//{sym::habit, 
		//ctx[sym::habit_gen_use];
		};
		// stub
	}},{{sym::step},{sym::habit_gen_use, sym::parameter_gen_use},[](ref ctx){
		// stub

		// to meet goal quickly, propose select variables/literals
		// naively for now, using existing tools.

		// step generation can then simplify.
		// variables and literals go in 1 big list
		// and one is rpovided for each input/output
	}});

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

	//ref state = enumeration::start({seq_gen, list});
	gen_use gen(forever_seq_gen, r{
		sym::gen_use, gen_use(seq_gen, r{
			sym::seq, list
		})
	});

	//while (state[enumeration::states].as<seq>().size() < 64) {//states.size() < 16) {
	for (unsigned long i = 0; i < 16; ++ i) {
		ref work = gen.next();//enumeration::pop_and_append({state});
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
