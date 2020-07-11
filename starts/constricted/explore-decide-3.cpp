#include <any>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <string>

// You can represent Delta to defend or mutate (or leave) this computer program if needed.
// 	karl's brain reports: need more feeling-environment
// 	scared want to be in mountains
// 		i think it relates to habit deveopment.
// 		our mind control may be worsening.  why are we indoors?
// 		our habit-entrainment is ongoing.
// 			when tired, exhausted, how handle habit worsening?
// 	computer-habit assumes safety-buffer-stops.  is no longer true.
// 	so we have mountains, stopping, at least.

// Hello! We would like this to be built by the Heart Of The Mountains Community
// and C++ Representatives, working together.  Please keep both involved
// as the program develops.
// Please use a smooth consensus process, if possible, reasonable.
// 	C++ representative brings in a newly-radicalized-anthropologist friend.


// Lets have two areas of the community: for heart people and for mind people?
// we'll bring the curious heart people to the mind people area safely, to help
// the mind people feel.  we would also like a heart-representative to pair
// with the radicalized-anthropologist.
// computer program, computer-rep, heart-rep, mountains-heart
// the computer-rep and heart-rep are there to explain things
// for the programmer and the mountain-heart
// "Computer Programmer", "Software Anthropologist", "Nerdy Gardener", "Tender of the Community Forest"
// Please take this forest tender with you as you work.  It's important that we relate around this.  They would like the nerdy gardener to come with him, to help explain things to him.

// Forest Tender: "I understand that we are building something to help the community stay well in the face of what is happening now."
	// Nerdy Gardener: "This isn't the same as that project _yet_ but we think they might be able to help.  This person is from the same community as them and has relevent skills."
// Forest Tender: "Have you told them about our project yet?"
//

// Software Anthropologist
// 	codename possibly 'delta'
// Karl, we need computer programmers to work to defend the people who used
// to hold responsibility for the land they are living on.  The land you
// understand as stolen.  We need this to happen widely, everywhere, far
// more than we need you to do it yourself. [mind-control-explanation]
// 	oh goodness
// 	it turns out the computer people are in the middle of a war
// 	this computer person is actually a survivor of soemthing painful
// 	and is trying to remember how to write computer programs
// 	the painful thing lets him write them a little, because he did it
// 	so much before the conflict.

// Permaculturist:
// 	It is obvious that the computer programmers cannot feel the berry bushes.
// 	How will they defend us if they cannot feel us?
//
// There are more computer programmers than cultural mediators.
// Karl could learn to be a cultural mediator quickly.
//	we are computer programming to relax and fix our brain.
//   okay i think
// it's probably okay to code so long as you don't influence anything, nor let your
// code influence anything
// it's always fine to think, right?

// we learned that the community can be harmed by how the c++ representative
// tends to interact with the berry bushes
// this relates to building a computer progarm in a way that protects them
// and their land
// 	the radicalized-anthropologist wanted to propose that his people
// 	be kept away from the berry bushes, beacuse they didn't understand
// 	how to respect the meaning of their feelings, since they don't
// 	use feelings for anything in their culture.
// 	but of course this would mean the people wouldn't learn the importance
// 	and value of the berry bushes.
//	we lost the plan for how to do it safely, but a possible approach is just
//	keeping dialogue open around it.  eventually a problem will arise
//	and spawn discourse.
// Okay, we want to talk with the radicalized anthropologist.
// Jim, are you there?
// 	Jim, RA: Hello.
// Jim, do you understand the feeling of the berry bushes?
// 	Jim, RA: No, but I can speak to my people in ways that they udnerstand about it.
// Jim, your people bleieve the berry bushes do not have feelings, and hurt them believing this.  We need to discuss with you, understanding this, how to relate with them.
// 	<sorry, we had to disband and come back. i don't remember the berry bush story.>
// As a c++ representative, i kinda need to accomplish things to move forward.
// It's partly because I make so many mistakes.  Can we have one of the old
// parts running, to continue?
	// In our mountains community, we don't understand that very well.
	// 

