#include "../level-1/sugar.hpp"
#include "sugar.hpp"

int createhabits()
{
	decls(link, source, type, target);
	ahabit(link, ((source, s), (type, t), (target, dst)),
	{
		s.link(t, dst);
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

	ahabit(copy, ((concept, c)),
	{
		// copies data too
		result = (make-concept)();
		*result.p = *concept.p;
	});

	// we want the habits expressive enough to code efficiently in.

	// constructors are tentatively abolished in the low-level habit language. (new-type-instance modifies, not creates)
	// we have one constructor of concepts, and knowledge attachment to concepts.
	// 	the purpose of this appears to be conceptual ease, not early development ease
	
	decls(make, unmake, know, concept, is, group, already, in);
	ahabit(make-concept, (),
	{
		//result = a(concept);
		result = intellect::level0::basic_alloc();
	}); 
	ahabit(concept-unmake, ((concept, c)),
	{
		intellect::level0::basic_dealloc(c);
	});

	ahabit(know-is, ((concept, c), (group, g)),
	{
		if (c.linked(is, group)) {
			throw (make-concept)().link
				(is, already-in-group,
				 habit, self,
				 "context", ctx,
				 concept, c,
				 group, g);
		}
		c.link(is, group);
		result = c;
	});

	// a way to iterate or inspect the links of a concept

	using links_it = level0::baseref::links_t::iterator;
	ahabit(populate-link-entry, ((link-entry, le)),
	{
		result = le;
		auto & it = result.vget<links_it>();
		if (it != result["source"].links().end()) {
			set(result, "type", it->first);
			set(result, "target", it->second);
		} else {
			unlink(result, "type");
			unlink(result, "target");
		}
	});
	ahabit(know-is-first-link-entry, ((link-entry, le), (concept, c)),
	{
		if (le.hasval()) { throw (make-concept)().link(
				is, "already-has-value",
				concept, le,
				"context", ctx); }
		(know-is)(le, link-entry);
		le.set<link_it>(c.links().begin());
		le.set("source", c);
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(know-is-last-link-entry, ((concept, c)),
	{
		if (le.hasval()) { throw (make-concept)().link(
				is, "already-has-value",
				concept, le,
				"context", ctx); }
		(know-is)(le, link-entry);
		le.set<link_it>(--c.links().end());
		le.set("source", c);
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(next-link-entry, ((link-entry, le)),
	{
		++le.vget<links_it>();
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(previous-link-entry, ((link-entry, le)),
	{
		--le.vget<links_it>();
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(same-link-entry, ((link-entry-A, lea), (link-entry-B, leb)),
	{
		return lea.vget<links_it>() == leb.vget<links_t>();
	});

	// a simple list primitive to aid in dev
	decls(list, nothing, next, previous, first, last, entry);
	decls(add, to, until, each, item, remove, from, somewhere);
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
	});
	ahabit(list-entry-unmake, ((list-entry, le)),
	{
		ref prev = (list-entry-previous)(le);
		ref next = (list-entry-next)(le);
		if (prev != nothing) {
			set(prev, "next", next);
		}
		if (next != nothing) {
			set(next, "previous", prev);
		}
		(concept-unmake)(le);
	});

	ahabit(list-each-entry, ((list, l), (context, c), (action, act)),
	{
		ref cur = (list-first-entry)(l);
		while (cur != nothing && result == nothing) {
			result = act(cur, c);
			cur = (list-next-entry)(cur);
		}
	});
	ahabit(list-has-item, ((list, l), (item, i)),
	{
		result = (list-each-entry)(l, i,
			ahabit(self-iter, ((list-entry, le), (remove-item, i)),
			{
				if ((list-entry-item)(le) == i) { result = true; }
			}));
		if (result == nothing) { result = false; }
	});
	ahabit(list-item-entry-unmake, ((list, l), (item, i)),
	{
		result = (list-each-entry)(l, i,
			ahabit(self-iter, ((list-entry, le), (remove-item, i)),
			{
				if ((list-entry-item)(le) == i) {
					result = true;
					(list-entry-unmake)(le);
				}
			}));
		if (result == nothing) {
			throw (make-concept)().link(
					is, "item-missing",
					item, i,
					list, l,
					"context", ctx
					);
		}
	});

	// make lists as an expression:
	// (make-concept)()
	// 	.act(know-is-list)
	// 	.act(make-next-list-entry, i1)
	// 	.act(make-next-list-entry, i2)
	// 	...;

	// a habit that evaluates a sequence of other habits
	decls(action,map);
	ahabit(make-map-item, ((source, s), (target, d)),
	{
		result = (make-concept)().link(
					source, s,
					target, d);
	});
	ahabit(make-action-list-item, ((needed-information-map, in), (made-information-map, out), (action, act)),
	{
		result = (make-concept)().link(
				needed-information-map, in,
				made-information-map, out,
				action, act);
	});
	ahabit(action-list-item, ((item, i), (outer-context, outerctx)),
	{

		ref action = i["action"];
		ref in = i["needed-information-map"];
		ref out = i["made-information-map"];

		ref subctx = (make-concept)()
		link(subctx, is, context);
		link(subctx, "outer-context", outerctx);
		(list-each-entry)(in, subctx,
				ahabit(self-needed-information-iter, ((list-entry, le), ("subcontext", subctx)),
				{
					ref i = (list-entry-item)(le);
					ref src = get(i, source);
					ref dst = get(i, target);
					ref outerctx = get(subctx, "outer-context");
					set(subctx, dst, get(outerctx, src));
				}));
		// here, let's check if the action is a habit, before doing this
		// also then we can check if it's a behavior, and do something else
		if (linked(action, is, habit) && !linked(action, habit) {
			action.fun<ref>()(subctx);
		} else if (linked(action, habit) && linked(get(action, habit), is, habit)) {
			ref habit = get(action, habit);
			habit({"context", subctx}, {"action", action});
		} else {
			throw (make-concept)().link(
				is, "unknown-action-type",
				"action", action,
				"action-list-item", i
			);
		}
		(list-each-entry)(out, subctx,
				ahabit(self-made-information-iter, ((list-entry, le), ("subcontext", subctx)),
				{
					ref i = (list-entry-item)(le);
					ref src = get(i, source);
					ref dst = get(i, target);
					ref outerctx = get(subctx, "outer-context");
					set(outerctx, dst, get(subctx, src));
				}));
	});

	// when we make an action list, we want to set(action, action-list) so it may be used in other lists.
	ahabit(action-list, ((context, subctx), (action, l)),
	{
		(list-each-entry)(l, subctx,
				ahabit(self-iter, ((list-entry, le), ("subcontext", subctx)),
				{
					(action-list-item)(list-entry-item(le), subctx);
				}));
	});
	
	// TODO: a habit that evaluates habits depending on a condition
	ahabit(condition-action, ((context, ctx), (condition, cond), (actions, a)),
	{
		condition.fun<ref>(ctx);
			// how to pass context to condition within action-list?
			//
			// this function will have one context.
			// propose condition and actions are special.
			// or, we could handle specially in other function
			// 	we want to be able to use values from our context, in the
			// 	habit called.
			// so it's almost like we want two maps
			// 	what would be needed to do this separate subcontext?
			// 		would need more boilerplate for mapping into it
			// 		current map code only places into normal context.
			// 		so, context would get value of local variable,
			// 		as whole context.
			// 	what would we want ideally?
			// 		we want context to be primarily mapped, maybe
			// 			really it can go anyway since we plan to wrap
			// 			with macro
			// 		and then condition and actions have their own special
			// 		mapping.
			// 		
			// 		would like condition and actions to not shadow contents
			// 		of context.
			// 		maybe a tag inside map
			// 		what is also interesting is that inside actions,
			// 		we want the same outer context to the list run.
			//
			// NOTE!! DEAL WITH LATER!! dohabit() uses the thread-local global context.  unlinks locals when done.
		// actions is again a set of pairs
		// index is the result of the condition
		// value is action to do
		// is pretty conducive to just a flat concept here
		// where types are condition-result
		// and targets are action-to-do
	});
}
