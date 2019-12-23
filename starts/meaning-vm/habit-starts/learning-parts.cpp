#include "learning-parts.hpp"

/*
# "How do you think we could show better understanding of the things we are disregarding?"
# "If we do understand these, can you help us?  Do you know who can?"
*/

// idea of learning to keep well having more process time and
// priority than risky behaviors

/*
idea of a secret group attacking a present group, and the attackers being
the only channel to deal with it.
        if we talk, we need nobody to _ever_ know this.  the walls all have ears;
        I was one of them. [from eastern half of continent where a targeted
        activist was living alone]
*/

using namespace habitstarts;
using namespace intellect::level2;

// Propose:
// 	everything that happens is passed to a set of common habits.
// 	these habits categorize, summarize, and pass to relevent habits.
//	high level triggers are thus efficient, because they respond only
//	to the group that applies to them.
//	these habits must be learned.
//	when providing a trigger at a high level, provide a way to get examples
//	of what it should and should not trigger for.  this provides for learning
//	how to do this.
// the above looks like relevence to me.  propose learning it.
//	to learn most effectively, apply to process of learning.
//	how do we adjust from success or from failure?  need some attribute
//	of scenario to store for next time, to respond to differently.
//	so when we do something, we'll want to be able to store all information
//	needed to learn to improve.
// we can include in this the meaning of a concept, and add language translation.
// is this 'apple'?  is this? yes, no.  then pattern recognition could engage
// triggers.  later we'll want to propagate wrongness from failures.
// likely we'll grow better if we use this on things before they have words.
// 	// propose using random or exhaustive trial to find successes until habits develop
// 	// and then using the same on possible structure matches of the data
// 		// it may not work, we'll need to creatively grow data; reaonable start though

