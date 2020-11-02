#define main_unit
#include "ref.hpp"
#include "seq.hpp"
#include "habit.hpp"
#include "dump.hpp"
#include "gens.hpp"

// might be sensical to have gen setup list its outputs and
// preserve those, passing all to next context

// generates sequences of items from the provided sequence of gen_uses
void finite_combinations_gen_setup(ref ctx);
void finite_combinations_gen_next(ref ctx);
gen finite_combinations_gen(
	{{sym::state, sym::work},{sym::seq},finite_combinations_gen_setup},
	{{sym::seq},{sym::state, sym::work},finite_combinations_gen_next});
void finite_combinations_gen_setup(ref ctx)
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
}
void finite_combinations_gen_next(ref ctx)
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
}

// generates infinite combinations from a single sequence, producing longer
// and longer sequences
void forever_seq_gen_setup(ref ctx);
void forever_seq_gen_next(ref ctx);
gen forever_seq_gen(
	{{sym::gen_use, sym::work},{sym::gen_use},forever_seq_gen_setup},
	{{sym::seq},{sym::gen_use, sym::work},forever_seq_gen_next});
void forever_seq_gen_setup(ref ctx)
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
}
void forever_seq_gen_next(ref ctx)
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
}

namespace sym {
	symbol(habit_gen_use);
	symbol(parameter_gen_use);
}

// make steps by combining parameters and habits
// 	make a finite_combination_gen for the outputs and inputs
void step_gen_setup(ref ctx);
void step_gen_next(ref ctx);
gen step_gen(
	{{sym::habit_gen_use,sym::parameter_gen_use,sym::state},{sym::habit_gen_use, sym::parameter_gen_use},step_gen_setup},
	{{sym::step},{sym::habit_gen_use, sym::parameter_gen_use, sym::state},step_gen_next}
);
void step_gen_setup(ref ctx)
{
	gen_use parameter_gen = ctx[sym::parameter_gen_use].as<gen_use>();
	gen_use habit_gen = ctx[sym::habit_gen_use].as<gen_use>().make_new();

	cxxhabit habit = habit_gen.next().as<cxxhabit>();
	// could make constructor to seq that maps a habit over another seq
	seq parameter_gens({});
	for (ref output : habit[sym::outputs].as<seq>()) {
		parameter_gens += parameter_gen.make_new();
	}
	for (ref input : habit[sym::inputs].as<seq>()) {
		parameter_gens += parameter_gen.make_new();
	}
	gen_use habit_parameter_gen(finite_combinations_gen, {parameter_gens});

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
}
void step_gen_next(ref ctx)
{
	// prolog
	ref state = ctx[sym::state];
	ref refhabit = state[sym::habit];

	if (!refhabit) {
		return; // control flow.
	}

	cxxhabit habit = refhabit.as<cxxhabit>();
	gen_use habit_parameter_gen = state[sym::gen].as<gen_use>();
	ref parametersref = habit_parameter_gen.next();
	if (!parametersref) {
		gen_use habit_gen_use = ctx[sym::habit_gen_use].as<gen_use>();
		refhabit = habit_gen_use.next();
		state <= r{sym::habit, refhabit};
		if (!refhabit) {
			ctx <= r{sym::step, sym::nothing};
			return;
		}
		habit = refhabit.as<cxxhabit>();
		gen_use parameter_gen = ctx[sym::parameter_gen_use].as<gen_use>();
		seq parameter_gens({});
		for (ref output : habit[sym::outputs].as<seq>()) {
			parameter_gens += parameter_gen.make_new();
		}
		for (ref input : habit[sym::inputs].as<seq>()) {
			parameter_gens += parameter_gen.make_new();
		}
		habit_parameter_gen = gen_use(finite_combinations_gen, {parameter_gens});
		state <= r{sym::gen, habit_parameter_gen};
		parametersref = habit_parameter_gen.next();
	}
	seq parameters = parametersref.as<seq>();
	ctx <= r{sym::step, step(parameters, habit)};
}

namespace check {
	// let's check a seq_gen iterator.
	// it should go in a seq_gen form and advance the seq_gen when used.
	// could add sanity checks too.
	

}

#include <iostream>
void write_func(ref context);
cxxhabit write({},{sym::text},write_func);
void write_func(ref context)
{
	std::cout << context[sym::text].as<text>().data();
}

