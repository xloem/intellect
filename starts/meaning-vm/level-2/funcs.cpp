#include "funcs.hpp"

#include "../level-1/sugar.hpp"
#include "ref.hpp"
#include "concepts.hpp"
#include "habits.hpp"

#include <set>

namespace intellect {
using namespace level1;
namespace level2 {

using namespace concepts;

ref & context()
{
	static thread_local auto ctx = a(concepts::context);
	if (!ctx.linked(concepts::root)) {
		if (ctx.linked("outer-context")) { throw noteconcept().link("is", "context-root-link-missing"); }
		ctx.link(concepts::root, ctx);
	}
	return ctx;
}

ref const & bootstrapnotepad()
{
	static auto notes = ref("bootstrap-space").link("is","notepad");
	return notes;
}

ref & notepad()
{
	// the reason to let vm set notepad is so it can spawn threads in notepads.
	static thread_local auto notes = bootstrapnotepad();
	return notes;
}

ref newnotepad(ref name) 
{
	if (level2::notepad().linked(name)) {
		throw noteconcept().link("is","notepad-already-linked", "notepad", level2::notepad(), "link", name);
	}
		// makeconcept instead of noteconcept because we don't want
		// random things from outer read-only context added
	ref newnotes = makeconcept();
	// any additional links must be blocked in subnotepad()
	newnotes.link("is", "notepad");
	newnotes.link("outer", level2::notepad());
	newnotes.link("name", name.isa("text") ? name : name.get("name"));
	level2::notepad().link(name, newnotes); // linked by name to find easily when used
	return newnotes;
}

ref subnotepad(ref name, bool allowouter)
{
	if (name == "is" || name == "name" || (name == "outer" && !allowouter) || name == "names" || !level2::notepad().linked(name)) {
		throw noteconcept().link("is","subnotepad-does-not-exist", "notepad", level2::notepad(), "subnotepad", name);
	}
	ref result = level2::notepad().get(name);

	// this attempts to defend against other internal links being treated as
	// notepads. assumption is such links would be in bootstrap space and not
	// modifiable by runtime to add the 'is notepad' relationship.
	if (!result.linked("is", "notepad")) { throw noteconcept().link("is","subnotepad-not-notepad", "notepad", level2::notepad(), "subnotepad", result, "name", name); }
	return result;
}

/*
ref outernotepad()
{
	return level2::notepad().get("outer");
}
*/

ref noteconcept(std::any data)
{
	ref result = intellect::level0::basic_alloc(data);
	level2::notepad().link(concepts::changeable, result);
	return result;
}
void checknotepad(ref concept)
{
	ref pad = level2::notepad();
	if (!innotepad(concept, pad)) {
		throw noteconcept().link("is", "concept-not-in-notepad", "concept", concept, "notepad", pad, "context", level2::context());
	}
}

bool innotepad(ref concept, ref pad)
{
	return pad.linked(concepts::changeable,concept) || pad == bootstrapnotepad();
}

void leavenotepad(ref concept, ref pad)
{
	ref check = pad;
	while (check != level2::notepad()) {
		if (!check.linked("outer")) {
			throw noteconcept().link("is", "not-a-subnotepad", "subnotepad", pad, "notepad", level2::notepad(), "context", level2::context());
		}
		check = check.get("outer");
	}
	if (!pad.linked(concepts::changeable,concept)) {
		throw noteconcept().link("is", "concept-not-in-notepad", "concept", concept, "notepad", pad, "context", level2::context());
	}
	pad.unlink(concepts::changeable,concept);
}

void considerwith(ref realitypad, ref imaginationpad)
{
	imaginationpad.link(realitypad, concepts::changing);
}

void givename(ref context, ref concept, std::string const & name, bool contextisnotepad)
{
	ref tname = gettext(name);
	if (!contextisnotepad || context != level2::notepad()) { checknotepad(context); }
	checknotepad(concept);
	ref names;
	if (!context.linked("names")) {
		names = noteconcept();
		context.link("names", names);
	} else {
		names = context.get("names");
	}
	names.set(tname, concept);
	concept.set("name", tname);
}

ref namelookup(std::string name, ref context, ref outerlink, bool makelocal, bool contextisnotepad)
{
	ref tname = gettext(name);
	ref checks = context;
	while (true) {
		for (ref names : checks.getAll("names")) {
			if (names.linked(tname)) { return names.get(tname); }
		}
		if (!checks.linked(outerlink) || makelocal) { break; }
		checks = checks.get(outerlink);
	}
	if (makelocal) {
		ref ret = noteconcept();
		givename(level2::notepad(), ret, name, contextisnotepad);
		return ret;
	} else {
		return level1::getnamed(name);
	}


	//throw noteconcept().link("is","name-not-known","name",tname,"context",context,"outer-link",outerlink);
}

ref getnamed(std::string name, bool create)
{
	return namelookup(name, level2::notepad(), "outer", create, true);
}

/*
void bootstrap2notepad(std::string name)
{
	ref pad = intellect::level2::notepad();
	ref concept = name;
	// a()'s are given level0::concepts::allocations, name, is-group, is-anonymous
	// an alternative approach would be to allocate in our notepad a concept that has the given name
	// (we could also provide a name lookup for the notepad, would be good for parsing)
	
	if (pad.linked(concepts::changeable,concept)) {
		throw noteconcept().link("is", "already-in-notepad", "concept", concept, "notepad", pad, "context", level2::context());
	}
	pad.link(concepts::changeable, concept);
}
*/

void entersubnotepad(ref concept, ref name, bool allowouter)
{
	checknotepad(concept);
	ref pad = subnotepad(name, allowouter);
	if (pad.linked(concepts::changeable,concept)) {
		throw noteconcept().link("is", "already-in-notepad", "concept", concept, "notepad", pad, "context", level2::context());
	}
	pad.link(concepts::changeable, concept);
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
	
	ref order = noteconcept();
	static ref informationneeded("information-needed"), nextinformation("next-information"), informationorder("information-order");
	ref last = habit.get(informationneeded);
	auto & vec = data<vector>(order);
	while (last.linked(nextinformation)) {
		last = last.get(nextinformation);
		vec.push_back(last.get(information));
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
				throw noteconcept()
					.link("is", unexpected-concepts::habit-information-concepts::name)
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
	if (!habit.linked(information-needed)) { throw noteconcept().link("is","not-a-habit"); }
	ref posinf = habit.get(information-needed);
	ref subctx = noteconcept();
	static ref outercontext("outer-context");
	static ref nextinformation("next-information");
	subctx.link(outercontext, ref::context());
	subctx.link(concepts::root, ref::context().get(concepts::root));
	ref::context() = subctx;
	for (ref const & arg : args) {
		if (!posinf.linked(nextinformation)) {
			ref::context() = subctx.get(outercontext);
			conceptunmake(subctx);
			throw noteconcept().link
				(is, unexpected-information,
				 concepts::habit, habit,
				 information-value, arg);
		}
		posinf = posinf[nextinformation];
		// TODO: subcontexts or call instances
		subctx.set(posinf[information], arg);
	}
	while (posinf.linked(nextinformation)) {
		posinf = posinf[nextinformation];
		if (!posinf.linked(assume)) {
			ref::context() = subctx.get(outercontext);
			conceptunmake(subctx);
			throw noteconcept().link
				("is", information-needed,
				 concepts::habit, habit,
				 information, posinf);
		}
		subctx.set(posinf[information], posinf[assume]);
	}
	if (!subctx.linked(self_)) { subctx.link(self_, habit); }
	habit.fun<ref>()(ref::context());
	ref ret = nothing;
	if (ref::context().linked(result)) {
		ret = ref::context().get(result);
		//ref::context().unlink(result, ret);
	}
	//posinf = habit.get(information-needed);
	//while (posinf.linked(nextinformation)) {
	//	posinf = posinf[nextinformation];
	//	ref::context().unlink(posinf[information]);
	//}
	ref::context() = subctx.get(outercontext);
	conceptunmake(subctx);
	return ret;
}

ref dohabit(ref habit, std::initializer_list<std::initializer_list<ref>> pairs, bool extra_information)
{
	using namespace concepts;
	if (!habit.linked(information-needed)) { throw noteconcept().link("is","not-a-habit"); }
	// TODO: subcontexts or call instances
	ref ctx = noteconcept();
	static ref outercontext("outer-context");
	ctx.link(outercontext, ref::context());
	ctx.link(concepts::root, ref::context().get(concepts::root));
	ref::context() = ctx;
	ref infn = habit.get(information-needed);
	std::map<ref, ref> provided;
	for (auto pair : pairs) {
		auto second = pair.begin(); ++ second;
		if (!infn.linked(*pair.begin()) && !extra_information) {
			ref::context() = ctx.get(outercontext);
			conceptunmake(ctx);
			throw noteconcept().link
				("is", unexpected-information,
				 concepts::habit, habit,
				 information, *pair.begin(),
				 information-value, *second);
		}
		if (provided.count(*pair.begin())) {
			ref::context() = ctx.get(outercontext);
			conceptunmake(ctx);
			throw noteconcept().link
				("is", "multiple-instances-same-name-not-implemented",
				 concepts::habit, habit,
				 information, *pair.begin(),
				 information-value, *second);
	       	}
		provided[*pair.begin()] = *second;
	}
	ref nextinf = infn;
	static ref nextinformation("next-information");
	while (nextinf.linked(nextinformation)) {
		nextinf = nextinf.get(nextinformation);
		ref inf = nextinf.get(information);
		if (!provided.count(inf)) {
			if (nextinf.linked(assume)) {
				ctx.link(inf, nextinf.get(assume));
			} else {
				ref::context() = ctx.get(outercontext);
				conceptunmake(ctx);
				throw noteconcept().link
					("is", information-needed,
					 concepts::habit, habit,
					 information, inf);
			}
		} else {
			ctx.link(inf, provided[inf]);
		}
	}
	if (!ctx.linked(self_)) { ctx.link(self_, habit); }
	habit.fun<ref>()(ctx);
	//nextinf = infn;
	//while (nextinf.linked(nextinformation)) {
	//	nextinf = nextinf.get(nextinformation);
	//	ref inf = nextinf.get(information);
	//	if (provided.count(inf)) {
	//		ctx.unlink(inf, provided[inf]);
	//	} else {
	//		ctx.unlink(inf, nextinf.get(assume));
	//	}
	//}

	//for (auto pair : pairs) {
	//	auto second = pair.begin(); ++ second;
	//	ctx.unlink(pair.begin(), second);
	//}
	ref ret = nothing;
	if (ctx.linked(result)) {
		ret = ctx.get(result);
		//ctx.unlink(result, ret);
	}
	ref::context() = ctx.get(outercontext);
	conceptunmake(ctx);
	return ret;
}

using namespace intellect::level0;
void rethrowref()
{
	try {
		throw;
	} catch(level1::ref r) {
		throw level2::ref(r.ptr());
	} catch(level2::ref r) {
		throw;
	} catch(no_such_link_type const & e) {
		throw noteconcept().link(
			"is", "no-such-concept-link-type",
			"source", e.source,
			"type", e.type
			);
	} catch(no_such_link_type_target const & e) {
		throw noteconcept().link(
			"is", "no-such-concept-link-type-and-target",
			"source", e.source,
			"type", e.type,
			"target", e.target
			);
	} catch(crucial_link_type_target const & e) {
		throw noteconcept().link(
			"is", "concept-part-is-crucial",
			"source", e.source,
			"type", e.type,
			"target", e.target
			);
	} catch(crucial_concept const & e) {
		throw noteconcept().link(
			"is", "concept-is-crucial",
			"topic", e.topic
			);
	} catch(link_type_not_unique const & e) {
		throw noteconcept().link(
			"is", "more-than-one-such-concept-link-type",
			"source", e.source,
			"type", e.type
			);
	} catch(still_referenced const & e) {
		throw noteconcept().link(
			"is", "concept-is-still-referenced",
			"topic", e.topic
			);
	} catch(still_referenced_by const & e) {
		throw noteconcept().link(
			"is", "concept-is-still-referenced-by",
			"topic", e.topic,
			"referrer", e.referrer
			);
	} catch(no_such_concept const & e) {
		throw noteconcept().link(
			"is", "no-such-concept-reference",
			"topic", e.topic
			);
	} catch(null_reference const & e) {
		throw noteconcept().link(
			"is", "null-reference"
			);
	} catch(...) {
		throw noteconcept().link(
			"is", "system-exception"
			);
	}
}

}
}
