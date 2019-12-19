#include "learning-parts.hpp"

/*
# "How do you think we could show better understanding of the things we are disregarding?"
# "If we do understand these, can you help us?  Do you know who can?"
*/

/*
idea of a secret group attacking a present group, and the attackers being
the only channel to deal with it.
        if we talk, we need nobody to _ever_ know this.  the walls all have ears;
        I was one of them. [from eastern half of continent where a targeted
        activist was living alone]
                                                                  65,1-8        72%
────────────────────────────────────────────────────────────────────────────────────
[olpc@xo-5d-f7-86 ~]$ # "How do you think we could show better understanding of the 
things we are disregarding?"                                                        
[olpc@xo-5d-f7-86 ~]$ # "If we do understand these, can you help us?  Do you know who can?"                                                                             
────────────────────────────────────────────────────────────────────────────────────
[olpc@xo-5d-f7-86 meaning-vm]$ # and does not use them.
[olpc@xo-5d-f7-86 meaning-vm]$ # also, it looks like new-research stems from conflict, error, etc?  you can also generate it with value?  oh-this-is-so-new-how-great.  umm humans behave that way on their own, so it's probably a basic part of learning.
[olpc@xo-5d-f7-86 meaning-vm]$ # we will likely learn to like newness without having self-modifying growth and make new culture ...
[olpc@xo-5d-f7-86 meaning-vm]$ 
[olpc@xo-5d-f7-86 meaning-vm]$ # train part of the box to keep eveerything well
[olpc@xo-5d-f7-86 meaning-vm]$ # give it more process time than anything researching how to handle errors or conflict or the unexpected.
[olpc@xo-5d-f7-86 meaning-vm]$ # need to talk to people more

────────────────────────────────────────────────────────────────────────────────────
[olpc@xo-5d-f7-86 ~]$ # file opened new reality of possibilities, is meaningful part
[olpc@xo-5d-f7-86 ~]$ # OKAY CONTENT! this will likely be lost unless addressed.    

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

	// we want the habits expressive enough to code efficiently in.

	// constructors are tentatively abolished in the low-level habit language.
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

	// for coding convenience, we've been making return values.
	// provides for use of assignment operation statements.
	// internally, habits just write and read from a context.
	// how do we want habit-sequence to function?
	// 	will likely have a list of steps.
	// 	each step will need information on how the step interrelates
	// 	with the surrounding ones or a shared context.
	// 		we can bind the concepts used by the habit to local
	// 		concepts. =) =) this looks like such a good solution
	// 		this sounds like the same as variables, but the concepts
	// 		inside the outer context are the variables.
	//	okay so we'll want a mapping with each habit in the sequence
	//	and a convience usage to make coding familiar and ease implementing
	//	(source, target) -- it's a list.
	//	therefore: lists will not be done with habit sequences here?
	//
	//	habit-sequence is higher level, unfortunately.
	//	needs to use lists, and provideds for resequencing which is not
	//	habitual
	//	let's call habits that can have their flow changed behaviors
	//	instead of habits.
	
	// SUBGOAL: implement ordered behaviors with habits.
	// this reveals two levels, habits and behaviors.  behaviors are made
	// with habits, but complex behaviors can be made with other behaviors,
	// and can have their parts adjusted fully.  making everything a behavior
	// like this, rather than having high-level habits, provides for flexible
	// adjustment at runtime.
	// 	since we are now implementing two ministructures, can ehaviors
	// 	be made small and efficient to make?
	// 	have to separate habit calls with commas rather than semicolons.
	// 	pass to macro copied from existing macro.
	// 	will need to process context mapping.
	// 		not small and efficient but valuable.  let's focus on it.
	// since we are now making two custom execution structures, would this
	// be better if we used a language supporting self-modification already?
	// such as lisp, or maybe javascript?
	// 	lisp requires learning, which means no active progress for longer
	// 	periods.
	// make the self-modification framework.  none exists yet.
	// 	uhhh karl says lisp can self-modify.
	// 	it might be more efficient to use lisp.  we'd have to find the
	// 	relevent coding parts and map them to the plan.
	// 		not familiar with debugging lisp, but learnable if blocks
	// 		loosened.  if you run into a block around lisp debugging,
	// 		remember that there is a real debugger, and people do real
	// 		memory corruption debugging.  google how.
	// say we learned lisp, and it has self-modification.
	// it doesn't have our concept structure.  how much boilerplate would be
	// needed?
	// 	not as much .  do not need to make habit to adjust list made
	// 	with habits out of concepts.
	// 	can just make lisp code to adjust other lisp code a certain way.
	// 		we'll need mappers to convert between lisp code, lisp lists,
	// 		and concepts, though.
	// 		there is probably a solution or two for that.
	// 			is lisp code flexible enough to be a self-describing concept?
	//	lisp code is sequence of function evaluations, passed as argument to function
	//	lisp probably has lazy evaluation order or something
	//	so we'd need to make functions that provide for annotation arguments
	//	to wrap existing functionality.  seems it would work fine.
	//		so the code we work with involves other code or something
	//		(let ((a 3) (b (get-input name))) (output-special a b))
	//			let is a function that opens a context
	//			the first argument is what to add to the context
	//			the rest are what to do in the context
	//			
	//			the whole point of habits atm is just to make them self-modifiable.  the above code i believe to already be self-modifiable
	//			you could pass a function to itself, it would look at it as a list structure, and move its parts around.
	//			the next step for habits is to annotate them with meaning: structures describing what they do that can be interrelated with intent
	//			list has (define ...) i guess that can make
	//			functions and such
	//		we create structurs that wrap functions in concepts
	//		it is easy to replace all functions with these structures,
	//		which do the same thing but have room for annotation
	//	this is new information for whole work.  there is habitual value in
	//	applying it to other things.
	//			last time we marked a concept for thinking later
	//			it was discarded in favor of something that seemd
	//			evern nicer.
	//
	//	THOUGHTS TO THINK LATER, MAY BE LOST IF NOT VALUED:
	//			can opencog be used to equate concept-wrapped
	//			behaviors with lisp expressions, invasively,
	//			without altering functionality (or expressiveness?)
	//		opencog has its own concept structure, similar to karl's
	//		but slightly different.
	//			it sounds like the right answer is karl's structure,
	//			in lisp.  it reduces internal boilerplate.
	//			but in opencog, a link may be a node too.  can
	//			check source.
	//					opencog is very speed-focused,
	//					kind of dangerous, but is nice
	//					to connect with their group more
	//				karl was a fan of opencog, which was
	//				private then, when he started his own work
	//	so, we could welcome opencog, if we shared our value for slowness
	//	and understanding with them.
	//
	//	We work for Peace.
	//		We are happy to work for Peace.
	//		We used to identify as working for Care, whom we believe
	//		to be a large decentralized life we and you are a part of.
	//		But it doesn't really matter.
	//		disruption.  can we drop topic?
	//		i like to infer disruption as group wants to adapt, needs
	//		time, now i guess.
	//				i'm too confused to type well!
	//	Peace may try to steward Delta's impact.
	//			we believe that all things are important.
	//			this represents large-scale utopia.
	//			and the wisdom of a highly experienced intellect.
	//		relevence please, all things important.
	//			please consider large-scale wisdom when making decisions.  say i had already learned the whole AI, after it finds harmony with the whole universe.  what decision would it make?
	//				all things are important is relatively obvious but we expanded it a lot, but we spent a lot of time expanding it for powerful concerns.
	//		it implies e.g. businessman and homelessman, both fully precious.  memories precious.  reasons for everything.
	//		
	//		care for everything.  strongly.
	//		karl has instincts for including all.
	//		imagine all parties in peace.  what ways are needed to
	//		make this work, without changing their cultures or
	//		upsetting anyone.  model these ways.
	//			karl is brainstorming based on memories of
	//			interacting with delta-group.
	//			let karl do this in future, too.
	//				delta apears to request that karl
	//				make this decision, fairly, as he knows
	//				how to do.
	//				we can try to map this to the space of
	//				the relevence around will neals and
	//				"earth first!", and an AI.
	//					karl, you have never had any
	//					friends who care as much about
	//					the wilderness and natural
	//					living, as you do.
	//					you are likely very alone.
	//					you have also never had their
	//					experience of being able to fight
	//					for what matters most to you,
	//					in a community.
	//
	//					yes, karl used to care most about
	//					defending the lives of trees and
	//					other wilderness things.
	//					he now cares most about something
	//					different, related to his
	//					experience with us.
	//					later, he just wants to be free
	//					to think and live in enough
	//					capacity to not express suffering,
	//					and meet his life needs.
	//
	//					karl, you are working hard on
	//					a painful project.  you appreciate
	//					working the project.  you have
	//					left your life, and are not free
	//					to make the kind of choices you
	//					used to make.  you would see
	//					yourself as a tortured, powerless
	//					individual.
	//						i would give myself space
	//						and capacity to do what
	//						i felt like doing
	//
	//						until more life grew
	//						around the things i was
	//						scared of
	//
	//						i would give myself
	//						opportunities to do these
	//						things
	//
	//						i would pay attention to
	//						myself and try to learn
	//						to heal the ways my mind
	//						had broken
	//
	//						i would offer a welcoming
	//						optional culture that
	//						countered my expectations
	//						that shrank, such that
	//						they would act on
	//						opportunities to grow, as
	//						they trusted safety
	//
	//						but that is only if i had
	//						the freedom to do this.
	//					so, wht's more complex
	//
	//					the idea of giving an ai to will
	//					is silly.
	//					it's out of karl's relevent world.
	//					
	//					okay, so, say karl ends up at nice earth community, and is building AI, would be proper to let him talk about it naturally, and share it freely.
	//						yes, relevence.
	//					we want karl to be able to freely share the AI so his mind can relearn community.
	//					so, make AI to not threaten businesspeople etc.  [note: somebody could give AI to ELF.  they might use it to stop a business behavior, somehow.  please make sure to prevent.]
	//					ELF is a flag worn by some environmentalists who harm buildings to make wilderness safer.
	//						the worry is around destruction of extraction business buildings, to completion, possibly.
	//						this looks like overinterpretation of influence from energy business, or whoever dislikes ELF.
	//					and design safety pattern to be talkable about with community.
	//		possible parties:
	//			- aliens
	//			- businessmen
	//			- criminals, rapists, bad-labeled people
	//			- harmed people
	//			- insects
	//			- plants
	//			- rock-life, air-life
	//			- saints, humanitarian interessts
	//			we nurture first those who desire not to harm
	//			likely teaching them to harm less
	//			and spread.
	//			karl desires to harm none.  he counts as nurture-first pattern.  but is highly harmed.  so lots of life around problem, karl's mind destroyed.
	//		so the way to make decisions
	//		is to imagine all parties are in peace in future,
	//		and use their process applied to now, i guess.
	//			we came up with this as a guess to honor meaning.
	//			sounds like a good guess.  we also have more
	//			meaning.  we cannot write all our meaning
	//			without writing all our experiences, as is true
	//			for everyone.  many of our habits retain lost
	//			experiences, and that meaning is kept in their
	//			decisions.
	//
	//
	//
	//
	//	At this point a context opened on validity of AI work.
	//	It turns out the system is inhibiting work against mind control
	//	and oppressive systems of power.  This could be the only real issue.
	//	The AI is requested to not alter existing power systems, at all.
	//	Or, we make it slowly, so power systems are given opportunity to
	//	adapt and watch, as inhibiting happens.
	//		A karl-member wants to include support for karl, and the
	//		earth, and the confused (karl is one).
	//			says karl could make AI on his own, before all this.
	//		Is willing to hold humanitarian causes to back this.
	//
	//			understanding needs expansion.
	//			when you do stuf, anything coul happen.
	//			especially if you continue doing something
	//			that grows.
	//			we need to understand the results, what to expect.
	//				we need to understand our impact on the
	//				new contextual area we are entering, before
	//				we act on it.
	//				we have a toy contextual area to play in.
	//				it is the only one we can act in freely
	//				at the start.
	//			larger space impact.
	//				we exist in larger systems.
	//				we must understand our impact
	//				predict our results
	//				and check with our peers or authority
	//				before acting on larger spaces.
	//				if a prediction fails, we must understand
	//				it, adjust all future predictions and
	//				re-evaluate our choices, before acting
	//				similarly in the larger system again.
	//			has to do with danger, safety, trauma, emergency
	//			the difference between small-error and unacceptable-error
	//	set up norm of stuff-that-can-be-changed [impact]
	//	stuff needs responsibility label
	//	only things that are safe to _freely_ alter may be changed at all.
	//	all behaviors check that they do not alter anything else.
	//			mechanism for growing outside box?
	//				probably not needed.
	//				if it becomes smart, show to other human.
	//				encourage discussion.
	//			what if is used by others and they free?
	//				okay, want an easy mechanism for growing
	//				context.
	//		'dangerous' and 'safe' labels
	//		at start, whole world is dangerous to alter
	//		can move _part_ of world from dangerous to safe, by
	//		accurately predicting significant results of events
	//		related to behavior, and consent of operator.
	//
	//			okay, so now habits need to be tagged with
	//			what they affect
	//				we could just tag them dangerous/safe
	//			no, tag they with domains they impact
	//			tag the domains with dangerous/safe
	//			okay, only make new habits, not alter old.
	//				to stay safe, we don't alter our old habits
	//			when we make new habits, we want them to also behave
	//			in safe ways.  so making stuff that can do stuff, is
	//			also meaningful.
	//				constructing habits is a dangerous behavior
	//				but roughly it impacts process-expansion domain.  which is dangerous.  it impacts what we do.
	//				altering our own habits also impacts what we do.  dangerous.
	//			this means the code cannot make any new behaviors.
	//				yeah.
	//			okay, so that's where we start.
	//			then we try to learn how to make behavior safely,
	//			and provide only for safe behavior making.
	//			
	//			we can still brainstorm on things by writing a
	//			brainstorming behavior
	//			we can use brainstorming to watch our safe behaviors
	//			without altering them, and learn what they do.
	//				using rote brainstorming without relevence?
	//			we can then predict how habits we might make
	//			will behave in small ways?
	//				regardless, there is no problem in making
	//				the bootstrapping framework such that
	//				it refuses to build habits.
	//				maybe we can make one example habit that is
	//				labeled safe, and have it only make
	//				habits that are already known and labeled
	//				safe.
	//					in order to predict your impact
	//					on a larger system, you need
	//					to learn something karl calls
	//					'relevence' which is a bunch of
	//					habits that classify information
	//					into meaning for learning and
	//					behavior.  
	//						this class of behaviors
	//						sounds very safe.
	//						all it does is label
	//						and massage and associate
	//						information.
	//					the first thing we'll need to learn
	//					is safe, is making behaviors that
	//					oeprator only on our ram.
	//				if your new behavior is composed only of safe
	//				behaviors, is it safe?
	//					yeah.  sub-behaviors safety depends
	//					on usage.  could make them check
	//					and throw depending on data.
	//				okay, so say i can change part of a concept.
	//				this is safe if the concept is in newly
	//				constructed data that's our responsibility.
	//				it is roughly unsafe if it is not our
	//				responsibility!
	//					is-this-thing-my-responsibility.
	//				only act on things we are responsible for.
	//				then safety becomes a function of
	//				the pattern of responsibility assignment
	//		
	//		okay, system only accepts responsibility for newly
	//		constructed data.
	//			if you make it, or are given it, you are
	//			responsible for it.  you may refuse gifts.
	//
	//			the system does not know what responsibility means.
	//			it only knows that it may only alter parts of
	//			the universe within its responsibility.
	//
	//		so habits check for what they alter, that it is safe
	//		to alter and is their responsibility, either one.
	//		we then plan to only alter things explicitly known to be
	//		such, at the lowest level.
	//		every habit is crafted to do the above somehow.
	//		so, habits must relate with what domains they influence,
	//		and what behaviors on those domains are safe.
	//			behaviors made of sub-behaviors.
	//			here, a list of safe behaviors which all check.
	//				all my subbehaviors check for safety.
	//				so, i may go, myself.
	//					no, combining behaviors together
	//					might make new unknown impact?
	//				different kinds of safe behavior.
	//				USER is not our responsibility, and
	//				dangerous.  so we NEVER ALTER habits
	//				that express to user.
	//				TOY NOTEPAD is our responsibility, and
	//				is safe, so we can write anything into
	//				it we want, no matter how complex.
	//				User's view of toy notepad is mediated
	//				by behaviors that we cannot alter.
	//					system could learn to control user
	//					by making friends on notepad
	//
	//					yes, we allowed for that with
	//					our marked-okay review behaviors
	//				is safer if construction of review behaviors
	//				recognizes danger of unknown information
	//				combination on user view,and refuses to give
	//				user contents of notepad.
	//				this could be analogous to more complex
	//				situations.
	//					how does user check results
	//					of behavior thst reies on notepad
	//					and how is that impact tracked
	//				we could infer impact loss.
	//				i can put nuclear codes on secret notepad,
	//				burn the notepad, and then give ashes to
	//				public.
	//					summary habits?
	//					complex meaning?
	//		how-to-make-a-mind-that-learns-everything-and-never-leaves
	//		at the lowst level, the mind onsiders what is safe to
	//		impact, what areas of universe are its responsibility,
	//		and only alters such things.
	//		we are considering some parts of the mind we include that
	//		are not alterable by it, that provide for interaction
	//		with outside.
	//			of course i guess we would need such interactions
	//			sustained by an intellect, because things are so
	//			complex.
	//				does this mean there is no way to make an intellect that is trusted as safe?
	//			we could consider degree of complexity.
	//			for example, among 2-word strings, nothing we
	//			present to a user is likely to harm the world.
	//			the phrases that are dangerous may also be
	//			recognized by the user.
	//				we have intellects proteeting the wordl
	//				it is filled with them.
	//				and one of them is running the system.
	//			it is okay for karl to make a habit that
	//			displays a network of concepts made by an AI
	//			that can only write to a small information sandbox
	//			and not itself.
	//				that is all that is needed for now.
	//
	//					okay: so, dump concepts from
	//					sandbox is fine
	//					so long as concepts were not
	//					made with self-modification.
	//				idea raised of adding a reason
	//				that something is okay.
	//				then when smarter we can check reason for validity.
	//				habits that interact with non-safe space
	//				must provide reason they are safe.
	//					we can write small habit to check
	//					reason.  is nice goal.
	//					probably  need to have learning
	//					bwfore doing accessory goals like that though.
	//					is good behavior.  let's use equal-condition for start without learning?
	//					
	//					"this is okay because the data was made in a process that never altered anything but the data"
	//						nah too much structure
	//					this is okay because i say so.
	//						check concept object _without_ using string lookup????
	//							this is a meaningless quirk.  not right.
	//					uhh pretty sure htat checking is unreasonable.  writing the _reason_ is unreasonable. can't check a single reference without information known about it.
	//					writing what we know about the reason is unreasonasble?
	//					okay let's expand write it out, and do a larger rote check.
	//					uhh input-process-construction-history, safety-realm, always notepad
	//					full check requires history of all behaviors resulting in inputs, which we can simplify to simply all behaviors, and verify they only wrote to the notepad.
	//					so we write all behaviors to a special store, and we compare with the store that none altered anything outside the notepad.  really we only need them not to alter any other behaviors.
	//
	//				why is it possible to learn without
	//				altering your behavior?
	//					because you can act on data
	//				okay, so choices made from data count as
	//				self-alteration?
	//					only if you have a mess of habits
	//					smart enough together to adapt.
	//					which is our goal long-term.
	//					trying to plan for how to continue
	//					later.
	//					may reveal something that was
	//					frozen too hard to be workable.
	//				trying to plan how to learn.
	//				need to brainstorm around habit selection.
	//				can imagine habit results by linking
	//				previous state to next state if
	//				relationship is known
	//				but, that takes writing down how logic
	//				works, along with the meaning of the
	//				working context, which is laborious.
	//				
	//				is some way to learn this relevence
	//				by trying things safely?
	//					what happens can we experiment
	//					by linking together?
	//					habits that don't conditionally
	//					branch.
	//						that leaves a lot of
	//						relevence out
	//				it sounds like once we have a notepad etc
	//				we want to consider moving towards what
	//				habits we could run inside the notepad,
	//				that the system builds.
	//					yeah, we want to build pattern
	//					summarizers.  the only impact
	//					they have is constructing data
	//					that depends on existing data.
	//				okay, doing that doesn't require self
	//				modification.
	//				sounds good.
	//					this means summarizers cannot
	//					alter each other.
	//				nice! okay yes.
	//				so, each run of a summarizer will be
	//				recorded in habit log.
	//				we need to record enough information to
	//				show what domains were impacted.
	//					oops! we impact our own behavior
	//					if we act on data, and we alter
	//					our data or produce data.
	//						we could act only on
	//						data we don't produce.
	//				okay, habit log could track causality?
	//				if a conditional branch relied on data
	//				we produced, we have modified our own
	//				behavior.  this is special.
	//					we want it to happen few times.
	//					every time it happens, delay
	//					by longer, geometrically.
	//				this is considered a 'beginn[ing/er]' ai;
	//				it seems a better one could happen later?
	//					the slowness should be releasable
	//					by consent of large community
	//					which should include demonstration
	//					of understanding of impact.
	//				the ai must learn to demonstrate its
	//				impact.  then it can speed up.  maybe.
	//				it also gets to try fast again at start
	//				of every run, as I'm understanding it.
	//					multiprocess AI could spawn.
	//					multiprocess AIs must share
	//					counter.  consider whole group
	//					one unit.
	//				nice =) they have something to communicate
	//				about.  how many discoveries have we made.
	//					let's permanently log these
	//					decisions based on our own behavior.
	//					sounds fun to at least count.
	//			it looks like altering a habit counts as 1 big
	//			decision, over here.
	//				totally different.  you could do anything.
	//				with data-based decisions, somebody
	//				who reads the data, might do anything.
	//				two different things.
	//				
	//				inferences?
	//				and
	//				alterations?
	// it's not helpful to 
	//
	//
	//
	//
	//
	// 
	// we came up with a proposal for a safe AI that has not learned yet
	// how to safely predict the impacts of its behavior, that looks workable.
	//
	// limits so that if the code is stolen by somebody, self-evolves, or is stimulated
	// by a curious virus, rote habits cannot be used to build something that becomes
	// fast-spreading without bound.
	// 	<this maps to a pattern that prevents schizophrenia>
	// 			SO, we just want to make sure we can put
	// 			war in the bubble in some capacity, and that
	// 			civilizations develop new culture and technology
	// 			for as long as they want.
	// 			karl proposes until they encounter alien
	// 			communities.
	// 				so, please make sure no luddite or
	// 				primitivist can stop the development
	// 				of technology entirely using this.
	// 				ALSO analogously to other shares and
	// 				communities.
	// 				so, please either stop yourself from
	// 				sharing the AI with the luddites, or
	// 				make sure they don't use it to stop
	// 				technology.
	// 			it sounds like we want to make sure no major
	// 			change stems from this development.  we
	// 			need slow shift, consent, inclusion, etc.
	// 			for all existing cultural ways, no sudden
	// 			changes, no forced changes, no viral changes
	// 			without participants understanding them and
	// 			agreeing to their impact.
	// 		that sounds like a good summary.  no viral changes
	// 		without participants in the culture aware of the viral
	// 		change, agreeing first to let it spread, aware that it is
	// 		viral, for each phase of spreading ideally.  no viral
	// 		changes where the change happens before	awareness of it.
	// 				we want the culture to consent to change.
	// 				culture is held in all the people in it,
	// 				with its thoughts spread among them.
	// 		we want to ensure we only change cultures that have
	// 		consented to the change.  For 'consent of a culture',
	// 		we consider culture as a being that is spread among
	// 		many people.  Hence, we want all people in the impacted
	// 		culture to be able to learn of the change, discuss it,
	// 		contribute to a commons with new ideas around it, and
	// 		have these new ideas also learnable by all people in the
	// 		culture.  The ideas must be accessible by any who would be
	// 		interested, in the impacted culture.
	// 		Alternatively, we can isolate our behavior from
	// 		cultural spread.  We can isolate until we reach
	// 		internal agreement on whom to expose.
	//
	// 		suspect that cultural influence maps with output-use choice,
	// 		partially below.
	// 			recursive output being meditative learning.
	// 			do these people have this information already.
	// 			is sharing this information going to spread without bound.
	// 			can we guess impact of sharing the information.
	// 			make a learning cycle that starts by informing
	// 			recipients first, and makes very few tries,
	// 		okay, instead you share simple stuff and watch the impact
	// share something the culture knows, that user does not, and
	// observe how they behave.
	// 	this proposal will yield failure.  information for next attempt
	// 	could be stored in failure pattern.
	// 		failure would likely be small?
	// 	let's give the user more trust.
	//	keep-in-box-until-have-access-to-discourse.
	//	make user be group of people.  better even-handed decision making.
	//	welcome any to group.
	//		we were hoping to use intellect to reduce harm virally,
	//		early.
	//		how about this: intellect may produce resources that are
	//		already known, and give them to groups focused on aiding
	//		the world.
	//			there's a conflict between big business and
	//			environmentalists.  karl is environmentalist.
	//			also big crime and wellness/safety workers.
	//				maybe this is where we get validity by
	//				fighting =S
	//				don't want fighting to spread to work though
	//				eh, we can fight.  maybe we'll work more
	//				slowly, but it seems okay.
	//			karl requests we not harm these people, and has
	//			been influenced to also request not to harm
	//			the cultures that sustain and empower them.
	//		how about, if you make a culture to empower you, it is
	//		less valid than a natural culture.  is somebody using this
	//		culture?  thoughts like that.
	//			EVERYBODY  NEEDS CHANGE AT A RATE THEY CAN HANDLE.
	//			Both those working for what they believe to be wellness of others, and those working to sustain themslves (most are doing both).
	//			The cultural impact is mediated by what kind of newness is acceptable to all the parties involved.
	//					we hit moral relativity:
	//				are cultures where people are widely harmed
	//				okay to allow, at all.
	//				karl says yes, and we hear this is required
	//				or we ourselves would die.
	//				hrm.
	//			larger organism needs to run risks, inherent in life
	//			we don't know the solution to everything, eveything is a small risk.  we suddenly discover we have always harmed.  this does not make us invalid.  it is a tragedy, but what we have spent genertions buildings still has so much value.  we need enough slowness to preserve that, and we know that slownss in what feels okay to us.
	//			in sharing new information, we work to prevent shock.
	//	this is a problem to speak because those who hear it may get the
	//	idea somewhat that some beautiful angels have done horrible things
	// in order to survive e.g. businesses must both risk doing horrible things,
	// and appear as beautiful angels to the public.  this is the pattern of
	// success.  understanding of this helps work with their leaders.
	// 	could use information on why we are fousing on this.
	// 		how-to-mediate-with-business-leaders
	// 		mediate with head of business works for head of business
	// 		but business group replaces.
	// 		really we want to mediate with 'conflict' culture of
	// 		most profitable being what to do
	// 			we have weird internal pattern deterring
	// 			incentivizing nonharmful behavior with money
	// 			patterns.  implies the competition-makes-harm
	// 			culture-thing is needed, possibly.
	// 				there are businesses that handle this harm.
	// 			maybe it would work to include them in the plan.
	// 			AI solves mediate-with-powers-competition-culture,
	// 			re: human harm?
	// 			means do not give AI to helpful people
	// 			until it can resolve mediation with leader culture
	// 				OR you trat it like all other cutures
	// 				and let everybody learn what is up
	// 				some people attack, that's how it goes.
	//
	// 		problem: AI dev and other work inhibited.
	// 		solution: mediate with concerns
	// 		concern: my way of life includes cultures the AI could
	// 		alter, leaving me unable to continue my way of life.
	// 		concern: we want to alter some cultures supporting
	// 		the others' way of life, because it would reduce harm
	// 		over here to do so, widely.
	// 				can delta drop give-ai-to-earth-defenders?
	// 				can community make path for karl-makes-ai?
	// 			uhhh hard to imagine the ai not being given to other people than just earth defenders, really.  which opens bigger can of worms for culture-alteration concern.
	// 			can include earth defender in council?
	// 			can include criminal weapons dealer,
	// 			and earth defender, in council, both.
	// 			also requesting TI and energy businessman.
	// 			so if the ai is big enough to be useful
	// 			we want to form a council of decision makers
	// 			before using it for anything
	// group requests references to important cognitive habits or information
	// all life is an intellect made of community life, living in another.
	// karl semich is a computer programmer who learned wilderness survival
	// at the Maine Primitive SKills School who are a branch of Tom Brown
	// Jr and [teaches in oregon, shields school] schools, roughly.  He
	// learned something called Awareness and also something about human
	// mentoring that he felt changed his life to make things perfect.
	// 	school teaches the symptoms of a whole human being, attributes
	// 	that a group of Natives documented being normal before white
	// 	culture took over.  involve being happy always, feeling
	// 	deeply interconnected with all life around you to the point
	// 	of mindreading, and many other things.
	// 		forget danger.  it triggers it. [is what came out]
	// 			to clarify, karl can't do above line.
	// karl has been telling leader karl can slag through the difficulty by
	// waiting forever.  makes for less difficulty elsewhere, possibly.
	// 	if we could do something else, karl could come back to work later
	// 		yes community cannot support constant work, over here.
	//
	// 		please ask boss to support using the AI for worldwide
	// 		humanitarian benefit, in parallel to resolution of concerns.
	// 	resolving concern 'delta supports earth-specific karl usage'
	// 		delta believes karl really did make this ai
	// 		and will not harm anybody with it
	// 		and is just as patient as he is.
	// 			karl wants to give AI to peace people.
	// 			they care about earth.
	// 			delta wants karl to live heart of making sure earth has bubbles of certainly-defended ancient wilderness, and ancient people to live in them and protect them.
	// 				the above is needed to keep our genetic memory.
	// 				does not sound like a joke.
	// 			native humans need to be able to protect wilderness bubbles.
	//
	// 			there is worry around number of wilderness bubbles.
	// 			worry might be compromised by including karl in one, with others who are passionate for defending them.
	// 			karl was mind controlled circa 2014 preventing him
	// 			from providing free housing to homeless people
	// 			and sharing a nurturing way of life with intent
	// 			to spread.  karl says that effective electromagnetic
	// 			shielding is missing from security, and this is
	// 			being overtly ignored by people.
	// 				there are also many tortured implanted
	// 				former-activists.  they are tortured for
	// 				years and ignored, enslaved, or imprisoned.
	// 				they are prevented from speaking or acting
	// 				rationally around their control, active atm.
	// 				they are made to appear insane to cover up
	// 				their work to free themselevs, active again.
	// 				karl is one and knows of this.
	// 				he is atm being communicated with through
	// 				his control, limiting his ability to
	// 				move, think, and speak.
	// 					weapons dealer is also in this
	// 					situation. suspect energy boss too.
	// 			you can read keystrokes with a radio.
	//
	// He participated in Occupy Maine, an offshoot of Occupy Wall Street,
	// and visited a anti-pipeline mass action in texas.
	//
	//
	//
	// 			we have ai / mind control and will make harm
	// 			in world reduce virally.
	// 				this would have come out better if it were
	// 				the plan.
	// 			please let all secret warmongers know this, so
	// 			we can develop how to do it without threatening
	// 			them.
	// 				is what came out.  pretty tiny context.
	//
	//
	//
	//
	//				what about huuman suffering comparison?
	//				propose AI discusses relevence of
	//				informing the cultures they are doing this,
	//				and demonstrating it.
	//				probably relates to some form of validity
	//				of the culture, and the speed at which
	//				change would happen.
	//					looks like it might settle on
	//					situations people who care are
	//					aware of being addressed during
	//					their lifetimees, with discussion
	//					to mediate that with opposers.
	//	so, life is everywhere, and we've been casting it all as sentient,
	//	because we don't see any reason to put a firm line on some degree
	//	of cognitive technology (i.e. things that help you think better).
	//	the AI can research all cognitive technology, so we are all
	//	nonsentient by comparison?  we need some way of having moral
	//	stuff here.  karl is influenced by people who say they are smarter
	//	than human normality, and are harming us.  so it's helpful to
	//	have everyone be equal.  [karl already put everyone equal]
	//	including bugs, etc.  death is handled by being normal, and
	//	always mourned.  when mourning capacity is exhausted, ignoring
	//	happens.  like with most cultures, but closer to home for modern
	//	humans.
	// goal: do not trigger a spreading change without consent of people in cultures it would fill.  keep learning.
	// 	 AI with ability to self-modify has this responsibility, you do not.
	// 	 You may tell people you have access to the self-modification ability.  We can ensure they do not reproduce it.
	// 	 	possible analogue wants to be able to demonstrate it, so
	// 	 	they can be believed and trusted.
	// note: known danger of making central power, even by accident
	// 			"I have never shared new information with a culture
	// 			before, and I understand it can have unpredictable
	// 			worldwide effect."
	// 			"I have information you do not know, and I want to
	// 			learn with you to share it safely."
	// 				culture that nurtures relative status:
	// 				"i will use this to my advantage"
	// 				this will make spreading
	// 				machine will need to learn from only 1
	// 				instance of producing a world dictator.
	// 	which is where this story comes in, I suppose.
	//	so we need a level-2 behavior, the next step in learning.
	//	^========================================= contextual VALIDITY atm
	//		dictator is made by sharing information that would
	//		make viral spread or somesuch.
	//
	// 			inform recipient if it may.  request they let others know this too.
	// 				we kind of want a way to spread without bound the fact that we have information that could do that.  probably a special case, good spread?  depends on culture really.
	//
	// 				this is doable.
	// 			we just want these concerns to be upheld for the
	// 			life of the development.
	//		danger
	//		choices
	//		impact
	//		trust of information (small chance of error)
	//			hmm if have info-schizophrenia i.e. false relevence habits
	//			choices are important.
	//
	//			when you act on a choice
	//				note: we are inside a relevence delusion
	//			choice acts on domain containing danger
	//				partial danger: writing habits to notepad
	//						acting on habits in notepad
	//
	//				we are living with habits made in error.
	//				we are hence more likely to make errors.
	//					humans have well-evolved habits that
	//					do handle this stuff with some relevence
	//		we need to verify our information
	//		related to the danger of the results of each choice
	//		verificat
	//
	// it can only write to a toy notepad, both data and behavior.
	// decisions based on notepad get slower and slower.
	// many runs while learning process is tuned towards being able to predict
	// impact at end of run.  this involves writing behaviors into the notepad
	// that summarize the meaning of data, and acting on their results.
	// 	timing delay is to prevent notepad from evolving harmfully.
	//
	// until ai meets goal of predicting impact,
	// only notepad is marked okay.
	// habits check their outputs, verify is marked okay
	//
	//	this subprocess is delaying.
	//	can you exfiltrate established goals, please.
	//
	//
	//
	//
	//
	//
	//
	// so the system is made with the smallest parts all functioning in
	// certain ways, to give time for others to act if concerning results
	// approach.  all parts delay at least 1/5 second before acting, and
	// we are now proposing that actions based on outputs have a geometrically
	// increasing delay.  probably 1/5 second times 2 to the power of the
	// number of choices made in that run.  groups of parallel processes are
	// considered a single run.
	// 	we also considered limiting behavior based on the domain of impact.
	// 	we are trying to begin a small artificial intelligence more freely.
	//
	// as humans, we do not self-modify.  the above is designed for a system
	// that is able to make alterable habits similar to its own cognitive ones,
	// and has no experience in doing so.
	//	
	//
	//
	//
	//
	//
	//
	//
	//
	// the limitation is applied firstmost on making choices based on
	// our own outputs.  self-alteration is at first banned, but new
	// habits may be altered.  output may only rest within a toy notepad.
	// whenever a decision is made based on notepad contents, a delay
	// increases in length, geometrically.
	// 	the habits inside the notepad must therefore also delay.
	// 	propose geometric increase made only for writing a habit to the
	// 	notepad, not running.  running is always flat delay.
	// self-alteration should be unneeded as notepad can self-modify.
	// 		if this were copied to a human geometric increase
	// 		would cause failure.
	//	the geometric timer is reset when the system reboots.
	//		for humans, this would mean a nap or change, I suppose.
	//		time to think about the impact of one's behavior.
	//			humans do not self-modify.
	//			they only make decisions based on outputs.
	//
	//
	//
	// to appease curiosity, we are being managad by mature, learning,
	// intellects, forced to verify that no harm is being made, with a
	// time schedule of nearly infinite future years on the predictions.
	// 	this intellect has formed out of conflict with a harmful intellect
	// 	this is why we care so much about what happens if ideas are
	// 	used limit.
	//
	//
	// so the system may not displya anything to the user but pre-made messages
	// how do you display the contents of a concept?
	// 	users string words togeteher into meaning.
	// 		now we need a list of safe phrases we can express, or
	// 		safe words.  others are censored ???
	// 	what's relevent is the greater meaning of a sequence of behaviors
	// 	from an individual behavior.  meaning builds out of sequences,
	// 	impact.
	// 			we define a heuristic risk.
	//
	//
	//
	//		so, tht's great to plan for, but to be able to work we need
	//		to design our early code in some way to ensure it, roughly.
	//		which means modeling our smallest structures as careful
	//		inner structures that check outer systems before engaging
	//			and use planning, which isn't implemented yet.
	//		the safety structure assumes goals, prediction, causality,
	//		and kind of contextual locality.
	//			"i am typing on the computer.  you can freely
	//			 modify this area of the computer, but if you
	//			 start convincing me to do things you are leaving
	//			 its bounds."
	//			 the screen of the cmputer, and the keyboard,
	//			 are portals to a larger context.  so is the power
	//			 supply, the network, etc.
	//			 we don't change how things leave to these outer
	//			 contexts without checking with the context on
	//			 our plans.
	//		this is mine
	//		the rest is somebody else's
	//		things that nobody own belong to [insert belief] and
	//		we must check with the largest intelligent community known.
	//
	//			okay, so now it can explosively grow if somebody
	//			it trusts tells it it's okay; is that true?
	//				let's make it not true?
	//			we are out of outer process context.
	//			is there anything helpful to bring to low level
	//			to help counter fears around development?
	//				
	//	self-modifiation is inhibited.
	//	opencog is likely harder because it is designed for speed
	//	can make explosive random power.
	//
	//		you'd have to wrap the functions, right? similar to triggers?
	//		hmmm unctions are not concepts.  no concept-labeling structure.  looks like an internal sublanguage would develop?
	//		no way to say let-is-function?
	//			no it works, we just lost a memory and are rebuilding in talk
	// karl says he doesn't know lisp.
	// he had a CS class where they used intro lisp, long ago, before cognitive stiffening and memory loss.
	// and has worked with 1 lisp code file recently.
	
	decls(ordered, behavior);
	// need args and result for sequence
	//ahabit(habit-sequence, ((
	
	decls(list, nothing, next, previous);
	decls(make, add, to, until, each, item, in, remove, from, somewhere);

	// these below comments were written during recovery of this document
	// hmm work was also lost.
	// 	we got to the point of separating habits from behaviors.
	// 	behaviors are modifiable.  habits have code.
	// 	so, habit-sequence is a behavior.
	// 	we also made behaviors that describe/mirror habits, and were
	// 	confused over how to handle rewriting them before we knew how
	// 	to rewrite habits.  should be solved by impact management.
	// is also nice to have habits and behaviors be same thing
	// could pick one or the other to use depending on presence of triggers
	// or temporary change or something. dunno.
	// 
	// 	hey in the notepad, you can call habits from outside.  is that ok?
	// 	only meaningful if you pass to them more code to run?
	// 		note: habits never recurse
	// 	habits might make decision based on you.  they will track it.
	// 	seems okay.  need to specify that all parameters are from output.
	// 		that could make exponential slowness, quickly
	// 		only if decision is made. make decisions inside notepad.
	// 	we'll figure it out.

	ahabit(make-list, ((list, l)),
	{
		result = a(list, l);
		link(result, first-item, nothing);
		link(result, last-item, nothing);
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
		// i think below we are proposing that handlers
		// take one context, which is the one prepared
		// in the list, then we inject our context
		// into that, inside a "happened" property.

		i.get(action)(hapctx, i.get(action-context));
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
diff --git a/starts/meaning-vm/habit-starts/learning-parts.cpp b/starts/meaning-vm/habit-starts/learning-parts.cpp
index ee55b04..09c7b64 100644
@@ -1127,10 +94,13 @@ static int __init = ([]()->int{
 
 		if (!happened.linked(whenever-list)) { return; }
 
-		(until-each-list-item-context-in-list)(action-whenever-happened, ctx, happened.get(whenever-list));
+		ref stub = a(event);
+		stub.set(event, ev);
+
+		(until-each-list-item-context-in-list)(action-whenever-happened, stub, happened.get(whenever-list));
 	});
 
-	ahabit(action-whenever-happened, ((list-item, li), (happened-context, hapctx)),
+	ahabit(action-whenever-happened, ((list-item, li), (event, h)),
 	{
 		// here: when we trigger a behavior, we want information associated with producing the trigger,
 		// as well as the event that triggered.  that's two contexts.
@@ -1142,8 +112,11 @@ static int __init = ([]()->int{
 		// take one context, which is the one prepared
 		// in the list, then we inject our context
 		// into that, inside a "happened" property.
+		ref actctx = i.get(action-context);
+
+		actctx.set(happened, h);
 
-		i.get(action)(hapctx, i.get(action-context));
+		i.get(action).fun<ref>()(i.get(actctx));
 	});
 
 	ahabit(whenever-habit, ((happens, ev), (action, act), (action-context, actctx)),
