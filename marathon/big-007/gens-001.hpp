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

		// 2 calls.  {'what' and 'next' are relevent to sym::state being an iterator}

		ref what = act::get({state, sym::what});
		//ctx.set(sym::what, *state);

		//++ state;
		state = act::get({state, sym::next});

		// boilerplate epilog
		//ctx.set(sym::state, element);
		act::set({ctx, sym::state, state});
		act::set({ctx, sym::what, what});
	}});
