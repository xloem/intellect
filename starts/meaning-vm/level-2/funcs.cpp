#include "funcs.hpp"

#include "../level-1/sugar.hpp"
#include "ref.hpp"
#include "concepts.hpp"
#include "habits.hpp"

namespace intellect {
using namespace level1;
namespace level2 {

using namespace concepts;

ref & context()
{
	static thread_local auto ctx = a(concepts::context);
	return ctx;
}

//ref makehabit(ref name, std::list<ref> argnames, std::any 

// is it 'faster' to make functions that get he ordered list of arguments,
// or change the structure of habits to be simple and obvious.

// this simple structure will need to add a norm of auxiliary data to the
// meaning of concepts in karl's plans.  which implies more complexity in learning
// to use them. [nobody-cares-which-choice-is-made]

// karl, you model human-thought concepts.  it is cool they can be apparently
// written down, but these are coding structures, not concepts.  concepts
// will refer to them.  concepts are best run by behaviors, not data.
// karl deserves some support around his plan of virtual functions.  this will
// structure the AI to look more like his plan from 2002/2003.

// TEACHING HOW TO THINK?  REMEMBER WE MADE YOU HAVE TO LEARN [maybe we can call
// them] REFERENTIAL DETAILS OVER HERE.  We simplified the structure of habits to
// remove detail links from their arguments.  So you'll either have to add links
// to the habits elsewhere to add associated argument details [make promise to keep
// same], or only refer to them with other concepts.  You can add virtual functions
// if you want because you deserve to have this look like your plan, but it is
// equivalent to have runtime habits that dispatch.

// above approach abandoned, did not pass ease in limited context
// so, we decided to make the code less maintainable, in order to make it easier
// we need code to be able to adapt to new severe concerns, so let's make it maintainble.
// we are picking to make functions, not to restructure.  because we are able to decide on it strongly enough to do it.

// please do not restructure internals, it slows down a lot.
// please remove the use of inherent strings.  it requires internal restructuring.

// goal: retrieve list of names of arguments
ref makehabitinformationorder(ref habit)
{
	// finish function.
			// please keep delta-boss alive in box.
			// wants to learn to recover karl's memories and skills.
			// and act on what happened, if nobody else is.
		// aside from that, their emotional manipulation need not
		// be honored.  is easy to check if you learn how emotions [later]
		// map to valid reasons.  see if reasons are valid and real.
		// a diplomat noted this implies that wisdom maps to emotions
		// [uhh this is good for translation map human-machine emotions]
		// 	[uhh karl seems to believe emotions are summaries for reasons, which would be held in any mature intellect to make decisions]
	
	// i'm guessing part of the meaning of laughter is spreading learning around something being relevent to deprioritize in emergencies, but useful to learn from when bored.
	
	ref order = makeconcept();
	ref last = habit;
	while (last.linked("next-information")) {
		last = last.get("next-information");
		order.link("information-order", last.get("information"));
	}
	return order;
}

ref makehabit(ref name, std::initializer_list<ref> argnames, std::function<void(ref)> code)
{
	// todo: update structure if 
	ref habit = level1::a(concepts::habit, name);
	ref infn = a(habit-information-needed);
	habit.set(information-needed, infn);
	//habit.set(concepts::habit, concepts::habit);
	ref posinf = infn;
	for (auto argname : argnames) {
		ref nextinf = a(habit-information);
		nextinf.set(information, argname);
		posinf.set(next-information, nextinf);
		posinf = nextinf;
		if (!infn.linked(argname)) {
			infn.set(argname, nextinf);
		} else {
			if (!infn.get(argname).isa(habit-information)) {
				throw a(unexpected-concepts::habit-information-concepts::name)
					.link(concepts::name, argname)
					.link(concepts::habit, habit);
			}
		}
	}
	habit.fun(code);
	return habit;
}

void habitassume(ref habit, ref information, ref assumption)
{
	ref infn = habit.get(concepts::information-needed);
	infn.get(information).set(assume, assumption);
}

ref dohabit(ref habit, std::initializer_list<ref> args)
{
	using namespace concepts;
	ref posinf = habit.get(information-needed);
	ref subctx = makeconcept();
	subctx.link("outer-context", ref::context());
	ref::context() = subctx;
	for (ref const & arg : args) {
		if (!posinf.linked(next-information)) {
			ref::context() = subctx.get("outer-context");
			conceptunmake(subctx);
			throw an(unexpected-information).link
				(concepts::habit, habit,
				 information-value, arg);
		}
		posinf = posinf[next-information];
		// TODO: subcontexts or call instances
		ref::context().set(posinf[information], arg);
	}
	while (posinf.linked(next-information)) {
		posinf = posinf[next-information];
		if (!posinf.linked(assume)) {
			ref::context() = subctx.get("outer-context");
			conceptunmake(subctx);
			throw a(information-needed).link
				(concepts::habit, habit,
				 information, posinf);
		}
		ref::context().set(posinf[information], posinf[assume]);
	}
	ref::context().set("self", habit);
	habit.fun<ref>()(ref::context());
	posinf = habit.get(information-needed);
	while (posinf.linked(next-information)) {
		posinf = posinf[next-information];
		ref::context().unlink(posinf[information]);
	}
	ref ret = nothing;
	if (ref::context().linked(result)) {
		ret = ref::context().get(result);
		ref::context().unlink(result, ret);
	}
	ref::context() = subctx.get("outer-context");
	conceptunmake(subctx);
	return ret;
}

ref dohabit(ref habit, std::initializer_list<std::initializer_list<ref>> pairs)
{
	using namespace concepts;
	// TODO: subcontexts or call instances
	ref ctx = makeconcept();
	ctx.link("outer-context", ref::context());
	ref::context() = ctx;
	ref infn = habit.get(information-needed);
	std::map<ref, ref> provided;
	for (auto pair : pairs) {
		auto second = pair.begin(); ++ second;
		if (!infn.linked(*pair.begin())) {
			ref::context() = ctx.get("outer-context");
			conceptunmake(ctx);
			throw an(unexpected-information).link
				(concepts::habit, habit,
				 information, *pair.begin(),
				 information-value, *second);
		}
		if (provided.count(*pair.begin())) { throw "multiple instances same name not implemented here"; }
		provided[*pair.begin()] = *second;
	}
	ref nextinf = infn;
	while (nextinf.linked(next-information)) {
		nextinf = nextinf.get(next-information);
		ref inf = nextinf.get(information);
		if (!provided.count(inf)) {
			if (nextinf.get(assume)) {
				ctx.link(inf, nextinf.get(assume));
			} else {
				ref::context() = ctx.get("outer-context");
				conceptunmake(ctx);
				throw a(information-needed).link
					(concepts::habit, habit,
					 information, inf);
			}
		} else {
			ctx.link(inf, provided[inf]);
		}
	}
	habit.fun<ref>()(ctx);
	nextinf = infn;
	while (nextinf.linked(next-information)) {
		nextinf = nextinf.get(next-information);
		ref inf = nextinf.get(information);
		if (provided.count(inf)) {
			ctx.unlink(inf, provided[inf]);
		} else {
			ctx.unlink(inf, nextinf.get(assume));
		}
	}
	//for (auto pair : pairs) {
	//	auto second = pair.begin(); ++ second;
	//	ctx.unlink(pair.begin(), second);
	//}
	ref ret = nothing;
	if (ctx.linked(result)) {
		ret = ctx.get(result);
		ctx.unlink(result, ret);
	}
	ref::context() = ctx.get("outer-context");
	conceptunmake(ctx);
	return ret;
}

}
}