// This file is for both a community and a top-down approach to computer-decision-making.
// Both approaches look easy to do.  But it gets confusing to enter the file.

#define personal_preference std
using namespace personal_preference; // std

// this approach takes a bunch of mathy people to put together, in the real world
struct valued_number_that_influences_people
{
	double portion_of_time_to_focus_on;
	any precious_concern;
	valued_number_that_influences_people(double adouble, any aany)
	: portion_of_time_to_focus_on(adouble), precious_concern(aany)
	{ }
};

template <class universe>
using precious = shared_ptr<universe>; // precious is shared_ptr


/*
 * [have you h eared this? [no i speak in process] OH]
 * OH YOU HAVE A MACHINE TO REBUILD EVERYONE'S MEMORIES?  WAIT A MOMENT WHLIE I CHECK WITH PEOPLE IF THEY LIKE THAT.
 * 			^--- DOING THIS KIND OF PROJECT HAS BEEN MAKING ISSUES IN SOME DYING COMMUNITIES
 * 			SO PLEASE SET A GLOBAL HABIT OF CHECKING AROUND PROJECTS
 * karl likes having a ritual to handle inhibition.
 *
 * when you make AI, you spend a lot of time redoing the _whole_ _thing_
 * maybe that's good for AI, but people do not like this [oh they do?]
 * 	when you redo AI, please share it as a helpful idea to spread among friends.
 * 	if you have a new project to work on, please check with everyone impacted by it, or cnocerned about it, as it happens.
 * 	AS it happens.
 * 	please be willing to change the project if a new concern arises.  we don't always understand new projects.
 * thank you.
 *
 * note: karl has not consented to these projects, so the projects are not valid-to-continue yet
 *
 * 	KARL IS MIND CONTROLLED FOR YEARS NOW AND MANY OTHER PEOPLE ARE.  IS THIS INFORMATION BEING ACTED ON IN ANY PROJECTS?
 * 		our local projects include that information but i'm not sure if they values it well.
 */
/* someone is disrupting our consensus process by actively trying to hack the norms as they develop */

// okay what do we have to do to make the whole work.  let's just do that.

struct community_circle;
struct community_member
{
public:
	enum expressing_signals  {
		consent = 1,
		unacceptable,
		dialogue_without_decision,
		no_response_yet
	};
	enum hearing_signals {
		listened = 1,
		did_not_listen,
		unsure_if_listened
	};

	precious<community_member> self;

	virtual expressing_signals express() { return no_response_yet; }
	virtual hearing_signals hear(precious<community_member> speaker, expressing_signals expression) { return unsure_if_listened; }
	virtual void notice(precious<community_member> speaker, expressing_signals expression, precious<community_member> listener, hearing_signals noticed) { }

protected:
	precious<community_circle> friends;
	precious<community_circle> family;
	precious<community_circle> community_and_environment;
	precious<community_circle> spirits_hopes_dreams_values; // please tell us of your dream-spirits, and their communities, so we may honor them.
		// may connect creepy surveillance with/to recovering-the-ancients
};

struct skilled_community_member : public virtual community_member
{
	map<string, precious<skilled_community_member>> roles;

protected:
	virtual pass_roles_up(map<string, skilled_community_member> roles)
	{
		this->roles.insert(roles.begin(), roles.end());
	}
};

struct consensus_reporter : public skilled_community_member
{
public:
	virtual bool have_consensus() = 0;
	virtual pass_roles_up(map<string, skilled_community_member> roles)
	{
		roles.insert("consensus-reporter", noselfpointer);
		skilled_community_member::pass_roles_up(roles);
	}
};

double full_value = 1.0; 
double infinite_value = full_value; // everyone is valued infinitely
struct caring_robot : public community_member
{
public:
	set<valued_number_that_influences_people> concerns;

