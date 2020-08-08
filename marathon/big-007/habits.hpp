#pragma once
#include "habit.hpp"
#include "seq.hpp"

namespace sym {
	symbol(from);
	symbol(type);
	symbol(to);

	//symbol(first);
	symbol(second);

	symbol(from_anchor);
	symbol(to_anchor);
}

namespace act {

	cxxhabit get({sym::to},{sym::from, sym::type},[](ref ctx)
	{
		ctx.set(sym::to, ctx[sym::from][ctx[sym::type]]);
	});
	
	cxxhabit set({},{sym::from, sym::type, sym::to},[](ref ctx)
	{
		ref from = ctx[sym::from];
		from.set(ctx[sym::type], ctx[sym::to]);
	});

	// map replacement.  
	cxxhabit set_map_same_anchor({/*sym::to_anchor, sym::to*/},{sym::to_anchor, sym::from, sym::to},[](ref ctx)
	{
		// prolog boilerplate
		seq from = ctx[sym::from].as<seq>();
		seq to = ctx[sym::to].as<seq>();
		ref anchor = ctx[sym::to_anchor].as<seq>();

		ref source = anchor;
		for (auto step : from) {
			source = source[step];
		}
		/*ctx[sym::to] = source;*/

		// first step is sym::from_anchor
		ref dest = ctx;
		ref dest_step = sym::to_anchor;
		for (auto step : to) {
			dest = dest[dest_step];
			dest_step = step;
		}

		dest.set(dest_step, source);
	});

	cxxhabit iterator_increment({},{sym::context,sym::iterator},[](ref ctx)
	{
		ref iterator_name = ctx[sym::iterator];
		ctx = ctx[sym::context];

		set_map_same_anchor({ctx, seq({iterator_name,sym::next}), seq({iterator_name})});
	});

	// to _use_ this we'll need to add a way to refer to the context, which is easy
	/*
	stepshabit iterator_increment({},{sym::context,sym::iterator},[](ref ctx)
	{
		// stub
	});
	*/

	// incrementation of an iterator is simplifiable
	// the item in the context is replaced by one of its attributes
	// the operation is of replacing a property with a property's property
		// replacing a property with its property is 2 gets and a set
	cxxhabit prop_to_prop_of({},{sym::what, sym::first, sym::second},[](ref ctx)
	{
		// map replacement could do this already.
		// set_map_same_anchor({what, seq({first,second}), seq({first})});

		// note: these 3 lines are boilerplate unneeded at runtime
		ref what = ctx[sym::what];
		ref first = ctx[sym::first];
		ref second = ctx[sym::second];

		// computer-generated function would be 3 statements long.
		ref previous = act::get({what, first});
		ref next = act::get({previous, second});
		act::set({what, first, next});

	});
}