static int __init = ([]()->int{

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
			result = s.unlink(t);
		} else {
			result = s.unlink(t, dst);
		}
	});

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
	
	decl(make, know, concept, is, group, already, in);
	ahabit(make-concept, (),
	{
		result = a(concept);
	}); 
	ahabit(know-is, ((concept, c), (group, g)),
	{
		if (c.linked(is, group)) {
			throw an(already-in-group).link
				(habit, self,
				 context, ctx,
				 concept, c,
				 group, g);
		}
		c.link(is, group);
		result = c;
	});

	// separate habits and behaviors.
	// behaviors are modifiable data run hy immutable habits.
	// they use translation maps to move concepts between
	// subhabits.
	// 	translation map is just list of equivalent pairs
	
	// note: lisp can self modify; would need wrapper
	// constructors to make functions and lists into
	// concepts.
	// 	remember can google how to debug lisp
	// opencog does concepts within lisp already, is
	// heavyweight with few habita.  just want goertzel's
	// effort honored, he probably came up with it before
	// I did.
	// 	opencog has functions for pattern matching etc
	// 	they arent self-modifiable, may not matter
	
	decls(ordered, behavior);
	// need args and result for sequence
	//ahabit(habit-sequence, ((
	
	decls(list, nothing, next, previous);
	decls(make, add, to, until, each, item, in, remove, from, somewhere);

	// list functiona are habits because ordered-behavior
	// would use a list
		// lists are being handled by providing a habit that
		// can be engaged for every item.  it responds to the item.
		// i was thinking it could be better to respond to the next-link.
		// 	these are roughly the same thing.
		// when doing an ordered behavior we want to act in response to
		// going to the next step, so we can decide to.
		// this maps to the step list item.  if result is to stop, list
		// stops iteration.
		// 	may want a more meaningful exploration of list. not sure
		// list is mostly the [first-entry, last-entry, next, prev] structure
		// can be handled innumerable ways.
	// LIST STRUCTURE PROMISE
	// should be a promise yandled by habits rather than
	// a bunch of specific habits, but is ok for now
	// 	is likely good for mind to discover
	// 	promises and structures on its own
	ahabit(know-is-list, ((list, l)),
	{
		result = l;
		(know-is)(l, list);
		link(l, first-item, nothing);
		link(l, last-item, nothing);
	});
	ahabit(know-is-list-entry, ((list-entry, l), (item, i), (previous, prev, nothing), (next, n, nothing)),
	{
		result = l;
		(know-is)(l, list-entry);
		link(l, item, i);
		link(l, previous, prev);
		link(l, next, n);
	});
	ahabit(list-first-item, ((list, l)),
	{
		result = get(l, first-item);
	});
	ahabit(list-last-item, ((list, l)),
	{
		result = get(l, last-item);
	});
	ahabit(list-entry-next, ((list-entry, i)),
	{
		result = get(i, next);
	});
	ahabit(list-entry-previous, ((list-entry, i)),
	{
		result = get(i, previous);
	});
	ahabit(list-entry-item, ((list-entry, e)),
	{
		result = get(e, item);
	});

	ahabit(add-to-list, ((item, i), (list, l)),
	{
		ref prev = (list-last-item)(l);
		ref li = (know-is-list-entry)(
				(make-concept)(),
				item,
				nothing,
				prev);
		li.link(item, i);
		li.link(next, nothing);
		li.link(previous, prev);

		if (l.linked(first-item, nothing)) {
			l.set(first-item, li);
			l.set(last-item, li);
		} else {
			ref prev = l.get(last-item);
			l.set(last-item, li);
			prev.set(next, li);
		}
	});
	ahabit(each-list-entry, ((action, a), (context, c), (list, l)),
	{
		ref cur = l.get(first-item);
		while (cur != nothing && result == nothing) {
			result = a(cur, context);
			cur = cur.get(next);
		}
	});
	ahabit(remove-from-list, ((item, i), (list, l)),
	{
		result = (until-each-list-item-context-in-list)(
			ahabit(self-iter, ((list-item, i2), (remove-item, i)),
			{
				if (i2.get(item) == i) {
					result = true
					ref prev = i2.get(previous);
					ref n = i2.get(next);
					if (prev != nothing) {
						prev.set(next, n);
					}
					if (n != nothing) {
						n.set(previous, prev);
					}
					i2.unlink(previous);
					i2.unlink(next);
					i2.unlink(item);
					dealloc(i2); // hmm.  we do have an active goal of making memory allocation be habit based.  this might work here, though.
				}
			}),
			i, l);
	});

	using links_it = level0::baseref::links_t::iterator;
	ahabit(populate-link-entry, ((link-entry, le)),
	{
		result = le;
		result.set("source", c);
		result.set("type", result.vget<links_t>().first);
		result.set("target", result.vget<links_t>().second);
	});
	ahabit(first-link-entry, ((concept, c)),
	{
		result = level1::alloc(level, c.links().begin());
		(populate-link-entry)(result);
	});
	ahabit(last-link-entry, ((concept, c)),
	{
		result = level1::alloc(level, --c.links().end());
		(populate-link-entry)(result);
	});

	ahabit(happened-habit, ((happened, ev)),
	{
		if (!happened.linked(whenever-list)) { return; }

		ref stub = a(event);
		stub.set(event, ev);

		(until-each-list-item-context-in-list)(action-whenever-happened, stub, happened.get(whenever-list));
	});

	ahabit(action-whenever-happened, ((list-item, li), (event, h)),
	{
		// here: when we trigger a behavior, we want information associated with producing the trigger,
		// as well as the event that triggered.  that's two contexts.

		// list-item has item
		// item has action and context
		ref i = li.get(item);
		// i think below we are proposing that handlers
		// take one context, which is the one prepared
		// in the list, then we inject our context
		// into that, inside a "happened" property.

		ref actctx = i.get(action-context);

		actctx.set(happened, h);
 
		i.get(action).fun<ref>()(actctx);
	});

	ahabit(whenever-habit, ((happens, ev), (action, act), (action-context, actctx)),
	{
		if ((action-context).linked(happened)) {
			throw std::logic_error("happened on action-context");
		}
		if (!ev.linked(whenever-list)) {
			ev.set(whenever-list, (make-list)(nothing));
		}
		ref list = ev.get(whenever-list);
		// happens gets the list
		ref item = a(whenever-action);
		item.set(action, act);
		item.set(action-context, actctx);

		(add-to-list)(item, list);
		// store ctx[action] on ctx[happens] as behavior to do
		// store ctx[action-context] as context for behavior
		// PROPOSE: automatically place [happened] inside [action-context] as a stub
		// for call event objects, and then place [context] inside [happened].
		// PROPOSE: report error if [action-context] contains [happened]
		// 		as a stub for error patterns, it would be pretty nice to throw
		// 		a unique concept ref for each error type.  plan to add to level-0.
	});

	ahabit(stop-when-habit, ((action, act), (happens, ev)),
	{
		// remove doing ctx[action] for ctx[happens]
	});

	ahabit(once-habit, ((happens, ev), (action, act), (action-context, actctx)),
	{
		// takes ctx[action] and ctx[happens] and ctx[action-context]
		// uses above habits to do the action only once, probably by using
		// a trigger on the habit-happening habit to check if a label is set,
		// and remove the habit if it is.
	});

	return 0;
})();