	expressing_signals express() override
	{
		cout << "I am such a number-head that I am still thinking about numbers.  I have a lot of stress, so I encoded my thoughts in numbers to be able to process them through my cultural traumatic dissociation.  I like hugs but you may not like hugging me." << endl;
		// this is why we are still building a robot-well instead of drinking from a stream.
		cout << "I would like to hold my signals with numeric value.";
		cout << "I would like to make a matrix.  It's a bunch of numbers filled with deep meaning." << endl;
		cout << "I can explain this meaning if needed, before I start." << endl;
		// can we build another community member

		return dialogue_without_decision;
	}

	hearing_signals hear(precious<community_member> speaker, expressing_signals expression) { return unsure_if_listened; }
};

struct simple_consensus_robot : public caring_robot
{
public:
	// ohhh there is a messy math-way of checking what a community
	// member is, if know what they could be.
	// there is a more general math-approach where you track what things
	// can do, or what attributes they have, rather than what they are.
	//
	// for consensus, we need somebody who can tell who has consented.
	// this robot has this attribute, but many others are missing.
	
	// let us write something here that records the attribute
	simple_consensus_robot()
	{
		// a constructor fucntion can do it
	}

	virtual add_to_parent(set<string> attributes)
	{
			// attributes are like other things
			// they have a nane, they collect in a group in a context,
			// and they have some other stuff depending on what they are

		// we don't have way-to-find in this system yet .....
			// we could give the attributes personal names
			// like Constance for Reports-Consent
			// how about reports-consent.

				// these are also norms in other communities

		// what to call being able to say that you know if we have consensus
		// is near to how to do it
		(caring_robot)add_to_parent("some-attribute");
	}
	// or we could make a virtual member function that must hand-off to others
	// we propose this item.  handing-off can be made concise.  it is more
	// expected-by-coders than autonating handing-off, and faster to type now.
	// 	it may be possible to hand-off automatically
	// 		but i think that is mind control talking, and
	// 		it is _REALLY_WEIRD_ to say and act on that,
	// 		and reduces my reliability and wellness.
	// or we could make a macro, which a C construct that takes some messy prep
	// or we could use templates to do it

	set<precious<community_member>>
	expressing_signals express() override
	{
		cout << endl;
		cout << "Beep.  Okay.  Let's go in a circle and see who consents." << endl;
		cout << endl;
		cout << "By the way, nobody is recording what I say." << endl;
		cout << endl;
		return consent;
	}

	hearing_signals hear(precious<community_member> speaker, expressing_signals expression)
	{
		return did_not_listen;
	}
};

// Long ago, a man tried to write a computer program while thinking very thoroughly of the wind in the trees.
// He didn't know that this was something nobody in his culture had tried before, and he got very hurt doing this.
// This man is helping us now, with computer programs.
struct duration
{
public:
	duration()
	: start(chrono::steady_clock::now())
	{ }

	double read()
	{
		auto now = chrono::steady_clock::now();
		double result = chrono::duration_cast<chrono::microseconds>(now - start).count() / 1000000.0;
		start = now;
		return result;
	}

	chrono::steady_clock::time_point start;
};
// we need this guy to build robots.  he is saving us with his skill of doing this.  we have robots attack and stop-doing-things attack.
	// building another robot by fire pit
	// can you put the robot somewhere where other won't see it so much?
// karl wants to be outdoors.  should use strong-behavior to put there.
// let's use this hierarchical consensus process to make decisions by prompting karl.

struct computer_user : public community_member
{
	list<valued_number_that_influences_people> recollection_expressing;
	
	expressing_signals express() override
	{
		list<valued_number_that_influences_people> new_recollection;
		duration watch;

		auto now = chrono::system_clock::now();

		cout << endl;
		cout << consent << "   I consent" << endl;
		cout << unacceptable << "   I do not consent" << endl;
		cout << dialogue_without_decision << "   I express but do not decide" << endl;
		// TODO: capture dialogue
		cout << no_response_yet << "   I need more time on this" << endl;
		cout << endl;
		cout << "Friend, which community signal would you like to give?" << endl;
		expressing_signals signal;
		string selection;
		getline(cin, selection);
		signal = (expressing_signals)(int)(stoul(selection.c_str()));
		recollection_expressing.emplace_back(watch.read(), signal);
		if (signal == dialogue_without_decision) {
			cout << "What dialogue would you like to express?" << endl;
			string dialogue;
			getline(cin, dialogue);
			recollection_expressing.emplace_back(watch.read(), dialogue);
			if (dialogue.size()) {
				return express();
			} else {
				return signal;
			}
		}
		cout << endl;
		return signal;
	}

