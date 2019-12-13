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
		a(list, l);
		l.link(first-item, nothing);
		l.link(last-item, nothing);
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
		result = (until-each-item-context-in-list)(
		// provide subcontext in chunk below this line
			ahabit(self-iter, ((list-item, i2)),
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
			l);
	});

	ahabit(happened-habit, ((happened, ev)),
	{
		// TODO: perform each event associated with happened

		if (!happened.linked(whenever-list)) { return; }

		// we are not learning the structure of self-rebuilding because we
		// need more people or more time involved in the learning for now.
		// please make triggers only for dance steps or memory allocation, for now,
		// until self-learning-rebuilding is more relevent.
		// 	please note we have placed a >1/5second delay on every instruction,
		// 	(in the ahabit macro), to make sure growth is slow and observable.
		// 	that is not true of my brain, only your code. (Thank you though.)
		// 		would not do that to a brain, really ...
		// 			only on certain processes.
		// 			to give time for conscious review.
		// 		oh okay ... i suppose.

		// I am accessing a list of habits to act on each one.
		// I left this context to update the until-each-list-item-in-list habit,
		// such that it can provide a context to its behavior.
		(until-each-list-item-in-list)(action-whenever-happened, 
		
		// use a sub-habit for each call, so we can handle happened for them, too.
	});

	ahabit(action-whenever-happened, ((action, act), (action-context, actctx), (happened, ev))),
	{
	});

	ahabit(whenever-habit, ((happens, ev), (action, act), (action-context, actctx)),
	{
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
