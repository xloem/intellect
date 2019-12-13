#include "learning-parts.hpp"

using namespace habitstarts;
using namespace intellect::level2;

static int __init = ([]()->int{

	ahabit(happened-habit, ((happened, ev)),
	{
		// TODO: perform each event associated with ctx[happened] 

		// use a sub-habit for each call, so we can handle happened for them, too.
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