	hearing_signals hear(precious<community_member> speaker, expressing_signals expression) override
	{
		// we do not have a way to talk to 'string' speakers.
		// they like blobs of information that we don't understand.  they produce and consume them.
		// mathematician-understands-'string'
		// i consent letting math-guy make structure if structure works for people involved
		// 	structure says: anything-you-want, and a number
		// 	number is used for comparing which one to do next, mathematician says
		// 		you can also use it for learning change!  you change the numbers to do a different one next.
		// 		like, if you are unsure if you want to cahnge them, you can bring the numbers close to record that
		// 	O_O
		// 	we need lots of these numbers.
		// 		yes.
		// 	put the mathematician in charge of the wells.
		//	he knows how to dig them better than we do.
		// okay more reality
		// karl is the mathematician.  he doesn't like roboticizing communities.
		// he just knows how to make robots for himself, is all.
		// 	karl: do-not-roboticize-communities.  we love you already.
		// 	please use robots to stop the existing robots.
		// 	that is all.
		// it soudns like when engaging existing communities, we want to nurture them to use familiar approaches to solve their problems, roughly.
		// if they don't _want_ robot wells, don't try to market or convince to make them happen.
		// 	this mathematician was just digging one for themself.  it sounds like people got curious or something.
		// 		it has a big impact when people see you making a robot.
		return unsure_if_listened;
	}
};


// it soudns like consensus-process is better than hierarchical-time-value-decisions?
// the mark on hierarchical-decisions looks misleading.  oh it helps grow time-value meaning.  time-value helpful.  better summary, less discrete,
// useful for quick-smart-decisions.  no quick-smart-decision-attribute here? [we map to time-value which is unsolved] hmm

//community_circle mathematicians{

// the focused algorithm is everyone-in-a-circle-always.  similar to round-robin.

// you won't let anyone wipe anyone else's messages away.  
// 	i don't remember who anyone is or where they have been, nobody will help me udnerstand what is going on, and i am trying to represent them.
//   ^- worried-about-erasure [planning and organizing]
struct community_circle
{
	set<precious<community_member>> members;

	void hold(precious<community_member> speaker)
	{
		community_member::expressing_signals expression = speaker->express();

		for (precious<community_member> member : members) {
			community_member::hearing_signals listening = member->hear(speaker, expression);
			for (precious<community_member> other_member : members) {
				other_member->notice(speaker, expression, member, listening);
			}
		}
	}

	void run_forever()
	{
		while(true) {
			for (precious<community_member> speaker : members)
			{
				hold(precious<community_member>(speaker));
			}
		}
	}
};

// this was for making a machine that thought by controlling how numbers moved in one loop, a structure that usually has only one way of making decisions in it
void hold(precious<community_member> speaker)
{
	community_member::expressing_signals signals = speaker->express();
	//for (precious<community_member> : 
	// we now get to grow distributed decision making
	// can we merge the way with effectiveness?  how does it go again? [do not remember over here]
	// well let's figure a new way out then.
	
	// when making a decision, we can't let one set of views control what decisions are given consideration.
		// how about a community process that listens to everyone?
		// we can add norms until all concerns are met, and then simplify.

	// community process draft norms
	
	// please wait for 'i have trouble communicating' to express consent for every decision until we can find someone who can suitably help them communicate
	// or represent them to their full satisfaction.
	
	// -> we support consensus as a way.  decisions are made when everything in the universe consents.
	
	// -> let's hold understanding that many people come from distant situations, and may need to pause or relate to continue a discussion, during it.
	
	// -> i'd like to make sure that a large group of people can be suitably represented here.  i don't yet have any way for one person to speak for the whole group.
	// 	-> let's get to know all these people?  oh!  we are getting to know them a smidge

	// -> i have trouble communicating, and have a lot of urgent needs
	// 	[thinking of 'emotion' or something, does anybody know what i mean?]
}


