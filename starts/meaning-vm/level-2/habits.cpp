#include "ref.hpp"
#include "../level-1/sugar.hpp"
#include "sugar.hpp"

namespace intellect {
namespace level2 {

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

	// we want the habits expressive enough to code efficiently in.

	// constructors are tentatively abolished in the low-level habit language. (new-type-instance modifies, not creates)
	// we have one constructor of concepts, and knowledge attachment to concepts.
	// 	the purpose of this appears to be conceptual ease, not early development ease
	
	decls(make, unmake, know, concept, is, group, already, in, iter);
	ahabit(make-concept, (),
	{
		//result = a(concept);
		result = intellect::level0::basic_alloc();
	}); 
	ahabit(make-copy, ((concept, c)),
	{
		// copies data too
		result = (make-concept)();
		result.replace(c);
	});
	ahabit(concept-unmake, ((concept, c)),
	{
		intellect::level0::basic_dealloc(c);
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

	using links_it = intellect::level0::baseref<ref>::links_t::iterator;
	decls(populate, entry);
	ahabit(populate-link-entry, ((link-entry, le)),
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
	});
	ahabit(know-is-first-link-entry, ((link-entry, le), (concept, c)),
	{
		if (le.hasval()) { throw (make-concept)().link(
				is, "already-has-value",
				concept, le,
				context, ctx); }
		(know-is)(le, link-entry);
		le.val<links_it>(c.links().begin());
		le.set(source, c);
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(know-is-last-link-entry, ((link-entry, le), (concept, c)),
	{
		if (le.hasval()) { throw (make-concept)().link(
				is, "already-has-value",
				concept, le,
				context, ctx); }
		(know-is)(le, link-entry);
		le.val<links_it>(--c.links().end());
		le.set(source, c);
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(next-link-entry, ((link-entry, le)),
	{
		++le.val<links_it>();
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(previous-link-entry, ((link-entry, le)),
	{
		--le.val<links_it>();
		(populate-link-entry)(le);
		result = le;
	});
	ahabit(same-link-entry, ((link-entry-A, lea), (link-entry-B, leb)),
	{
		return lea.val<links_it>() == leb.val<links_it>();
	});

	// a simple list primitive to aid in dev
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

	// a habit that evaluates a sequence of other habits
	decls(action, map);
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
				(unmake-concept)(subctx2);
			} else {
				h({{context, subctx}, {action, act}});
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
	decls(needed, made, information);
	ahabit(make-context-action, ((needed-information-map, in), (made-information-map, out), (action, act)),
	{
		result = (make-concept)().link(
				habit, context-action,
				needed-information-map, in,
				made-information-map, out,
				action, act);
	});
	ahabit(context-action, ((outer-context, outerctx), (context-action, ca)),
	{

		ref action = ca["action"];
		ref in = ca["needed-information-map"];
		ref out = ca["made-information-map"];

		ref subctx = (make-concept)();
		link(subctx, is, context);
		link(subctx, "outer-context", outerctx);
		(list-each-entry)(in, subctx, context-action-needed-information-iter);
		act(action, subctx);
		(list-each-entry)(out, subctx, context-action-made-information-iter);
		(unmake-concept)(subctx);
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

	// when we make an action list, we want to set(habit, action-list) so it may be used in other lists.
	ahabit(action-list, ((context, subctx), (action, l)),
	{
		(list-each-entry)(l, subctx, action-list-iter);
	});
		ahabit(action-list-iter, ((list-entry, le), ("subcontext", subctx)),
		{
			(action-list-item)(list-entry-item(le), subctx);
		});

	// maybe condition and list could be merged better by factoring out the concept
	// of translating?
	// 	we're going to want to provide lists of subactions to condition
	// 	the irritation around not having the condition be a function is the
	// 	creation of anonymous variable names to hold the values.  is likely fine.
	// 	note: the name "outer-context" is being treated specially.  we'll just use that.
	// 		this name could make a quoting requirement.  could be worked around.
	//
	// 		if we remove outer-context, we'll probably need to recognize condition-
	// 		action in action-list-item, and pass the outer-context to it.
	// 		either way, we can implement condition-action with outer-context as
	// 		a passed value.
	
	ahabit(nothing, (), {});
	link(nothing, habit, nothing);

	// does acts[cond] in outer-context.
	// uses acts[anything] if there is no acts[cond].
	ahabit(condition-action, ((condition, cond), (actions, acts)),
	{
		ref outerctx = linked(ctx, "outer-context") ? ctx["outer-context"] : ctx;
		if (!linked(acts, cond)) {
			if (linked(acts, "anything")) {
				act(acts["anything"], outerctx);
			} else {
				throw (make-concept)().link(
						is, "unknown-condition",
						"condition", cond,
						"actions", acts,
						context, ctx,
						"subcontext", outerctx);
			}
		} else {
			act(acts[cond], outerctx);
		}
	});
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
