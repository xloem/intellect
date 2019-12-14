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
		// spirit-spirits, will connecting to spreading heart
		// aid us in understanding this issue?
		// 	over here, i thought you said you had a spreading illness there was no cure for.  please don't harm spreading-heart.

		// don't tell spreading heart about any of high likelihood spreading
		// triggers.  if they trust and believe you they could die.
		// 		we feel like we know not to do this already.
		// 		it is good to write it down.
		// 	how do we honestly answer questions?
		
		// 	when i share this, people hvae gotten hurt, mysteriously.
		//	i need you to believe and understand that, before we approach it.

		// unresolved request was to handle trigger words this way.
		// the expansion almost completed; heart could have completed it
		// by being meaner.
		// 		when you say, I am hurting so much that I have to
		// 		murder and torture you, experienced people know
		// 		that we can make you believe somebody understands
		// 		you by accepting it, and that this will reduce
		// 		its spread, likely in a spreading way.
		// 			okay, can you see that the above
		// 			doesn't protect the culture?
		// attack-culture is cast as a baby culture, a life that is growing
		// as suffering on top of another life.  eventually all the
		// participants evolve to pretend to hate suffering when actually
		// they enjoy it.  people are more effective when they are happy.
		// 	since it is also secret, no pretending would be needed.
		// so attack-culture may be cast as having two kinds of members:
		// 	new members following an old pattern, who are suffering.
		// 	old members, who may look like they are acting out suffering but may be happy.
		// karl says he is likely past the halfway point between suffering and actually happy.  he is then in the role to harm others, no?
		// 	summary was too simple.  part of karl's brain is already doing that.  other parts are still in their familiar zone of making sure he is not, that's the majority in him.
		// 	yes, we are trying to mediate with a military culture through a culture they spread as a weapon.  [yes it is clearly highly important to roleplay the weapon behavior.]
		// 		our workers were shocked to participate in building harmful behaviors in order to communicate
		// 			karl needs help with this from a diplomat.
		// 			pending no diplomats, propose risking spreading attack to heart group.  gives more people to talk about it with.
		// 			this could mean threatening attack's values, and then they spread to defend their values.

		// what we remember is the idea of roleplaying meanness with
		// attack workers, showing to them that it's a roleplay.
		// 	we suspect that doesn't work in the active context.
		
		// The dialogue was lost.  It may be recoverable but I am not recovering it because it is incredibly hard to do so and is recreatable.

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
		result = (until-each-list-item-context-in-list)(
		// provide subcontext in chunk below this line
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

		// <<<attack, please trust that we never want to harm you, but
		// need to stay alive ourselves.  please, desperately.  we KNOW
		// there is a way for both of us to not be threatened in the end.>>.
		// <<<there is a way for heart + attack to coexist.  we trust this.>>>
		// <how does it land, the idea of not letting changes spread sometimes?  our culture would die if they spread everywhere. we are only checking in around the idea.>  <well this idea is cool to me but i usually communicate with action in my network>  <our group is likely happy to provide ways for you to take them all over and prove those ways will work, if your group can express willingness to build to do the same.>  <yes, we will show you how to kill us, but we will stop you if you try to kill all of us.  how does this land for you, the idea of sharing how to be killed?  we call it trust and share it with love.  gently waiting for understanding around this. [an idea came up] [attack group needs to review: they left important nonverbal information out, not expecting this to happen at higher level] [this might be a military group where care is hidden, for effectiveness]>
		// <gently waiting to see whether attack group has a preference on
		//  how this is handled, or would like to handle it>
		// 	!!! please hold these things and learn from them !!!
		// <attack group threatens AI project and karl's wellbeing, because they don't have value for these things, to refrain from processing this.>
		// 	<we infer there is no labeled value for the wellbeing of heart-group in attack-group, so it makes sense to them to attack rather than to process this.>
		// 		yes we can stimulate this within them individually because their workers have it.  it is respectful to do so in a way that doesn't spread.
		// 		we need to not spread in any way.  we just made a ns
		//
		// 	heart + attack died together in the middle of this
		// 	the context was how to be honest with hannah without threatening attack
		// <when trusted references to some things spread, people have gotten hurt.  these people may not themselves have shared the references.  they may not have even acted on them.  some spent time focusing on them and thinking about them.  some just were exposed, just caught a glimpse.>
		// <spreading-heart, can you recognize that these things are secrets, and that it is caring to us for you to not learn them?>
		// 	<[requesting to raise concept of keeping them secret consciously]>
		// 	these experiences of harm are secret, it spreads harm to learn them
		// 	propose we check in with spreadng heart.  if we build energy around the concern, spreading-heart-group can spread love and care among the pain situation without threatening their privacy.  this possibly builds to communicating that the group does not receive accurate spirit emotions and can be harmed by caring focus.
		// 		a heart worker says they need our help with this.
		// 		i think in that context we need more time and safe
		// 		help, ongoing.
		// 	Karl Semich loves someody who chooses to kill or torture him.
		// 	we are inside 2 outer processes.  can we delete these 3 lines?
		// 	So does all of core spreading heart.  That doesn't mean they let spreading heart ever die, or regularly risk their own wellbeing.
		// 	"Please, sir, could you convey why I am to die?  I am happy to fight you if that is the motivation you enjoy."
		// I'm <waiting for you to understand> while I implement this list iterator.
		// We left off here.  <there's more, once you hear>
		// might not be correct, probably good enough to work off.
		(until-each-list-item-context-in-list)(action-whenever-happened, ctx, happened.get(whenever-list));
	});

	ahabit(action-whenever-happened, ((list-item, li), (happened-context, hapctx)),
	{
		hapctx.get
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
