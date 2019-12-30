#include "ref.hpp"
#include "../level-1/sugar.hpp"
#include "sugar.hpp"

namespace intellect {
namespace level2 {

ref makeconcept()
{
	//result = a(concept);
	result = intellect::level0::basic_alloc();
}

void conceptunmake(ref c)
{
	intellect::level0::basic_dealloc(c);
}

using links_it = intellect::level0::baseref<ref>::links_t::iterator;
void poplinkentry(ref le)
{
	result = le;
	auto & it = result.val<links_it>();
	if (it != result[source].links().end()) {
		set(result, type, it->first);
		set(result, target, it->second);
	} else {
		unlink(result, type);
		unlink(result, target);
	}
};

void contextmapinto = [](ref c1, ref m, ref c2)
{
	for (auto link : m.get(translation)) {
		c2.set(link.second, c1.get(link.first));
	}
	if (m.linked(known) and m.get(known) != nothing) {
		for (auto link : m.get(known)) {
			c2.set(link.second, link.first);
		}
	}
}

int createhabits()
{
	// making new information should delay, for learning with good curve.
	decls(link, source, type, target);
	ahabit(link, ((source, s), (type, t), (target, dst)),
	{
		s.link(t, dst);
	});

	ahabit(link-crucial, ((souce, s), (type, t), (target, dst)),
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
			return ref(c.ptr()->data.type().name());
		} else {
			return nothing;
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

	decls(make, unmake, know, concept, is, group, already, in, iter);
	ahabit(make-concept, (), { result = makeconcept(); }); 
	ahabit(make-copy, ((concept, c)),
	{
		// copies data too
		result = makeconcept();
		result.replace(c);
	});
	ahabit(copy-data-to, ((source, s), (target, t)),
	{
		t.ptr()->data = s.ptr()->data;
	});
	ahabit(concept-unmake, ((concept, c)), { conceptunmake(c); });
	ahabit(concept-crucial, ((concept, c)),
	{
		result = c.crucial();
	});
	ahabit(concept-set-crucial, ((concept, c)),
	{
		c.setcrucial();
	});

	decls(habit, context);
	ahabit(know-is, ((concept, c), (group, g)),
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

	ahabit(make-first-link-entry, ((concept, c)),
	{
		if (le.hasval()) { throw makeconcept().link(
				is, "already-has-value",
				concept, le,
				context, ctx); }
		ref le = makeconcept().link(is, link-entry);
		le.val<links_it>(c.links().begin());
		le.set(source, c);
		poplinkentry(le);
		result = le;
	});
	ahabit(make-last-link-entry, ((concept, c)),
	{
		if (le.hasval()) { throw makeconcept().link(
				is, "already-has-value",
				concept, le,
				context, ctx); }
		ref le = makeconcept();
		le.link(is, link-entry);
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
	decls(needed, made, known, information);
	// TODO: comment out lists; no longer used
	ahabit(make-translation-map, ((translation-map, m), (known-map, k, nothing)),
	{
		result = makeconcept().link(
			//habit, translation-map,
			translation, m
		);
		if (k != nothing) { result.link(known, k); }
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
	ahabit(make-context-action, ((previous-step, ps), (known-information, literals), (needed-information-map, in), (made-information-map, out), (action, act)),
	{
		if (ps != nothing && ps.linked(next-step)) { throw makeconcept().link(is, previous-step-already-has-next-step, previous-step, ps, context, ctx); }
		result = makeconcept().link(
				//habit, context-action,
				needed-map, (make-translation-map)(needed-information-map, known-information),
				made-map, (make-translation-map)(made-information-map)
				action, act);
		if (ps != nothing) { ps.set(next-step, result); }
	});

	ahabit(make-steps, ((information-order, io, nothing)),
	{
		result = makeconcept();
		ref infn = a(habit-information-needed);
		result.set(information-needed, infn);
		ref posinf = infn;
		for (auto pair : io.getAll(information-order)) {
			ref nextinf = a(habit-information);
			nextinf.set(information, pair.second);
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
		result.ptr()->data = steps.ptr()->data;
	});

	ahabit(steps, (),
	{
		ref s = self;
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

		ref state = makeconcept();
		ref c = ctx;
		bool cleanupcontext = false;
		//if (c == nothing) {
		//	cleanupcontext = true;
		//	c = makeconcept();
		//}
		state.set(context, c);
		c.set(active-state, state);
		c.set(context, c);

		if (s.linked(next-step)) {
			state.set(next-step, s.get(next-step));
		}
		while (state.linked(next-step) && state.get(next-step) != nothing) {
			s = state.get(next-step);
			state.set(active-step, s);
			state.set(next-step, s.linked(next-step) ? s.get(next-step) : nothing);
			// if needed-map, load subcontext
			ref subctx = c;
			if (s.linked(needed-map)) {
				subctx = makeconcept()
				contextmapinto(c, s.get(needed-map), subctx);
				subctx.set(outer-context, c);
				subctx.set(active-state, state);
				subctx.set(context, subctx);
				subctx.set(self, action);
				state.set(context, subctx);
			}
			state.get(action).fun<ref>(subctx); // <-- maybe we should check arguments
				// i think that might exzclude user-defined funcs
				// one moment: do subcontexts work with user-defined funcs?
				// we'll need a reference to our own subcontext to make that work
			if (s.linked(made-map)) {
				contextmapinto(subctx, s.get(made-map), c);
			}
			if (s.linked(needed-map)) {
				c = subctx.get(outer-context);
				conceptunmake(subctx);
			}
		}
		conceptunmake(state);
		if (cleanupcontxt) { conceptunmake(c); }
	});
	// steps must be actual steps, not a list of steps
	ahabit(make-condition-action, ((previous-step, ps), (condition, cond), (steps, s, nothing)),
	{
		if (ps != nothing && ps.linked(next-step)) { throw makeconcept().link(is, previous-step-already-has-next-step, previous-step, ps, context, ctx); }
		if (s == nothing) { s = makeconcept(); }
		result = makeconcept().link(
			needed-map, (make-translation-map)(makeconcept().link(cond, condition), makeconcept().link(next-steps, s)),
			action, condition
		);
		if (ps != nothing) { ps.set(next-step, result); }
	});
	ahabit(condition-action-add, ((condition-action, ca), (value, v), (step, s)),
	{
		ca.get(needed-map).get(known).get(next-steps).set(v, s);
	});
	ahabit(condition, ((condition, cond), (next-steps, steps), (active-state, state)),
	{
		// because this sets active-state's next-step instead of calling something,
		// a subcontext is not opened for the steps unless they have one.
		ref next = nothing;
		if (!linked(steps, cond)) {
			if (linked(steps, "anything")) {
				next = steps["anything"];
			} else {
				throw makeconcept().link(
						is, "unknown-condition",
						"condition", cond,
						"next-steps", steps,
						context, ctx);
			}
		} else {
			next = acts[cond];
		}

		if (next != nothing) {
			state.set(next-step, next);
		}
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
	return 0;
}

void ensurehabitscreated()
{
	static int sentinel = createhabits();
	(void)(sentinel);
}

static int sentinel = (ensurehabitscreated(), 1);

}
}
