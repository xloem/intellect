#include "ref.hpp"
#include "seq.hpp"
#include "habit.hpp"
#include "dump.hpp"

namespace sym {
	symbol(options);
	symbol(work);
}

namespace enumeration {

	symbol(states);
	
	cxxhabit start({sym::state},{sym::options},[](ref ctx){
		ctx.set(sym::state, ref({
			{enumeration::states, seq({
				ref({
					{sym::options, ctx[sym::options]},
					{sym::work, seq({})}
				})
			})}
		}));
	});

	cxxhabit pop_and_append({sym::work},{sym::state},[](ref ctx){
		ref state = ctx[sym::state];
		seq states = state[enumeration::states].as<seq>();

		state = states.pop_front();
		seq work = state[sym::work].as<seq>();
		seq options = state[sym::options].as<seq>();

		for (ref option : options) {
			seq next_work = work.clone();
			next_work += option;
			states += ref({
				{sym::options, options},
				{sym::work, next_work}
			});
		}
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

	ref state = enumeration::start({list});

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

	cxxhabit read({sym::text},{},[](ref context)
	{
		std::string word;
		std::cin >> word;
		context.set(sym::text, text(word));
	});

	write({text("hello world\n")});
	write({text("enter a word:\n")});
	ref word = read({});
	write({text("you entered: ")});
	write({word});
	write({text("\n")});
}
