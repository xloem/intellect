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
			}
			generator.reset();
			item = generator.next();
			workitem.set(sym::what, item);
			++ workitem;
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

// make steps by combining parameters and habits
// 	make a finite_combination_gen for the outputs and inputs
gen step_gen({{sym::habit_gen_use,sym::parameter_gen_use,sym::work},{sym::habit_gen_use, sym::parameter_gen_use},[](ref ctx){
		gen_use parameter_gen = ctx[sym::parameter_gen_use].as<gen_use>();
		gen_use habit_gen = ctx[sym::habit_gen_use].as<gen_use>();
	
		cxxhabit habit = habit_gen.next().as<cxxhabit>();
		// could make constructor to seq that maps a habit over another seq
		seq parameter_gens({});
		for (ref output : habit[sym::outputs].as<seq>()) {
			parameter_gens += parameter_gen.make_new();
		}
		for (ref input : habit[sym::inputs].as<seq>()) {
			parameter_gens += parameter_gen.make_new();
		}
		gen_use habit_parameter_gen(finite_combinations_gen, parameter_gens);
	
		ctx <= rs{
			{sym::habit_gen_use, habit_gen},
			{sym::parameter_gen_use, parameter_gen},
			{sym::state, rs{
				{sym::habit, habit},
				{sym::gen, habit_parameter_gen}
			}}//,
			//{sym::habit, habit},
			//{sym::seq, parameters}
		};
	}},{{sym::step},{sym::habit_gen_use, sym::parameter_gen_use, sym::state},[](ref ctx){
		ref state = ctx[sym::state];
		ref refhabit = state[sym::habit];
		if (!refhabit) {
			return;
		}
		cxxhabit habit = refhabit.as<cxxhabit>();
		gen_use habit_parameter_gen = state[sym::gen].as<gen_use>();
		seq parameters = habit_parameter_gen.next().as<seq>();
		if (!parameters) {
			gen_use habit_gen_use = ctx[sym::habit_gen_use].as<gen_use>();
			habit = habit_gen_use.next().as<cxxhabit>();
			state <= r{sym::habit, habit};
			if (!habit) {
				ctx <= r{sym::step, sym::nothing};
				return;
			}
			gen_use parameter_gen = ctx[sym::parameter_gen_use].as<gen_use>();
			seq parameter_gens({});
			for (ref output : habit[sym::outputs].as<seq>()) {
				parameter_gens += parameter_gen.make_new();
			}
			for (ref input : habit[sym::inputs].as<seq>()) {
				parameter_gens += parameter_gen.make_new();
			}
			habit_parameter_gen = gen_use(finite_combinations_gen, parameter_gens);
			state <= r{sym::gen, habit_parameter_gen};
			parameters = habit_parameter_gen.next().as<seq>();
		}
		ctx <= r{sym::step, step(parameters, habit)};
	}});

#include <iostream>
int main()
{
	symbol(one);
	symbol(two);
	seq list({one,two});
	symbol(thingy);
	symbol(stuff);
	seq list2({thingy,stuff});
	symbol(apple);
	symbol(orange);
	symbol(banana);
	seq list3({apple,orange,banana});

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

	gen_use gen2(finite_combinations_gen,
		r{sym::seq, seq({
			gen_use(seq_gen, r{sym::seq, list}),
			gen_use(seq_gen, r{sym::seq, list2}),
			gen_use(seq_gen, r{sym::seq, list3})
		})}
	);

	while (ref work = gen2.next()) {
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
