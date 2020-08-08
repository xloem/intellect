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
		iterator<ref> element = ctx[sym::state].as<iterator<ref>>();

		act::set({ctx, sym::what, *element});
		//ctx.set(sym::what, *element);

		++ element;

		//ctx.set(sym::state, element);
		act::set({ctx, sym::state, element});
	}});
