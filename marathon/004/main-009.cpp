auto notice = "uhh note: maybe move type verification to virtual method, copy to other files when opened?";  // incremental organization

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

}

#include <iostream>
int main()
{
	symbol(one);
	symbol(two);
	symbol(three);
	seq list({one,two,three});

	std::cout << dump(list) << std::endl;

	cxxhabit printlist({},{sym::seq},[](ref context)
	{
		for (ref item : seq(context.get(sym::seq))) {
			std::cout << item.data<std::string>() << std::endl;
		}
	});

	cxxhabit write({},{sym::text},[](ref context)
	{
		std::cout << context[sym::text].as<text>().data() << std::endl;
	});

	printlist({list});
	write({text("hello world\n")});

	ref state = enumeration::start({list});
	seq states = state[enumeration::states].as<seq>();

	while (states.size() < 16) {
		ref state = states.pop_front();
		seq work = state[sym::work].as<seq>();
		seq options = state[sym::options].as<seq>();
		for (ref option : options) {
			seq next_work = work.clone();
			next_work += option;
			std::cout << dump(next_work) << std::endl;
			states += ref({
				{sym::options, options},
				{sym::work, next_work}
			});
		}
	}
}
