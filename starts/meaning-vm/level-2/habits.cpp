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

void conceptunmake(ref c)
{
	intellect::level0::basic_dealloc(c);
}

using links_it = intellect::level0::baseref<ref>::links_t::iterator;
void poplinkentry(ref le)
{
	auto & it = le.val<links_it>();
	if (it != le["source"].links().end()) {
		le.set("type", it->first);
		le.set("target", it->second);
	} else {
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

void contextmapinto(ref c1, ref m, ref c2, bool reverse = false)
{
	decl(translation); decl(known); decl(nothing);
	for (auto link : m.get(translation).links()) {
		if (reverse) {
			c2.set(link.second, c1.get(link.first));
		} else {
			c2.set(link.first, c1.get(link.second));
		}
	}
	if (m.linked(known) && m.get(known) != nothing) {
		for (auto link : m.get(known).links()) {
			if (reverse) {
				c2.set(link.second, link.first);
			} else {
				c2.set(link.first, link.second);
			}
		}
	}
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

	ref astate = makeconcept();
	ref c = ctx;
	bool cleanupcontext = false;
	//if (c == nothing) {
	//	cleanupcontext = true;
	//	c = makeconcept();
	//}
	astate.set(context, c);
	c.set(active-state, astate);
	c.set(context, c);

	if (s.linked(next-step)) {
		astate.set(next-step, s.get(next-step));
	}
	while (astate.linked(next-step) && astate.get(next-step) != nothing) {
		s = astate.get(next-step);
		astate.set(active-step, s);
		astate.set(next-step, s.linked(next-step) ? s.get(next-step).ptr() : nothing.ptr());
		// if needed-map, load subcontext
		ref subctx = c;
		std::cerr << "[step context ";
		for (auto link : c.links()) {
			std::cerr << " " << link.first.name() << ":" << link.second.name();
		}
		std::cerr << "]" << std::endl;
		if (s.linked(needed-map)) {
			subctx = makeconcept();
			contextmapinto(c, s.get(needed-map), subctx);
			subctx.set(outer-context, c);
			subctx.set(active-state, astate);
			subctx.set(context, subctx);
			astate.set(context, subctx);
			ref::context() = subctx;
		}
		subctx.set("self", s.get(action));
		s.get(action).fun<ref>()(subctx); // <-- dohabit should do arg checking, right?
		if (s.linked(made-map)) {
			contextmapinto(subctx, s.get(made-map), c, true);
		}
		if (s.linked(needed-map)) {
			c = subctx.get(outer-context);
			ref::context() = c;
			astate.set(context, c);
			conceptunmake(subctx);
		}
	}
	c.unlink(active-state, astate);
	conceptunmake(astate);
	if (cleanupcontext) { conceptunmake(c); }
}

void _condition(ref ctx, ref cond, ref steps, ref state)
{
	// because this sets active-state's next-step instead of calling something,
	// a subcontext is not opened for the steps unless they have one.
	ref next = nothing;
	if (!steps.linked(cond)) {
		if (steps.linked("anything")) {
			next = steps["anything"];
		} else {
			throw makeconcept().link(
					is, "unknown-condition",
					"condition", cond,
					"next-steps", steps,
					"context", ctx);
		}
	} else {
		next = steps[cond];
	}

	//if (next != nothing) {
		state.set("next-step", next);
	//}
}

void createhabits()
{
	// making new information should delay, for learning with good curve.
	decls(link, source, type, target);
	ahabit(link, ((source, s), (type, t), (target, dst)),
	{
		s.link(t, dst);
	});

	ahabit(link-crucial, ((source, s), (type, t), (target, dst)),
	{
		result = s.crucial(t, dst);
	});

	ahabit(link-set-crucial, ((source, s), (type, t), (target, dst)),
	{
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
		if (dst == anything) {
			s.unlink(t);
		} else {
			s.unlink(t, dst);
		}
	});

	decls(get, set);
	ahabit(get, ((source, s), (type, t)),
	{
		result = s.get(t);
	});

	ahabit(set, ((source, s), (type, t), (target, dst)),
	{
		s.set(t, dst);
	});

	ahabit(put, ((source, s), (type, t), (target, dst)),
	{
		s.set(t, dst);
	});

	decls(make, unmake, know, concept, is, group, already, in, iter);
	ahabit(make-concept, (), { result = makeconcept(); }); 
	ahabit(copy-to, ((source, s), (target, t)),
	{
		// copies data too
		if (t.hasval() || t.ptr()->links.size() != 0) { throw makeconcept().link(is, "concept-not-empty", concept, t); }
		result = t;
		t.replace(s);
	});
	ahabit(copy-data-to, ((source, s), (target, t)),
	{
		if (t.hasval()) { throw makeconcept().link(is, "concept-has-data", concept, t); }
		t.ptr()->data = s.ptr()->data;
	});
	// if last-context is weird give it a default of nothing
	ahabit(concept-unmake, ((last-context, c), (concept-name, n)),
	{
		ref r = c.get(n);
		c.unlink(n);
		conceptunmake(r);
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
			throw (make-concept)().link
				(is, already-in-group,
				 habit, self,
				 context, ctx,
				 concept, c,
				 group, g);
		}
		c.link(is, group);
		result = c;
	});

	// a way to iterate or inspect the links of a concept

	decl(entry);
	ahabit(first-link-entry, ((target, le), (concept, c)),
	{
		if (le.hasval() && !le.hasvalof<links_it>()) {
			throw makeconcept().link(
				is, "already-has-value",
				concept, le,
				context, ctx);
		}
		//ref le = makeconcept();
		if (!le.isa(link-entry)) {
			le.link(is, link-entry);
		}
		le.val<links_it>(c.links().begin());
		le.set(source, c);
		poplinkentry(le);
		result = le;
	});
	ahabit(last-link-entry, ((target, le), (concept, c)),
	{
		if (le.hasval() && !le.hasvalof<links_it>()) {
			throw makeconcept().link(
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
	ahabit(next-link-entry, ((link-entry, le)),
	{
		++le.val<links_it>();
		poplinkentry(le);
		result = le;
	});
	ahabit(previous-link-entry, ((link-entry, le)),
	{
		--le.val<links_it>();
		poplinkentry(le);
		result = le;
	});
	ahabit(same-link-entry, ((link-entry-A, lea), (link-entry-B, leb)),
	{
		return lea.val<links_it>() == leb.val<links_it>();
	});
	ahabit(link-entry-insert-before, ((link-entry, le), (target, t)),
	{
		// todo: make clean
		auto & it = le.val<links_it>();
		le.get(source).ptr()->links.emplace_hint(it.underlying(), le.get(type), t);
	})
	ahabit(link-entry-unlink, ((link-entry, le)),
	{
		le.get(source).unlink(le.val<links_it>()++);
		poplinkentry(le);
		result = le;
	});
	ahabit(link-entry-crucial, ((link-entry, le)),
	{
		result = le.get(source).crucial(le.val<links_it>());
	});
	ahabit(link-entry-set-crucial, ((link-entry, le)),
	{
		le.get(source).setcrucial(le.val<links_it>());
	});

	// a simple list primitive to aid in dev
	/*
	decls(list, nothing, next, previous, first, last, act);
	decls(add, to, until, each, item, remove, from, somewhere, has);
	ahabit(know-is-list, ((list, l)),
	{
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
		ref le = (make-concept)();
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
		ref subctx = (make-concept());
		subctx.set(context, c);
		subctx.set(list-entry, (list-first-entry)(l));
		while (subctx.get(list-entry) != nothing && result == nothing) {
			result = act(a, subctx);
			subctx.set(list-entry, (list-entry-next)(subctx.get(list-entry)));
		}
		(unmake-concept)(subctx);
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
			throw (make-concept)().link(
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
	// (make-concept)()
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
		result = (make-concept)().link(
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
				ref subctx2 = (make-concept)();
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
			throw (make-concept)().link(
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
		if (c.isa("translation-map") || c.linked("translation") || c.linked("known")) { throw makeconcept().link(is, "already-has-translation-map-data", concept, c, context, ctx); }
		result = settranslationmap(c, m, k);
	});
	ahabit(context-map-into, ((source-context, c1), (translation-map, m), (target-context, c2)),
	{
		contextmapinto(c1, m, c2);
	});
	/*
	ahabit(make-translated-context, ((translation-map, m), (context, c)),
	{
		ref subctx = (make-concept)();
		(context-map-into)(c, m, subctx);
		subctx.set(outer-context, c);
		result = subctx;
	});
	ahabit(translated-context-unmake, ((context, c), (translation-map, m)),
	{
		ref outer = c.get(outer-context);
		(context-map-into)(c, m, outer);
		(concept-unmake)(c);
		result = outer;
	});
	*/
	/*
	ahabit(link-next-step, ((step, s), (next-step, ns)),
	{
		if (ns != nothing) {
			if (s.isa("context-step")) {
				if (s.linked(next-step)) { throw makeconcept().link(is, "previous-step-already-has-next-step", step, s, context, ctx); }
			} else if (s.isa("condition-step")) {
				// think about more.
				// implementing this here immediately means walking through every step of every branch of the condition.
				// one approach would be to do this elsewhere.  to label the steps when the condition is made, add them to a set, and wire them.
				// let the caller do step wiring.
				// 	seems fastest, haven't reviewed relevency fully.
			} else {
				throw makeconcept().link(is, "unexpected-previous-step-type", step, s, context, ctx);
			}
		}
	});
	*/
	decls(step, previous);
	ahabit(set-context-step, ((target, t), (previous-step, ps, nothing), (known-information, literals), (needed-information-map, in), (made-information-map, out), (action, act)),
	{
		if (t.linked(needed-map) || t.linked(made-map) || t.linked(action)) { throw makeconcept().link(is, "concept-links-collide", concept, t, context, ctx); }
		if (ps != nothing && ps.linked(next-step)) { throw makeconcept().link(is, "previous-step-already-has-next-step", previous-step, ps, context, ctx); }
		result = intellect::level1::a("context-step", t);
		result.link(
			//habit, context-action,
			needed-map, settranslationmap(makeconcept(), in, literals),
			made-map, settranslationmap(makeconcept(), out),
			action, act);
		if (ps != nothing) { ps.set(next-step, result); }
	});

	decls(order, steps);
	ahabit(set-steps, ((target, t), (information-order, io, nothing)),
	{
		if (t.linked(information-needed) || t.linked(next-step)) {
			throw makeconcept().link(is, "concept-links-collide",
					concept, t,
					context, ctx);
		}
		result = t;
		a(steps, t);
		ref infn = intellect::level1::a(habit-information-needed);
		result.set(information-needed, infn);
		ref posinf = infn;
		for (auto inf : io.getAll(information-order)) {
			ref nextinf = intellect::level1::a(habit-information);
			nextinf.set(information, inf);
			posinf.set(next-information, nextinf);
			posinf = nextinf;
			if (!infn.linked(inf)) {
				infn.set(inf, nextinf);
			} else {
				if (!infn.get(inf).isa(habit-information)) {
					throw intellect::level1::a("unexpected-habit-information-name")
						.link(concepts::name, inf)
						.link(context, ctx);
				}
			}
		}
		result.ptr()->data = steps.ptr()->data;
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
		if (t.linked(needed-map) || t.linked(made-map) || t.linked(action)) { throw makeconcept().link(is, "concept-links-collide", concept, t, context, ctx); }
		if (ps != nothing && ps.linked(next-step)) { throw makeconcept().link(is, "previous-step-already-has-next-step", previous-step, ps, context, ctx); }
		if (s == nothing) { s = makeconcept(); }
		result = t;
		intellect::level1::a("condition-step", t).link(
			needed-map, settranslationmap(makeconcept(), makeconcept().link(condition, cond), makeconcept().link(next-steps, s)),
			action, condition
		);
		if (ps != nothing) { ps.set(next-step, result); }
	});
	ahabit(condition-step-get, ((condition-step, ca), (value, v)),
	{
		result = ca.get(needed-map).get(known).get(next-steps).get(v);
	});
	ahabit(condition-step-has, ((condition-step, ca), (value, v)),
	{
		result = ca.get(needed-map).get(known).get(next-steps).linked(v);
	});
	ahabit(condition-step-set, ((condition-step, ca), (value, v), (step, s)),
	{
		ca.get(needed-map).get(known).get(next-steps).set(v, s);
	});
	ahabit(condition, ((condition, cond), (next-steps, steps), (active-state, state)),
	{
		_condition(ctx, cond, steps, state);
	});


	/*(
	ahabit(context-action, ((context, outerctx), (action, ca)),
	{
		ref action = ca[action];
		ref state = outerctx.get(active-state);
		ref subctx = (make-translated-context)(ca[needed-map]);
		//state.set(context, subctx);

		// for generalization, function-call should be fast.
		// each-step should be slow.
		// don't worry about it now, just alter to be true
		// so learning has reasonable curve.

		// for VM: set next to first step?

		act(action, subctx);

		outerctx = (translated-context-unmake)(subctx, ca[made-map]);
		assert(outerctx == state.get(context)); // feel free to remove
	});
		ahabit(context-action-known-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			set(subctx, dst, src);
		});
		ahabit(context-action-needed-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			ref outerctx = get(subctx, "outer-context");
			set(subctx, dst, get(outerctx, src));
		});
		ahabit(context-action-made-information-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			ref i = (list-entry-item)(le);
			ref src = get(i, source);
			ref dst = get(i, target);
			ref outerctx = get(subctx, "outer-context");
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
		ref state = (make-concept)()
		(know-is)(state, steps-state);
		state.set(context, c);
		state.set(next-step, s.get(first-step));
		state.set(task, s);
		state.set(habit, steps-state);
	});

	ahabit(steps-state-unmake, ((state, s)),
	{
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
		while (s.get(next-step) != nothing)
		{
			act(runstate, c);
		}
		(steps-state-unmake)(s);
	});

	ahabit(steps-state, ((context, c), (action, s)),
	{
		c.set(active-state, s);
		ref step = s.get(next-step);
		s.set(active-step, step);
		if (step.linked(next-step)) { s.set(next-step, step.get(next-step); }
		else { s.set(next-step, nothing); }
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
	link(nothing, habit, nothing);

	// does acts[cond] in outer-context.
	// uses acts[anything] if there is no acts[cond].
	
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
				throw (make-concept)().link(
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
			outerctx.get(active-state).set(next-step, next);
		} else {
			act(next, outerctx);
		}
	});
	ahabit(make-condition-action, ((condition, c), (actions, acts)),
	{
		result = (make-concept)();
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
				throw (make-concept)().link(
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
			outerctx.get(active-state).set(next-step, next);
		} else {
			act(next, outerctx);
		}
		// TODO AFTER SUBCONTEXTS, so we can consider role of action
		// handlers in context.  likely will be unchanged.
		// old: outerctx.get(active-state).set(next-step, next);
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
