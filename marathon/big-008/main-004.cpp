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
#include <sstream>
#include <functional>
std::ostream * output_stream = &std::cout;
std::istream * input_stream = &std::cin;

void write_func(ref context);
cxxhabit write({},{sym::text},write_func);
void write_func(ref context)
{
	*output_stream << context[sym::text].as<text>().data();
}


using handler_void_void = std::function<void()>;
handler_void_void noop_void_void = [](){};
handler_void_void before_read_handler = noop_void_void;

// default-to-literal inefficient
void read_func(ref context);
cxxhabit read({variable(sym::text)},{},read_func);
void read_func(ref context)
{
	std::string word;
	before_read_handler();
	*input_stream >> word;
	context.set(sym::text, text(word));
}

namespace txt {
	text null("");
	text space(" ");
	text comma(",");
	text endl("\n");
}

//namespace var {
//	text seq("seq", false);
//	text word("word", false);
//}

void writeseq_func(ref ctx);
cxxhabit writeseq({},{sym::seq, sym::text, sym::next},writeseq_func);
void writeseq_func(ref ctx)
{
	seq list = ctx[sym::seq].as<seq>();
	bool first = true;
	for (auto item : list) {
		if (first) {
				first = false;
		} else {
			*output_stream << ctx[sym::text].as<text>().data();
		}
		*output_stream << item.as<text>().data();
	}
	*output_stream << ctx[sym::next].as<text>().data();
};
//stephabit printlist({},{variable(sym::seq)},{
//	{{},{sym::seq, txt::comma, txt::null}, writeseq}
//});
void printlist_func(ref ctx);
cxxhabit printlist({},{sym::seq},printlist_func);
void printlist_func(ref ctx)
{
	seq list = ctx[sym::seq].as<seq>();
	bool first = true;
	for (auto item : list) {
		if (first) {
			first = false;
		} else {
			*output_stream << ",";
		}
		*output_stream << dump_ex(item, 0);//item.as<text>().data();
	}
	//*output_stream << ctx[sym::next].as<text>().data();
};

void printsteps_func(ref ctx);
cxxhabit printsteps({},{sym::steps},printsteps_func);
void printsteps_func(ref ctx)
{
	// this is not a stephabit yet
	// it is just a sequence of steps
	stephabit steps = ctx[sym::steps].as<stephabit>();

	*output_stream << dump_ex(steps, 0) << ":" << std::endl;
	for (ref step : steps[sym::what].as<seq>()) {
		*output_stream << "  ";
		seq outputs = step[sym::outputs].as<seq>();
		if (outputs.size()) {
			printlist({step[sym::outputs]});
			std::cout << " = " ;
		}
		*output_stream << dump_ex(step[sym::what], 0) << "(";
		printlist({step[sym::inputs]});
		*output_stream << ")" << std::endl;
	}
}

