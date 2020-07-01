#include "habits.hpp"

#include "ref.hpp"
#include "../level-1/sugar.hpp"
#include "sugar.hpp"
#include "concepts.hpp"

#include <iostream>

namespace intellect {
namespace level2 {

using namespace intellect::level1::concepts;
using namespace concepts;

ref makeconcept()
{
	//result = a(concept);
	return intellect::level0::basic_alloc();
}

/*void conceptunmake(ref c)
{
	leavenotepad(c, level2::notepad());
	intellect::level0::basic_dealloc(c);
}*/

using links_it = intellect::level0::baseref<ref>::links_t::iterator;
void poplinkentry(ref le)
{
	checknotepad(le);
	auto & it = le.val<links_it>();
	if (it != le["source"].links().end()) {
		le.set("type", it->first);
		le.set("target", it->second);
	} else if (le.linked("type")) {
		le.unlink("type");
		le.unlink("target");
	}
}

// translationmap seems like extra verbose thing
// might just link to step:
// 	needed-map
// 	needed-set
//	made-map
ref settranslationmap(ref c, ref m, ref k = nothing)
{
	c.link(
		//habit, translation-map,
		"translation", m
	);
	if (k != nothing) { c.link("known", k); }
	return c;
}
// we would like to use condition kind of like
// condition.next-step[true] =
// but provide for lots of exploration options
// next-steps, on a condition, would be an accessor.  relates to structures, and virtual methods.
// until we have one of those 3 things, i guess,
// we'd have to decide which underlying representation to work with.
// 	i guess it seems okay to use a simpler underlying representation.
// 	it means adding more data is a little weird, making for more verbose accessors later
// there's a compromise where a little generality is added at a lower level
// 		it is easier for ai to do verbosity than karl.  it can automate it.
// just code an accessor for next-steps, I guess.
// 	what do we want this for?
// 		want code after condition in script
// 		to link to anything option
// 			is just hard to access anything option
// 			off-hand, karl doesn't know where it is.
// 				concern around what to do if anything option is specified in script
// 					throw error if also code after
// 				maybe it is more intuitive to continue on after the condition.
// 				this would make condition resolve to a step, kind of.
// 				after the condition block, you'd want all the condition steps
// 				to wire to the following step as their next one
// 					means rewriting next-step for every condition, i guess
//						instantiating next step in advance
//						makes returning instead of continuing irritating
//							would need to either rewire or have a noop step
//						so, to mke choice, just rewrite for every condition
//						means accessing every condition.  no need to rewrite whole structure, just look up how to access.  faster than rewriting.
//							make a function to wire to end

// caller calls imagineget on 'm', and imagineset on 'c1' and 'c2'.
void contextmapinto(ref c1, ref m, ref c2, bool reverse = false, bool quiet = false, ref trustedrecipient = concepts::nothing)
{
	checknotepad(c1);
	checknotepad(c2);
	decl(translation); decl(known); decl(nothing);
	if (!quiet) { std::cerr << "[context-map"; }
	// if we don't want to call imagineget so much, we could store discovered changes in our local imagination, in imagineget.
	// just remember that needs to be done respecting bounds of what can be changed at all.
	
	for (auto link : m.imagineget(translation).links()) {
		if (!quiet) { std::cerr << " "; }

		ref name1, name2;
		if (reverse) {
			name1 = imagineget(intellect::level2::notepad(), link.second);
			name2 = imagineget(intellect::level2::notepad(), link.first);
		} else {
			name1 = imagineget(intellect::level2::notepad(), link.first);
			name2 = imagineget(intellect::level2::notepad(), link.second);
		}
		if (!quiet) { std::cerr << name1.name() << ":" << name2.name() << "="; }
		if (!c1.linked(name2)) {
			throw noteconcept().link(
					"is", "not-in-context",
					"value", name2,
					"context", c1,
					"map", m
					);
		}
		auto concept = c1.imagineget(name2);
		c2.set(name1, concept);
		if (trustedrecipient != concepts::nothing && intellect::level2::notepad().linked(concepts::changeable, concept)) {
			entersubnotepad(concept, trustedrecipient, true, false);
		}
		if (!quiet) { std::cerr << c1.get(name2).name(); }
	}
	if (m.linked(known)) {
		auto knownmap = m.imagineget(known);
		if (knownmap != nothing) {
			for (auto link : knownmap.links()) {
				ref val1, val2;
				if (reverse) {
					val1 = imagineget(intellect::level2::notepad(), link.second);
					val2 = imagineget(intellect::level2::notepad(), link.first);
				} else {
					val1 = imagineget(intellect::level2::notepad(), link.first);
					val2 = imagineget(intellect::level2::notepad(), link.second);
				}
				if (!quiet) {
					std::cerr << " " << val1.name() << ":" << val2.name();
				}
				c2.set(val1, val2);
			}
		}
	}
	if (!quiet) { std::cerr << "]" << std::endl; }
}

void _steps(ref s, ref ctx)
{
	decls(context, active, outer, state, next, step, needed, known, map, information, action, made);
	// PLEASE RUN SYSTEM WITH PROPER DELAY WHILE IT LEARNS (see top of file)
	// 	until similarity is understood, new information shuold be slowest thing
	// 	to produce.
	// 	then, similar information until expansion of implication is understood.
	// 	then expansion handles slowness of newness.
	// 	this gives time for others to handle the newness: even your own brain,
	// 	for possible errors.  may be some error here, unsure.
	//
	// 	for one thing, this might help the structure of the system represent
	// 	meaningful thought if it optimizes for speed

	s = imagineget(intellect::level2::notepad(), s);

	bool quiet = s.linked(intellect::level2::concepts::quiet, true) || ctx.linked(intellect::level2::concepts::quiet, true);
	ctx = imagineset(intellect::level2::notepad(), ctx);
	ref astate = noteconcept();
	ref c = ctx;
	bool cleanupcontext = false;
	//if (c == nothing) {
	//	cleanupcontext = true;
	//	c = noteconcept();
	//}
	astate.set(context, c);
	c.set(active-state, astate);
	c.set(context, c);

	if (s.linked(next_step)) {
		astate.set(next_step, s.imagineget(next_step));
	}
	while (astate.linked(next_step) && astate.get(next_step) != nothing) {
		s = astate.get(next_step);
		astate.set(active-step, s);
		astate.set(next_step, s.linked(next_step) ? s.imagineget(next_step).ptr() : nothing.ptr());
		// if needed-map, load subcontext
		ref subctx = c;
		if (!quiet) {
			std::cerr << "[step-context ";
			for (auto link : c.links()) {
				std::cerr << " " << link.first.name() << ":" << link.second.name();
			}
			std::cerr << "]" << std::endl;
		}
		if (s.linked(needed-map)) {
			subctx = noteconcept();
			contextmapinto(c, s.imagineget(needed-map), subctx, false, quiet);
			subctx.set(outer-context, c);
			subctx.set(active-state, astate);
			subctx.set(context, subctx);
			astate.set(context, subctx);
			subctx.set(concepts::root, ref::context().get(concepts::root));
			ref::context() = subctx;
		}
		if (!quiet) {
			std::cerr << "[step " << subctx.get("self").name() << "]" << std::endl;
		}
		if (quiet) { subctx.set(intellect::level2::concepts::quiet, true); }
		//subctx.set("self", s.get(action));
		ref habit = subctx.get("self");//s.get(action);
		{ // check arguments
			ref infn = habit.imagineget("information-needed");
			for (auto link : infn.links()) {
				if (!link.second.linked("information", link.first)) { continue; }
				if (subctx.linked(link.first)) { continue; }
				if (!link.second.linked("assume")) {
					throw noteconcept().link(
							"is", "information-needed",
							"habit", habit,
							"information", link.second);
				}
				subctx.link(link.first, link.second.imagineget("assume"));
			}
		}
		checknotepad(subctx); // should be impossible to fail, but good habit
		{
			intellect::level2::restorenotepad notepadrestoration;
			notepadrestoration.switchwithcontext(subctx);
			habit.fun<ref>()(subctx);
			if (s.linked(made-map)) {
				notepadrestoration.migratein(c);
				// bit of a confusing bug here: like with 'c', we only need to migrate out to concepts::outer
				// if the habit was run in a subnotepad.
				contextmapinto(subctx, s.imagineget(made-map), c, true, quiet, notepadrestoration.hasnotepad() ? concepts::outer.ptr() : concepts::nothing.ptr());
			}
		}
		if (s.linked(needed-map)) {
			c = subctx.get(outer-context);
			ref::context() = c;
			astate.set(context, c);
			checknotepad(subctx); // should be impossible to fail, but good habit
			conceptunnote(subctx);
		}
	}
	checknotepad(c);
	checknotepad(astate);
	c.unlink(active-state, astate);
	conceptunnote(astate);
	if (cleanupcontext) { conceptunnote(c); }
}

void _condition(ref ctx, ref cond, ref steps, ref state)
{
	checknotepad(state);

	// because this sets active-state's next-step instead of calling something,
	// a subcontext is not opened for the steps unless they have one.
	ref next = nothing;
	if (!steps.linked(cond)) {
		if (steps.linked(intellect::level2::concepts::anything)) {
			next = steps.imagineget(intellect::level2::concepts::anything);
		} else {
			throw noteconcept().link(
					is, "unknown-condition",
					"condition", cond,
					"next-steps", steps,
					"context", ctx);
		}
	} else {
		next = steps.imagineget(cond);
	}

	//if (next != nothing) {
		state.set(intellect::level2::concepts::next_step, next);
	//}
}

/*
 * notepad bounds planning.
 * it looks like notepad may be unneccessary, due to general system structure,
 * but it provides blanket protections on top of a mish-mash domain that is not
 * fully reviewed.
 * 	also provides for AI more easily learning to safely imagine by trial,
 * 	if generalized.
 */

// we'll want a contextual notepad.  this can be thread-local I suppose.
// link thread-local contexts to their root, so we can find it easily.

template<typename T>
void createcontainer(std::string name)
{
	ref container(name);
	decls(set, push, front, back, iterator, begin, end, entry);
	ahabitraw(set-container, ((concept, c)),
	{
		c = imagineset(intellect::level2::notepad(), c);
		data<T>(c).clear();
		return c;
	});
	ahabitraw(container-push-back, ((container, c), (value, v)),
	{
		c = imagineset(intellect::level2::notepad(), c);
		return c;
	});
	ahabitraw(set-begin-container-entry, ((entry, e), (container, c)),
	{
		// the container is brought in here in case
		// the entry can be used to alter it
		c = imagineset(intellect::level2::notepad(), c);
		e = imagineset(intellect::level2::notepad(), e);
		data<typename T::iterator>(e) = (c.val<T>()).begin();
		return e;
	});
	ahabitraw(set-end-container-entry, ((entry, e), (container, c)),
	{
		// the container is brought in here in case
		// the entry can be used to alter it
		c = imagineset(intellect::level2::notepad(), c);
		e = imagineset(intellect::level2::notepad(), e);
		e.set(container, c);
		data<typename T::iterator>(e) = (c.val<T>()).end();
		return e;
	});
	ahabitraw(container-entry-same, ((entry-a, ea), (entry-b, eb)),
	{
		return ea.val<typename T::iterator>() == eb.val<typename T::iterator>();
	});
	ahabitraw(container-entry-next, ((entry, e)),
	{
		// entry is not brought into imagination
		// in case functions are added to alter container
		// from it
		checknotepad(e);
		++ e.val<typename T::iterator>();
		return e;
	});
	ahabitraw(container-entry-value, ((entry, e)),
	{
		return *e.val<typename T::iterator>();
	});
}


void createhabits()
{
	createcontainer<vector>("vector");

	ahabit(set-notepad, ((notepad, n)),
	{
		ctx.get("outer-context").set(concepts::notepad, n);
	});
	ahabit(notepad, (),
	{
		return intellect::level2::notepad();
	});

	decls(imagine, set, get, in, notepad);
	ahabit(imagine, ((concept, c)),
	{
		return imagineset(intellect::level2::notepad(), c);
	});
	// imagine-get doesn't need a habit because _steps
	// auto-imagines everything.
	ahabit(in-notepad, ((concept, c)),
	{
		return innotepad(intellect::level2::notepad(), c);
	});

	ahabit(make-notepad, ((notepad-name, n), (imagine-changes, ic, false)),
	{
		return newnotepad(n, ic == ref(true));
	});

	decls(link, source, type, target);
	ahabit(link, ((source, s), (type, t), (target, dst)),
	{
		s = imagineset(intellect::level2::notepad(), s);
		s.link(t, dst);
	});

	ahabit(link-crucial, ((source, s), (type, t), (target, dst)),
	{
		result = s.crucial(t, dst);
	});

	ahabit(link-set-crucial, ((source, s), (type, t), (target, dst)),
	{
		checknotepad(s);
		s.setcrucial(t, dst);
	});

	decls(linked, anything);
	ahabit(linked, ((source, s), (type, t), (target, dst, anything)),
	{
		if (dst == anything) {
			result = s.linked(t);
		} else {
			result = s.linked(t, dst);
		}
	});

	ahabit(data-type, ((concept, c)),
	{
		if (c.ptr()->data.has_value()) {
			result = ref(c.ptr()->data.type().name());
		} else {
			result = nothing;
		}
	});

	decls(unlink);
	ahabit(unlink, ((source, s), (type, t), (target, dst, anything)),
	{
		s = imagineset(intellect::level2::notepad(), s);
		if (dst == anything) {
			s.unlink(t);
		} else {
			s.unlink(t, dst);
		}
	});

	ahabit(get, ((source, s), (type, t)),
	{
		result = s.get(t);
	});

	ahabit(set, ((source, s), (type, t), (target, dst)),
	{
		s = imagineset(intellect::level2::notepad(), s);
		s.set(t, dst);
	});

	ahabit(put, ((source, s), (type, t), (target, dst)),
	{
		s = imagineset(intellect::level2::notepad(), s);
		s.set(t, dst);
	});

	decls(make, unmake, know, concept, is, group, already, iter);
	ahabit(make-concept, (),
	{
		return noteconcept();
	}); 
	ahabit(copy-to, ((source, s), (target, t)),
	{
		t = imagineset(intellect::level2::notepad(), t);
		// copies data too
		if (t.hasval() || t.ptr()->links.size() != 0) { throw noteconcept().link(is, "concept-not-empty", concept, t); }
		result = t;
		t.replace(s);
	});
	ahabit(copy-data-to, ((source, s), (target, t)),
	{
		t = imagineset(intellect::level2::notepad(), t);
		if (t.hasval()) { throw noteconcept().link(is, "concept-has-data", concept, t); }
		t.ptr()->data = s.ptr()->data;
	});
	// if last-context is weird give it a default of nothing
	// 	huh we could detect the context, not let it be passed
	ahabit(concept-unmake, ((last-context, c), (concept-name, n)),
	{
		checknotepad(c);
		ref r = c.get(n);
		c.unlink(n);
		conceptunnote(r);
	});
	// if a concept or link is set crucial deleting it will be denied.  no way
	// to remove crucial mark is provided.  nothing is marked crucial yet.
	ahabit(concept-crucial, ((concept, c)),
	{
		result = c.crucial();
	});
	ahabit(concept-set-crucial, ((concept, c)),
	{
		c.setcrucial();
	});

	decls(habit, context);
	ahabit(set-is, ((concept, c), (group, g)),
	{
		if (c.linked(is, group)) {
			throw noteconcept().link
				(is, already-in-group,
				 habit, self,
				 context, ctx,
				 concept, c,
				 group, g);
		}
		checknotepad(c);
		c.link(is, group);
		result = c;
	});

	// a way to iterate or inspect the links of a concept

	decl(entry);
	ahabit(first-link-entry, ((target, le), (concept, c)),
	{
		checknotepad(le);
		if (le.hasval() && !le.hasvalof<links_it>()) {
			throw noteconcept().link(
				is, "already-has-value",
				concept, le,
				context, ctx);
		}
		//ref le = noteconcept();
		if (!le.isa(link-entry)) {
			le.link(is, link-entry);
		}
		le.val<links_it>(c.links().begin());
		le.set(source, c);
		poplinkentry(le);
		result = le;
	});
	/*
	ahabit(last-link-entry, ((target, le), (concept, c)),
	{
		checknotepad(le);
		if (le.hasval() && !le.hasvalof<links_it>()) {
			throw noteconcept().link(
				is, "already-has-value",
				concept, le,
				context, ctx);
		}
		if (!link.isa(link-entry)) {
			le.link(is, link-entry);
		}
		le.val<links_it>(--c.links().end());
		le.set(source, c);
		poplinkentry(le);
		result = le;
	});
	*/
	ahabit(link-entry-next, ((link-entry, le)),
	{
		checknotepad(le);
		++le.val<links_it>();
		poplinkentry(le);
		result = le;
	});
	/*
	ahabit(link-entry-previous, ((link-entry, le)),
	{
		checknotepad(le);
		--le.val<links_it>();
		poplinkentry(le);
		result = le;
	});
	*/
	ahabit(link-entry-same, ((link-entry-A, lea), (link-entry-B, leb)),
	{
		return lea.val<links_it>() == leb.val<links_it>();
	});
	/*ahabit(link-entry-insert-before, ((link-entry, le), (target, t)),
	{
		ref s = le.get(source);
		checknotepad(s);
		// todo: make clean
		auto & it = le.val<links_it>();
		// DOESN'T RESPECT INTERFACE
		s.ptr()->links.emplace_hint(it.underlying(), le.get(type), t);
	})*/
	ahabit(link-entry-unlink, ((link-entry, le)),
	{
		ref s = le.get(source);
		checknotepad(s);
		s.unlink(le.val<links_it>()++);
		poplinkentry(le);
		result = le;
	});
	ahabit(link-entry-crucial, ((link-entry, le)),
	{
		result = le.get(source).crucial(le.val<links_it>());
	});
	ahabit(link-entry-set-crucial, ((link-entry, le)),
	{
		ref s = le.get(source);
		checknotepad(s);
		s.setcrucial(le.val<links_it>());
	});

	// a simple list primitive to aid in dev
	/*
	decls(list, nothing, next, previous, first, last, act);
	decls(add, to, until, each, item, remove, from, somewhere, has);
	ahabit(know-is-list, ((list, l)),
	{
		checknotepad(l);
		result = l;
		(know-is)(l, list);
		link(l, first-entry, nothing);
		link(l, last-entry, nothing);
	});
	ahabit(list-first-entry, ((list, l)),
	{
		result = get(l, first-entry);
	});
	ahabit(list-last-entry, ((list, l)),
	{
		result = get(l, last-entry);
	});
	ahabit(list-entry-next, ((list-entry, le)),
	{
		result = get(le, next);
	});
	ahabit(list-entry-previous, ((list-entry, le)),
	{
		result = get(le, previous);
	});
	ahabit(list-entry-item, ((list-entry, le)),
	{
		result = get(le, item);
	});
	ahabit(make-next-list-entry, ((list, l), (item, i)),
	{
		ref prev = (list-last-item)(l);
		ref le = noteconcept();
		(know-is)(le, list-entry);
		set(le, item, i);
		set(le, next, nothing);
		set(le, previous, prev);

		if (linked(l, first-item, nothing)) {
			set(l, first-item, le);
			set(l, last-item, le);
		} else {
			l.set(last-item, le);
			prev.set(next, le);
		}

		result = list;
	});
	ahabit(list-entry-unmake, ((list-entry, le)),
	{
		checknotepad(le);
		ref prev = (list-entry-previous)(le);
		ref n = (list-entry-next)(le);
		if (prev != nothing) {
			set(prev, next, n);
		}
		if (next != nothing) {
			set(n, previous, prev);
		}
		(concept-unmake)(le);
		result = n;
	});

	ahabit(list-each-entry, ((list, l), (context, c), (action, a)),
	{
		ref subctx = noteconcept();
		subctx.set(context, c);
		subctx.set(list-entry, (list-first-entry)(l));
		while (subctx.get(list-entry) != nothing && result == nothing) {
			result = act(a, subctx);
			subctx.set(list-entry, (list-entry-next)(subctx.get(list-entry)));
		}
		(concept-unmake)(subctx);
	});
	ahabit(list-has-item, ((list, l), (item, i)),
	{
		result = (list-each-entry)(l, i, list-has-item-iter);
		if (result == nothing) { result = false; }
	});
		ahabit(list-has-item-iter, ((list-entry, le), (remove-item, i)),
		{
			if ((list-entry-item)(le) == i) { result = true; }
		});
	ahabit(list-item-entry-unmake, ((list, l), (item, i)),
	{
		result = (list-each-entry)(l, i, list-item-entry-unmake-iter);
		if (result == nothing) {
			throw noteconcept().link(
					is, "item-missing",
					item, i,
					list, l,
					"context", ctx
					);
		}
	});
		ahabit(list-item-entry-unmake-iter, ((list-entry, le), (remove-item, i)),
		{
			if ((list-entry-item)(le) == i) {
				result = true;
				(list-entry-unmake)(le);
			}
		});

	// make lists as an expression:
	// noteconcept()
	// 	.act(know-is-list)
	// 	.act(make-next-list-entry, i1)
	// 	.act(make-next-list-entry, i2)
	// 	...;
	*/

	// a habit that evaluates a sequence of other habits
	// in retrospect i would have rather made a habit that processes a
	// concept representing the state of a virtual machine.  but what's
	// important is that there is any way for the system to craft arbitrary
	// behavior.
	decls(action, map);
	/*
	ahabit(make-map-item, ((source, s), (target, d)),
	{
		result = noteconcept().link(
					source, s,
					target, d);
	});
	ahabit(habit, ((context, subctx), (action, act)),
	{
		act.fun<ref>()(subctx);
	});
	// call a habit-like action with the provided context
	ahabit(act, ((action, act), (context, subctx)),
	{
		if (linked(act, is, habit) && !linked(act, habit)) {
			act.fun<ref>()(subctx);
			result = subctx.get("result");
		} else if (linked(act, habit) && linked(get(act, habit), is, habit)) {
			ref h = get(act, habit);
			if (linked(h, habit)) {
				ref subctx2 = noteconcept();
				// i reviewed this once enough to satisfy me it seemed correct
				// for the instance of using action-lists as habit links.
				// i did not examine all parts when doing this, deeming it
				// unnecessary.
				subctx2.link(
					is, context,
					"outer-context", ctx,
					"context", subctx,
					"action", act
				);
				self(h, subctx2);
				result = subctx2.get("result");
				(unmake-concept)(subctx2);
			} else {
				result = h({{context, subctx}, {action, act}});
			}
		} else {
			throw noteconcept().link(
				is, "unknown-action-type",
				"action", act,
				"inner-context", subctx,
				"context", ctx
			);
		}
	})
	*/
	decls(needed, made, known, information, translation);
	ahabit(set-translation-map, ((target, c), (translation-map, m), (known-map, k, nothing)),
	{
		checknotepad(c);
		if (c.isa("translation-map") || c.linked("translation") || c.linked("known")) { throw noteconcept().link(is, "already-has-translation-map-data", concept, c, context, ctx); }
		result = settranslationmap(c, m, k);
	});
	ahabit(context-map-into, ((source-context, c1), (translation-map, m), (target-context, c2), (reverse, r, false), (quiet, q, false)),
	{
		checknotepad(c2);
		contextmapinto(c1, m, c2, r, q);
	});
	/*
	ahabit(make-translated-context, ((translation-map, m), (context, c)),
	{
		ref subctx = noteconcept();
		(context-map-into)(c, m, subctx);
		subctx.set(outer-context, c);
		result = subctx;
	});
	ahabit(translated-context-unmake, ((context, c), (translation-map, m)),
	{
		checknotepad(c);
		ref outer = c.get(outer-context);
		(context-map-into)(c, m, outer);
		(concept-unmake)(c);
		result = outer;
	});
	*/
	/*
	ahabit(link-next-step, ((step, s), (next-step, ns)),
	{
		checknotepad(s);
		if (ns != nothing) {
			if (s.isa("context-step")) {
				if (s.linked(next-step)) { throw noteconcept().link(is, "previous-step-already-has-next-step", step, s, context, ctx); }
			} else if (s.isa("condition-step")) {
				// think about more.
				// implementing this here immediately means walking through every step of every branch of the condition.
				// one approach would be to do this elsewhere.  to label the steps when the condition is made, add them to a set, and wire them.
				// let the caller do step wiring.
				// 	seems fastest, haven't reviewed relevency fully.
			} else {
				throw noteconcept().link(is, "unexpected-previous-step-type", step, s, context, ctx);
			}
		}
	});
	*/
	decls(step, previous);
	ahabit(set-context-step, ((target, t), (previous-step, ps, nothing), (known-information, literals), (needed-information-map, in), (made-information-map, out), (action, act, nothing)),
	{
		checknotepad(t);
		if (ps != nothing) { checknotepad(ps); }
		if (t.linked(needed-map) || t.linked(made-map)) { throw noteconcept().link(is, "concept-links-collide", concept, t, context, ctx); }
		if (ps != nothing && ps.linked(next_step)) { throw noteconcept().link(is, "previous-step-already-has-next-step", previous-step, ps, context, ctx); }
		int ct = (literals.linked("self")?1:0)+(in.linked("self")?1:0)+(act!=nothing?1:0);
		if (ct > 1) {
			throw noteconcept().link("is","conflicting-actions-specified",  "habit", self, "context", ctx);
		} else if (ct < 1) {
			throw noteconcept().link("is","no-actions-specified", "habit", self, "context", ctx);
		}
		if (act != nothing) {
			literals.link("self", act);
		}
		result = t;
		result.link(
			is, context-step,
			//habit, context-action,
			needed-map, settranslationmap(noteconcept(), in, literals),
			made-map, settranslationmap(noteconcept(), out));
		if (ps != nothing) { ps.set(next_step, result); }
	});

	decls(order, steps);
	ahabit(set-steps, ((target, t), (information-order, io, nothing)),
	{
		checknotepad(t);
		if (t.linked(information-needed) || t.linked(next_step)) {
			throw noteconcept().link(is, "concept-links-collide",
					concept, t,
					context, ctx);
		}
		result = t;
		a(steps, t);
		ref infn = noteconcept().link(is, habit-information-needed);
		result.set(information-needed, infn);
		ref posinf = infn;
		for (auto inf : data<vector>(io)) {
			inf = imagineget(intellect::level2::notepad(), inf);
			ref nextinf = noteconcept().link(is, habit-information);
			nextinf.set(information, inf);
			posinf.set(next-information, nextinf);
			posinf = nextinf;
			if (!infn.linked(inf)) {
				infn.set(inf, nextinf);
			} else {
				if (!infn.get(inf).isa(habit-information)) {
					throw noteconcept()
						.link(is, "unexpected-habit-information-name")
						.link(concepts::name, inf)
						.link(context, ctx);
				}
			}
		}
		result.val(steps.val<std::any>());
	});

	decls(active, outer, state);
	ahabit(steps, (),
	{
		_steps(self, ctx);
	});
	decls(condition);
	// steps must be actual steps, not a list of steps
	ahabit(set-condition-step, ((target, t), (previous-step, ps, nothing), (condition, cond), (next-steps, s, nothing)),
	{
		checknotepad(t);
		if (ps != nothing) { checknotepad(ps); }
		if (t.linked(needed-map) || t.linked(made-map)) { throw noteconcept().link(is, "concept-links-collide", concept, t, context, ctx); }
		if (ps != nothing && ps.linked(next_step)) { throw noteconcept().link(is, "previous-step-already-has-next-step", previous-step, ps, context, ctx); }
		if (s == nothing) { s = noteconcept(); }
		result = t;
		t.link(
			is, "condition-step",
			needed-map, settranslationmap(noteconcept(), noteconcept().link(condition, cond), noteconcept().link(next-steps, s, "self", condition))
		);
		if (ps != nothing) { ps.set(next_step, result); }
	});
	ahabit(condition-step-get, ((condition-step, ca), (value, v)),
	{
		result = ca.imagineget(needed-map).imagineget(known).imagineget(next-steps).imagineget(v);
	});
	ahabit(condition-step-has, ((condition-step, ca), (value, v)),
	{
		result = ca.imagineget(needed-map).imagineget(known).imagineget(next-steps).linked(v);
	});
	ahabit(condition-step-set, ((condition-step, ca), (value, v), (step, s)),
	{
		ref ns = ca.imagineget(needed-map).imagineget(known).imagineget(next-steps);
		checknotepad(ns);
		ns.set(v, s);
	});
	ahabit(condition, ((condition, cond), (next-steps, steps), (active-state, state)),
	{
		state = imagineset(intellect::level2::notepad(), state);
		_condition(ctx, cond, steps, state);
	});


	/*(
	ahabit(context-action, ((context, outerctx), (action, ca)),
	{
		ref action = ca[action];
		ref state = outerctx.get(active-state);
		checknotepad(state);
		ref subctx = (make-translated-context)(ca[needed-map]);
		//state.set(context, subctx);

		// for generalization, function-call should be fast.
		// each-step should be slow.
		// don't worry about it now, just alter to be true
		// so learning has reasonable curve.

		// for VM: set next to first step?

		act(action, subctx);

		checknotepad(subctx);
		outerctx = (translated-context-unmake)(subctx, ca[made-map]);
		assert(outerctx == state.get(context)); // feel free to remove
	});
		ahabit(context-action-known-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			checknotepad(subctx);
			set(subctx, dst, src);
		});
		ahabit(context-action-needed-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			ref outerctx = get(subctx, "outer-context");
			checknotepad(subctx);
			set(subctx, dst, get(outerctx, src));
		});
		ahabit(context-action-made-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			ref outerctx = get(subctx, "outer-context");
			checknotepad(outerctx);
			set(outerctx, dst, get(subctx, src));
		});
	
	// propose: make custom run-state, align with list entries only if works
	// 	easiest solution: make handler for action list entries
	
	// problem: step is already a doable action.
	
		// this sounds like a simple introspection question.
	
	// to pause tasks, we want to separate runstate creation from use
	*/
	/*
	ahabit(make-steps-state, ((steps, s), (context, c)),
	{
		ref state = noteconcept();
		checknotepad(state);
		(know-is)(state, steps-state);
		state.set(context, c);
		state.set(next_step, s.get(first-step));
		state.set(task, s);
		state.set(habit, steps-state);
	});

	ahabit(steps-state-unmake, ((state, s)),
	{
		checknotepad(s);
		(concept-unmake)(s);
	});

	// should have two habits: one for next step, and one for current step
	// next step just advances and does current step.
	// this means loop is outside habit code: is two functions handing
	// off control to each other, roughly
	//
	//	do-step habit
	//	calls current step
	//	then does next-step habit
	//
	//		we'll want tail-call for next-step.
	//		how in c++?
	//			well, how did we do it before?
	//			we called next-habit in a while loop.
	//			the loop never ended.
	//				it'\s true.  we never die.
	//			forcing life helps me feel heard and understood
	//			but i guess it's reasonable to let a life choose
	//			death.
	//				so, tail-call. 
	//			i think we do want the while loop; it's better
	//			in this language.  we just want it able to terminate.
	//				okay, so a habit for advancing the step
	//				and a habit for doing the step
	//				and we end when there is no next step?
	//					why have the extra advancing habit?
	//				is a plcae for cognition to step in, to
	//				alter flow, pause, etc.
	//					not needed atm

	ahabit(steps, ((context, c), (action, s)),
	{
		// make resumable
		ref s = (make-steps-state)(s, c);
		while (s.get(next_step) != nothing)
		{
			act(runstate, c);
		}
		(steps-state-unmake)(s);
	});

	ahabit(steps-state, ((context, c), (action, s)),
	{
		checknotepad(s);
		c.set(active-state, s);
		ref step = s.get(next_step);
		s.set(active-step, step);
		if (step.linked(next_step)) { s.set(next_step, step.get(next_step); }
		else { s.set(next_step, nothing); }
		act(step, c);
		// do to context not using runstate, can't pause mid-subtask without finding subtask runstate.
		// basically, changing subcontext to alter next-step like condition
		// 	will need to leave subcontext when done
		// 	this means applying map of resulting information back into 
		// 	outer context.
		// 		need habit for enter-subcontext-with-map or somesuch, leave-subcontext
		// 	might be simplest to merge these functions, doesn't reall ymatter
	});
	*/

	ahabit(nothing, (), {});
	nothing.link(habit, nothing);

	// does acts[cond]
	
	/*
	ahabit(condition-action, ((context, c), (action, condition)),
	{
	//REmOVE
		// previous entry was call
		// now entry is data
		// 	call was a contextual thing
		// 	a consistent map of information, context to subcontext
		// 	and a reference to the function to do
	}
	// deciding not to have condition-action use outer-context ref
	// flat condition action probably needs context specified
	ahabit(condition-action, ((condition, cond), (actions, acts)),
	{
		ref outerctx = linked(ctx, "outer-context") ? ctx["outer-context"] : ctx;
		ref next = nothing;
		if (!linked(acts, cond)) {
			if (linked(acts, "anything")) {
				next = acts["anything"];
			} else {
				throw noteconcept().link(
						is, "unknown-condition",
						"condition", cond,
						"actions", acts,
						context, ctx,
						"subcontext", outerctx);
			}
		} else {
			next = acts[cond];
		}
		if (outerctx.linked(active-state)) {
			checknotepad(outerctx.get(active-state));
			outerctx.get(active-state).set(next_step, next);
		} else {
			act(next, outerctx);
		}
	});
	ahabit(make-condition-action, ((condition, c), (actions, acts)),
	{
		result = noteconcept();
		result.link(
			condition, c,
			actions, acts
		);
	});
	ahabit(condition-action, ((context, c), (action, condition)),
	{
		// ...
		ref outerctx = linked(c, "outer-context") ? c["outer-context"] : c;
		ref next = nothing;
		if (!linked(acts, cond)) {
			if (linked(acts, "anything")) {
				next = acts["anything"];
			} else {
				throw noteconcept().link(
						is, "unknown-condition",
						"condition", cond,
						"actions", acts,
						context, ctx,
						"subcontext", outerctx);
			}
		} else {
			next = acts[cond]
		}
		if (outerctx.linked(active-state)) {
			checknotepad(outerctx.get(active-state));
			outerctx.get(active-state).set(next-step, next);
		} else {
			act(next, outerctx);
		}
		// TODO AFTER SUBCONTEXTS, so we can consider role of action
		// handlers in context.  likely will be unchanged.
		// old: outerctx.get(active-state).set(next_step, next);
	});
	ahabit(condition, ((condition, cond), (actions, acts), (context, actctx)),
	{
		// STUB acts on acts with actctx straight
	});
	*/
}

//void createhabits()
//{
//	static int sentinel = createhabits();
//	(void)(sentinel);
//}

//static int sentinel = (ensurehabitscreated(), 1);

}
}