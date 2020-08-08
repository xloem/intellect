#define ref_cpp
#define main_unit
#include "../big-006/ref.hpp"
#include "../big-006/gen.hpp"
#include "../big-006/dump.hpp"

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
	symbol(apple);
	symbol(orange);
	symbol(banana);
	symbol(hello);
	symbol(world);
	text space(" ");
	text endl("\n");
	text questionmark("?");
	seq parameters({hello,space,world,endl,questionmark,var(apple),var(orange),var(banana)});

	cxxhabit write({},{sym::text},[](ref context)
	{
		std::cout << context[sym::text].as<text>().data();
	});
	cxxhabit read({var(sym::text)},{},[](ref context)
	{
		std::string word;
		std::cin >> word;
		context.set(sym::text, text(word));
	});
	seq habits({write,read});

	gen_use parameter_gen(seq_gen, {parameters});
	gen_use habit_gen(seq_gen, {habits});

	cxxhabit habit = habit_gen.next().as<cxxhabit>();
	seq parameter_gens({});
	// could make constructor to seq that maps a habit over another seq
	for (ref input : habit[sym::inputs].as<seq>()) {
		parameter_gens += parameter_gen.make_new();
	}
	for (ref output : habit[sym::outputs].as<seq>()) {
		parameter_gens += parameter_gen.make_new();
	}

	// TODO: pass parameter_gens to finite_combinations_gen to get all the options
	// instead of the below chunk

	seq parameters({});
	for (ref genref : parameter_gens) {
		gen_use gen = genref.as<gen_use>();
		parameters += gen.next();
	}


	ref ctx({
		{sym::habit_gen_use, habit_gen},
		{sym::parameter_gen_use, parameter_gen},
		{sym::work, rs{
			{sym::habit, habit},
			{sym::seq, parameter_gens}
		},
		{sym::habit, habit},
		{sym::seq, parameters}
	});

	ref first_habit = habit_gen.next();
	std::cout << dump(first_habit) << std::endl;
	ref first_parameter = parameter_gen.next();
	std::cout << dump(first_parameter) << std::endl;
}