void smartwrite_func(ref ctx);
cxxhabit smartwrite({},{sym::text},smartwrite_func);
void smartwrite_func(ref ctx)
{
	ref text = ctx[sym::text];
	if (text[sym::is] == sym::seq) {
		writeseq(text, txt::space, txt::endl);
	} else {
		write(text);
		write(txt::endl);
	}
}
stephabit smartprompt({variable(sym::what)},{sym::text},{
	{{},{sym::text},smartwrite},
	{{sym::what},{},read}
});

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
	*output_stream << input.data() << std::endl;
	*output_stream << dump_ex(input, 3, "", true) << std::endl;
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


	symbol(word);
	//cxxhabit prompt({word},{sym::seq}) {
	//}
	stephabit promptword({word},{},{
		{{},{text("enter a word:\n")}, write},
		{{word},{}, read}
	});
	stephabit showword({},{word},{
		{{},{text{"you entered: "}}, write},
		{{},{word}, write},
		{{},{txt::endl}, write}
	});
	seq words({
		variable(word),
		seq({text("enter"),text("a"),text("word:")}),
		seq({text("you"),text("entered:")}),
		seq({text("hello"),text("world")}),
		//txt::space,
		//txt::endl,
		//text("hello"),
		//text("world"),
		//text("enter a word\n"),
		//text("you entered: "),
	});

	seq actions({
		//write,
		//read
		smartprompt,
		smartwrite,
		//promptword,
		//showword,
		//write
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
		{{},{txt::space}, write},
		{{},{text("world")}, write},
		{{},{txt::endl}, write}
	});

	// let's make the loop be a stephabit.
	// it will need a way to break.
	stephabit readwrite({word},{},{
		{{},{text("enter a word, 'q' to stop:\n")}, write},
		{{word},{}, read},
		{{},{text{"you entered: "}}, write},
		{{},{word}, write},
		{{},{txt::endl}, write}
	});

	printsteps({helloworld});
	printsteps({readwrite});

	helloworld({});

	cxxhabit testreadwritefunc({sym::what},{sym::habit},[](ref ctx){
		std::stringstream test_input_stream;
		std::stringstream test_output_stream;
		auto old_input = input_stream;
		auto old_output = output_stream;
		input_stream = &test_input_stream;
		output_stream = &test_output_stream;

		ctx.set(sym::what, sym::is);

		test_input_stream.str("something\n" + std::string("dummy1 dummy2 dummy3 dummy4 dummy5 dummy6 dummy7 dummy8 dummy9 dummy10"));
		cxxhabit habit = ctx[sym::habit].as<cxxhabit>();

		before_read_handler = [&](){
			if  (test_output_stream.str() != "enter a word:\n") {
				ctx.wipe(sym::what);
			}
		};

		habit();
		// NOTE: this could be a nearness comparison, and return a relative value,
		// that could be used to pick exploration branches
		if (test_output_stream.str() != "enter a word:\nyou entered:\nsomething\n") {
			ctx.wipe(sym::what);
		}

		before_read_handler = noop_void_void;

		input_stream = old_input;
		output_stream = old_output;
	});

	cxxhabit testhelloworldfunc({sym::what},{sym::habit},[](ref ctx){
		std::stringstream test_input_stream;
		std::stringstream test_output_stream;
		auto old_input = input_stream;
		auto old_output = output_stream;
		input_stream = &test_input_stream;
		output_stream = &test_output_stream;

		ctx.set(sym::what, sym::is);

		test_input_stream.str(std::string("dummy1 dummy2 dummy3 dummy4 dummy5 dummy6 dummy7 dummy8 dummy9 dummy10"));
		before_read_handler = [&](){
			ctx.wipe(sym::what);
		};

		cxxhabit habit = ctx[sym::habit].as<cxxhabit>();
		habit();
		// NOTE: this could be a nearness comparison, and return a relative value,
		// that could be used to pick exploration branches
		if (test_output_stream.str() != "hello world\n") {
			ctx.wipe(sym::what);
		}

		before_read_handler = noop_void_void;

		input_stream = old_input;
		output_stream = old_output;
	});

	stephabit designedpassinghabit({},{},{
		{{word}, {seq({text("enter"),text("a"),text("word:")})}, smartprompt},
		{{}, {seq({text("you"),text("entered:")})}, smartwrite},
		{{}, {word}, smartwrite}
	});

	if (testreadwritefunc(designedpassinghabit)) {
		*output_stream << "designed passing readwrite passed" <<std::endl;
	} else {
		// TODO: relating with flow streams, open a space for what subhabits to pick based on goals.  wrong data can be isolated to wrong flow chain that produced that data.
		*output_stream << "designed passing readwrite failed" <<std::endl;
		return 0;
	}

	if (testhelloworldfunc(helloworld)) {
		*output_stream << "designed passing helloworld passed" <<std::endl;
	} else {
		// TODO: relating with flow streams, open a space for what subhabits to pick based on goals.  wrong data can be isolated to wrong flow chain that produced that data.
		*output_stream << "designed passing helloworld failed" <<std::endl;
		return 0;
	}

	unsigned long count;
	count = 0;
	while (true) {
		++ count;
		seq steps = habit_gen.next().as<seq>();
		stephabit habit({},{},steps);
		//printsteps(habit);
		bool success = testhelloworldfunc(habit);
		*output_stream << count << "\r" << std::flush;
		if (success) {
			*output_stream << "found working helloworld after " << count << std::endl;
			printsteps(habit);
			habit();
			break;
		}
	}

	count = 0;
	while (true) {
		++ count;
		seq steps = habit_gen.next().as<seq>();
		stephabit habit({},{},steps);
		//printsteps(habit);
		bool success = testreadwritefunc(habit);
		*output_stream << count << "\r" << std::flush;
		if (success) {
			*output_stream << "found working readwrite after " << count << std::endl;
			printsteps(habit);
			habit();
			break;
		}
	}
	return 0;

	stephabit mainloop({},{},{
		{{variable(sym::what)}, {sym::is}, setvar},
		{{},{variable(sym::what)}, write},
		{{},{txt::endl}, write},
		{{variable(sym::what)},{variable(sym::what),seq({ // todo? change whilesteps to not rely on outer context, or to copy outer context in fully
			step({sym::what},{habit_gen, readwrite},loopbody),
			step({},{sym::what},write)
		})},act::whilesteps_gimmick}
	});

	mainloop({});
	//while (loopbody({}));

}