// default-to-literal inefficient
void read_func(ref context);
cxxhabit read({var(sym::text)},{},read_func);
void read_func(ref context)
{
	std::string word;
	std::cin >> word;
	context.set(sym::text, text(word));
}

void printlist_func(ref context);
cxxhabit printlist({},{sym::seq},printlist_func);
void printlist_func(ref context)
{
	bool first = true;
	for (ref item : seq(context.get(sym::seq))) {
		if (first) {
			first = false;
		} else {
			std::cout << ",";
		}
		std::cout << dump_ex(item,false);
	}
}

void printsteps_func(ref ctx);
cxxhabit printsteps({},{sym::steps},printsteps_func);
void printsteps_func(ref ctx)
{
	// this is not a stephabit yet
	// it is just a sequence of steps
	stephabit steps = ctx[sym::steps].as<stephabit>();

	std::cout << dump_ex(steps, false) << ":" << std::endl;
	for (ref step : steps[sym::what].as<seq>()) {
		std::cout << "  ";
		seq outputs = step[sym::outputs].as<seq>();
		if (outputs.size()) {
			printlist({step[sym::outputs]});
			std::cout << " = " ;
		}
		std::cout << dump_ex(step[sym::what], false) << "(";
		printlist({step[sym::inputs]});
		std::cout << ")" << std::endl;
	}
}

void setvar_func(ref ctx) {}
cxxhabit setvar({sym::what},{sym::what},setvar_func);

void loopbody_func(ref ctx);
cxxhabit loopbody({sym::what},{sym::gen, sym::input},loopbody_func);
void loopbody_func(ref ctx)
{
	gen_use habit_gen = ctx[sym::gen].as<gen_use>();
	stephabit a_habit({},{},habit_gen.next().as<seq>());
	printsteps({a_habit});
	stephabit readwrite = ctx[sym::input].as<stephabit>();
	text input = readwrite({});
	std::cout << input.data() << std::endl;
	std::cout << dump_ex(input, true, "", true) << std::endl;
	if (input.data() == "q" || !input) {
		ctx.wipe(sym::what);
	} else {
		ctx.set(sym::what, sym::is);
	}
}

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

	printlist({list});
	std::cout << std::endl;


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
		std::cout << std::endl;
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
		std::cout << std::endl;
	}

	std::cout << dump(list) << std::endl;


	text space(" ");
	text endl("\n");

	symbol(word);
	seq words({
		space,
		endl,
		text("hello"),
		text("world"),
		text("enter a word"),
		text("you entered:"),
		var(word)
	});

	seq actions({
		write,
		read
	});

	gen_use words_gen(seq_gen, {words});
	gen_use actions_gen(seq_gen, {actions});
	
	gen_use single_step_gen(step_gen, {actions_gen, words_gen});
	gen_use habit_gen(forever_seq_gen, {single_step_gen});

	// let's pass habit_gen to the mainloop and loopbody
	// and see if whilesteps is stable enough to do it
	// 	considering tests goes with use-things-as-soon-as-they-function
	// 	for stability shape.  we need some testing, or something like it.

	stephabit helloworld({},{},{
		{{},{text("hello")}, write},
		{{},{space}, write},
		{{},{text("world")}, write},
		{{},{endl}, write}
	});

	// let's make the loop be a stephabit.
	// it will need a way to break.
	stephabit readwrite({word},{},{
		{{},{text("enter a word, 'q' to stop:\n")}, write},
		{{word},{}, read},
		{{},{text{"you entered: "}}, write},
		{{},{word}, write},
		{{},{endl}, write}
	});

	printsteps({helloworld});
	printsteps({readwrite});

	helloworld({});

	stephabit mainloop({},{},{
		{{var(sym::what)}, {sym::is}, setvar},
		{{},{var(sym::what)}, write},
		{{},{endl}, write},
		{{var(sym::what)},{var(sym::what),seq({ // todo? change whilesteps to not rely on outer context, or to copy outer context in fully
			step({sym::what},{habit_gen, readwrite},loopbody),
			step({},{sym::what},write)
		})},act::whilesteps_gimmick}
	});

	mainloop({});
	//while (loopbody({}));

}