#include "gen.hpp"
#include "seq.hpp"
#include "habits.hpp"

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

		// boilerplate prolog
		iterator<ref> state = ctx[sym::state].as<iterator<ref>>();

		// 2 calls, 4 parameters.  {sym::what and sym::next are relevent to sym::state being a sequence iterator}

		// ref what = *state;
		ref what = act::get(state, sym::what);

		//++ state;
		state = act::get(state, sym::next).as<iterator<ref>>(); // common thing to do to an iterator

		// boilerplate epilog
		act::set(ctx, sym::state, state);
		act::set(ctx, sym::what, what);
	}});

namespace sym {
	symbol(interesting);
	symbol(boring);
}

// sym::gen is here for considering the idea of using a gen to pick our options.
// we'd have to instantiate it with the options.
gen explore_gen({{sym::work/*, sym::interesting, sym::boring*/},{sym::what/*, sym::interesting, sym::boring*/},[](ref ctx)
{
	ref what = ctx[sym::what];
	
	seq work({what});

	ctx.set(sym::work, work);
	
	//ref gen = ctx[sym::gen]; // this idea was to control the priority in a seq.
	                           // it roughly takes a generator and spews the options out in
				   // a different order.
		// what's left out is that it was going to apply to the link types,
		// not the returned nodes
			// we could use priority
			// or a seq
}},{{sym::what},{sym::work},[](ref ctx){
	seq work = ctx[sym::work].as<seq>();

	ref what = work.pop_front();
	for (auto link : what->refs) {
		// link.first lost with loss of order-of-interest part
		work.push_back(link.second);
	}

	ctx.set(sym::what, what);
}});