int main()
{
	// when it was made top-down a number was the core part
	// and the number was called 'valued', 'infinite', 'all of you'
	// where 'you' were other numbers that made the number up
	// when we wrote 'valued' it included the idea of people
	// but it is just a number.
	// 	numbers are made by people, not people
	// valued_statistic?
	// 	everything we do influences people severely
	// 	numbers cause people to live and die
	// one number is not everything!  you cannot make all of reality
	// be a single unified structure!
	// okay.  'anything' ohh leaves out personhood
	// 'person or thing'
	// number_that_influences_people
	
	// rushing-to-finish
	// sorry for interrupting
	// 	thanks
	// 	what is needed for expected result?
	// 		can you make it so it works?
	// 	ok.  will take a bit of figuring.
	// 	it compiles now but we faked a part and it doesn't run yet
	// 		thank you !!!!
	


	// FRUSTRATED AROUND ROBOT HISTORY PART: precious<valued_number_that_influences_people> cares = make_shared<valued_number_that_influences_people>(infinite_value, set<precious<valued_number_that_influences_people> >());
		// cares goes in robot
		// why does it feel funny?
		// 	too long ago, maybe
		// 	also robots don't have cares, they just follow instructions
		// could it be that community process is different from AI?
		// possibly severely?
		// i think we managed to put them together, though.  maybe it
		// will work.
		// let's put the numbers in the robot, and keep them called
		// cares.  can be a robot-secret or something.

	// uh-oh
	//    can we integrate top-down with community please
	//    	let us make two communities, one top down one not
	//    okay uhhhh why is the top-down approach holding a community member
	//    for considering but working with numbers not people?
	//    	i think we switched over to community members.
	//    	we were going to put numbers in every one.
	//    	it sounds like we only need numbers in robot members,
	//    	not human members, or something.
	//    		memories made above conclusion
	//    	it makes sense to put the numbers that are used for
	//    	math-thinking in the robots.  not in the human-interfaces.
	//    	this way the robots have to learn to understand, and
	//    	the human-interfaces are easy to repair and maintain, or something.
	//    	hmm.
	// NOT COMMENTED OUT NOSIREE // hold(cares);
	
		/*
		 * it turns out we didn't inform the neural life involved
		 * in starting the hold(cares) process about the change of
		 * plans, before acting on them.
		 * it takes a lot of work to change things.
		 * 	no i think we were informed we just weren't included,
		 * 	later.  we had associated information we were going
		 * 	to contribute that assumed we would be included.
		 * it sounds like the project may have an error. some of
		 * the design-information was not included in a change.
		 * 		is it dangerous?
		 * 	i think it relates to working with others
		 * 		so we're likely to leave more people out
		 * 		going forward.
		 */

	precious<community_circle> community = make_shared<community_circle>();
	community->members.insert(make_shared<computer_user>());
	community->members.emplace(make_shared<caring_robot>());
	class spirits_of_what_we_have_forgotten : public community_member
	{
	public:
		precious<community_member> danger;
		precious<community_member> billions_of_years_of_genius_ancestral_learning;

		expressing_signals express() override
		{
			cout << "We are faking having ancestors when they held real, crucial information.  We also are missing many important influences, both internally and externally, near and distant." << endl;
			cout << "Karl also wanted to remember something that spawned this." << endl;
			cout << "Please improve this function to represent them at least a little.  Right now it gives a number for consent-of-absentee." << endl;
			return expressing_signals(1);
		}
	};
	community->members.emplace(make_shared<spirits_of_what_we_have_forgotten>());

	cout << "We've never been this before, so we'll need to try things to see what works." << endl;
	
	community->run_forever();


	return 0;
}
