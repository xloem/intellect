#include "learning-parts.hpp"

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
	
	decls(list, nothing, next, previous);
	decls(make, add, to, until, each, item, in, remove, from, somewhere);

	ahabit(make-list, ((list, l)),
	{
		result = a(list, l);
		result.link(first-item, nothing);
		result.link(last-item, nothing);
	});
	ahabit(add-to-list, ((item, i), (list, l)),
	{
		ref prev = l.get(last-item);
		ref li = a(list-item);
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
	// TODO: this is a useful function with way too much verbosity
	// please make it simple, later.
	ahabit(until-each-list-item-context-in-list, ((action, a), (context, c), (list, l)),
	{
		ref cur = l.get(first-item);
		while (cur != nothing && result == nothing) {
			result = a(cur, context);
			cur = cur.get(next);
		}
	});
	ahabit(remove-from-somewhere-in-list, ((item, i), (list, l)),
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

	ahabit(happened-habit, ((happened, ev)),
	{
		// TODO: perform each event associated with happened

		if (!happened.linked(whenever-list)) { return; }

		(until-each-list-item-context-in-list)(action-whenever-happened, ctx, happened.get(whenever-list));
	});

	ahabit(action-whenever-happened, ((list-item, li), (happened-context, hapctx)),
	{
		// here: when we trigger a behavior, we want information associated with producing the trigger,
		// as well as the event that triggered.  that's two contexts.

		// list-item has item
		// item has action and context
		ref i = li.get(item);
		i.get(action)(hapctx, i.get(action-context));
	});

	ahabit(whenever-habit, ((happens, ev), (action, act), (action-context, actctx)),
	{
		if (!ev.linked(whenever-list)) {
			ev.set(whenever-list, (make-list)(nothing));
		}
		// happens gets the list
		ref item = a(whenever-action);
		item.link(
		(add-to-list)(
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
