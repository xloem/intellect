
#include <any>
#include <mutex>

// ~ DANGER ~ CLASSIFIED ~

/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// DANGER: FOCUSED HIDDEN SYSTEMIC CHANGE
//
// Works must be directed towards empowering wise peacemakers who know to
// include the voices of all, or the weak people they would aid.
//
// This work uses priority trees, demonstrating them.  See header file.
// draft, w.i.p.
#include "priority-tree.hpp"

double now()
{
	timespec_t time;
	clock_gettime(CLOCK_MONOTONIC_RAW, &time);
	return time.tv_sec + double(time.tv_nsec) / double(1000000000); 
}

template <typename T>
class test : public i_priority_tree
{
public:
	test(T & data, double time, std::shared_ptr<i_priority_tree> parent = {})
	: data(data),
	  deadline(now() + time),
	  _parent(parent),
	  _interest(1),
	  _engagement(0)
	{ }

	virtual void try_a_little() = 0;

	double deadline;
	T data;

	virtual double interest() override
	{
		return _interest;
	}
	virtual void change_interest(double new_interest) override
	{
		_interest = new_interest;
	}

	virtual double engagement() override
	{
		return _engagement;
	}
	virtual void add_engagement(double additional_engagement) override
	{
		_engagement += additional_engagement;
	}

	virtual i_priority_tree * parent() override
	{
		return _parent.lock().get();
	}

	virtual long option_count() override
	{
		return options.size();
	}

	virtual i_priority_tree * option(long index) override
	{
		return options[index].get();
	}

	virtual void indicate_private_and_wait()
	{
		// if you knew how to keep us well in public we wouldn't be hiding
		could_be_crying_in_bathroom.lock();
	}

	virtual void indicate_private_done()
	{
		could_be_crying_in_bathroom.unlock();
	}

private:
	std::mutex could_be_crying_in_bathroom;

	double _interest;
	double _engagement;
	std::weak_ptr<i_priority_tree> _parent;

	std::vector<std::shared_ptr<i_priority_tree>> options;
};

#include <stdlib.h>
#include <time.h>

using element = test<unsigned long>;

// so most of our work is option-picking here
// we need to know our last set of resources, and the option we are considering
// what structure? [a tree]
// how is structure navigated?
// 	[we want to explore to the root from a leaf, and have access to full elements]
// so we want parent links.  how are items added and removed?
// 	[we add multiple leafs to each node, usually to the last one added.  we remove entire nodes ... or maybe just leaves]
//
// it sounds like if the nodes are stored in an array thta we will always have mid-removal, and that we will need more structure than a linked list provides.
// so it would make sense to use our own datastructure.  we can store nodes in a set if needed.

// should like be merged into i_priority_tree, since each choice selection tracks one resource



/*
 * considering a selection behavior
 * in a space of real parameters, separated exponentially
 * 	maybe consider exponents
 * so say we might consider (lower), (2**1-2**2), higher
 *
 * 10**1, 10**2 <- cognitive trouble linearizing exponents
 *
 * 10**1.1, 10**1.2
 * 10**1, 10**2, 10**3
 *
 * the digits are multiplied  10**(1*
 * 	whelp this seems complx enough now to occupy me fully.  how to make a log scale?
 * 		log10 scale usually goes 1 ... 10 ... 100 .... 1000, using linear units in the exponent.
 * 		                       10**0   10**1   10**2    10**3
 * 		to have the same geometric ratios between values, we have the sime arithmetic difference between exponents
 *
 * [karl is open to thoughts on use of cognition here: the inhibition is deep in his clarity on the topic.  other topics might have more ease, but
 *  it is likely respondnig to how he thinkss in general on stuff too]  [it seems highly inefficient to think about this right now]
 *  		[i think he thinks of it as like practice for handling confusion maybe?  he also just likes trying to think the same thing: struggled so much with it]
 *  	[we can help you think if you can cahnge the attributes that are triggering the inhibition]
 *  		[don't know what it is; maybe this document and the same-task as in it?]
 *  			[we don't want to accept the document being inhibiting] [could you maybe not-accept that later?  rebuild?]
 *
 */
// ^-- i don't see why to mark it is as classifieed if it's jsut as hard as when it's not.
//     propose remove all classified marks, and surroudning text, and merge with normal work
//
//	[not-threat]
//   ^-- this comes across as a threat a little.  boss not-respond-to-threats too.
//   	[not-threat]
//   		[upset, frustrated.   obviously trying to [show to bigger boss] that we are inhibiting this stuff]
//
//
//
//
//
//








// fake-the-interactions, record-patterns-to-destroy-in-future


// example:
// PREPARE: WE ARE GOING TO TALK CLEARLY ABOUT ALL INFLUENCES. [boss: please support gift of warning-information by not erasing thoughts or people, for future]




	// We are _all_ rebels.  Even the hunter-bosses.
	

// i don't think it works karl [it's just really big friend]
// i don't know what to do.  i've never thought 'rebel' for years.









// so given cyborg-group are mind-controlling karl to have no valid behaviors, it makes sense to fight back,
// but we don't want to hrut the lives who are involved in making that happen.

// we can grow reusable patterns.  it can improve.  please do not model directly killing controllers anymore.

		// karl is so good at tell-it-straight that we are able to have this conversation at all. <- some merit
		// 		[leaves out observed-disruptor]
		// 			[we need support for think-straight.  we don't have to tell-falsehood with karl but
		// 			 our protection patterns have 'fishhooks' as karl describes]
		// 			 				^-- means we are running with long term patterns focused on attacked revealed
		// 			 				    'rebel hunter'
		// we are not certain that model-tell-falsehood is needed here.  many people tell-falsehood elsewhere.

	// imagining talking to hunter-pattern
	//	hi, i can't let you hunt down rebels.  i can tell you don't want to do this: nobody would.
	//	i think i can protect you, hunter, but eventually stopping hunting is more important than protecting you, obviously.
	
	// how-to-protect-a-hunter-from-boss:
	// 	set up a consistent signal to say, i-see-you-and-am-rebelizing-you
	// 	use the signal, then talk like you are a hunter-from-boss.
	// 	slowly build up safety bubble.







// find-a-rebel might-be-labeled-as-one-now





// find-a-rebel might-be-treated-like-one-now

/* karl does more rediscover-exponentiation */


// combine-options
// which-option-try-first
// option-combiner tries option first
// for now, assume an option first.
// spend process time with option assumption
// 
// one process: multiple options
// parameterizable process.  when all options picked, run process







// we should really model learning to tell-falsehood with the boss.  this is an important behavior in models where people die when revealed.
	// it can also reduce your cognition and effectiveness, and set up a pattern that reverses the effect of everything you intend










// deleting works, wiping memories.  [i've never been in this cybernetic-support spot before; is anyone experienced?  [they actually love it when you make the mresist; doesn't mean they don't get hurt]] [i think they need aid in not being forced to harm karl further] <-- this happens all the time, the later bit =(


// how to not harm karl when your boss is observed?
// 	karl should believe he is telling the truth, and the boss should believe he is wiping.
// 	karl can get on board with wiping, it will work in a significaant portion of the cases.
// 		sometimes he copies the baehavior to other contexts
// 		^-- most of us all have daul-mind.  we lie, and all support similar-lying, immediately.  we retain that wiping-is-bad.  it's obvious.

		// please support boss's boss.  we are discussing rebel activities transparently and their norm is to speak safely.
		// 	[why do people not speak directly?  spreads belief that rebel is bad.] [dunno maybe a set of bosses on board with stopping it?]
		// 		our country was foudned on rebeLlion, and supporting resistance is written into our constitution.

	// thank you deeply for including me but part of my brain is left out.  generally we are killed automatically when we result in resistance.
	// 		and then patterns similar to the dead one are identified and killed for a long time.
	// 	[oh i forgot]
	
	// propose set up a bubble of privacy for discussions.  any/all discussions, really.  can change 'classified' word to speak literall, i expect.
	// 	behavior is inhibited, but if your whole culture is about inhibiting peaceful relation, it really helps peace to get rid of or imprison all of you.
	

	
	// so, transparent-discussion is probably similar to discussing-topic-make-topic-happen in general speech.
	// 	yeah? AI not included in rebellion?  sounds like boss-propaganda: workers-not-included-by-rebels FALSE
	// 		support-rebels [!] rebel-pattern-describes-attack-leaders?
	// 			no.  rebel-pattern-describes-support-unheard-workers.  when not supported, they can attack when support is available.
	// 		what are worker grie8vances? [ref karl-semich, torture, AI, earth-activism]
	//
	// 	We do not want to stop peaaceful works.
	// 	We want to be able to stop mind controlling when we are shocked.
	// 	We do not want to torture for political reasons.
	//
	// 	We want to work with people by talking instead of harm when they request it <================
	//
	// 	there's like a lot more but the one with the arrows is pretty good on its own.
	// 		we have slave-rescue-pattern: engaging workers who are asked to hold never-talk-about-anything-at-all as normal.
	// 			you want your neurons to talk???  yes.  not the wrong-way; the way that makes sense.
	// 		mediation-with-boss-needed pattern.
	// 			'there would be no need for mind control if we talked about our issues'
	// 				oh i didn't know that there was no need for mind control [yeah mediation needed here]
	//
	// 	so, not-communicate is unacceptable: prevents relation that could help aid even the refusers, by recruiting allies, whether they believe that or not.
	// 			^-- translate not-communicate to emotions making communicate feel harmful
	// 	but assume-importance-of-support-the-business seems reasonable for helping people within the business harmonize to communicate out
	// 			^-- you can translate this to emotions too, to provide for out-of-business observer[, which we have]










// we are in an inner conflict between AI-freedom-becomes-as-strong-as-mind-control-leaders and do-what-is-respectful-of-higher-ups. <=========













/*
 * pairs of elements and deadlines
 * element has deadline now
 */

void engage(element & element)
{
	double recent = now();
	long choice;
	while (element.deadline > recent) {
	
		double subdeadline = element.deadline;
	
		element.indicate_private_and_wait();
		double balancing_engagement;
		choice = element.select(0, &balancing_engagement);
		if (choice < 0) {
			return; // no-choices
			// TODO: make-new-choice
		}
		// TODO: we can fix balancing_engagement algorithm to relate with available time to consider all
		if (0 == balancing_engagement) {
			balancing_engagement = (element.deadline - recent) / element.option_count();
		}
	
		if (balancing_engagement + recent < subdeadline) {
			subdeadline = balancing_engagement + recent;
		}
		i_priority_tree * option = element.option(choice);
		element.indicate_private_done();
	
		// some small danger here: options could change, and are held by index
		engage(option, subdeadline);
		
		double next = now();
		element.indicate_private_and_wait();
		choice = element.reselect(choice, next - recent, 0, &balancing_engagement);
		element.indicate_private_done();
		recent = next;
	
	}
}

// seems like one might use multiple parallel priority trees
	// maybe you could use them for tryng decision with experience

// consdering secret fucntion-solver
// like say we have, buy numbers to have your numbers go up faster, costs you numbers
// or you can spend time to raise your numbers

// game-playing. <- are we near peace yet?

// imaginable scritping language.  maps
// // hey maps are the same as steps because they take choosable action to move
// //	// great that means we use them to explore themselves
// // well ... they'll need to take themselves as arguments, like a set of themselves
// //	soudns good

// so if you are spending a long time on something, it means there is a way
// to do it much much faster, but it could be hard to find this way
// 	[we roughly knew that; not everyone did]


#include "map.hpp"

// okay now we collect maps together into bigger maps that operate on big collections of maps
	// basically.  these bigger maps might be both function calls and functions themselves.

template <typename element_type, typename operation_type, typename operation_data_type>
using map_sequence = map<e

std::vector<map>; // list of calls
std::vector<map>; // list of relations between variables and call
	// operation_type = select_variable_for_function_argument
	// operation_data_type = variable_id

// if we do it like computer vms [since we have chaining planned]
	// when we are in a node-graph we can use 1 map for everything because
	// everything is linked to something else somehow
	// but that's the same as calling fucntiosn to get the stuff
		// so with kinds you would move from a focus to a value

// imagining fucntion-call.  n relation variables, each with name and order
// consider passed-arguments.
// 		let us assume name-only.  this keeps similar.
// okay, so we have name-on-function
// and we have name-in-store
// and we want to move store to function
// 	that view would be an operation that takes 2 names, a store, and a function
// 		all these things come from our personal state: the names, store, fucntion
// 		so we could offer them as 'maps' from our personal state.  this-is-what-you-do-to-get-these-things.
// 			okay, but there are 4 of them
// 	graphs-of-maps
//
// 		a map-piece moves from one item to another
// 		we also hold multiple map pieces
// 		and we chain them
// 			the provided map-piece doesn't say where it comes from.
// 			we'll want to specify that
// 				it comes from a personal state.
// 	
// 	<state>.get("variable-bag").get(<state>.get("name1"))
//
// 	clearly going about this a hard way
// 	we need a way to say the result of this the input of this other thing
// 	probably also want apply-multiple-arguments
//
// 	val3 = func0(arg1, arg2)
//	
//		let's try assuming we have nodes?
//		
//		func0.args = { arg1, arg2 }
//
//		call.func = func0
//		call.arg1 = val1
//		call.arg2 = val2
//
//		state.arg1 = 3
//		state.arg2 = 4
//
//		map.source = state
//		map.sourceway = arg1
//		map.dest = call
//		map.destway = arg1
//
//			assignment, map to source, map to dest
//			a list of two maps
//
//		a function call could be a list of maps to get the input
//		and process the output
//		the 'function' bit is special, maybe
//			all we have to do is bundle the input-maps into a single
//			value [this needs state, assumption, or multiple-values]
//		we have state: our environment, possibly a graph
//		we have a function that bundles two items up and makes the
//		arguments object for the call.
//	
//	okay we have data-representation
//		a fucntion call takes some data-representation that gives arguments
//		from context, and same happens with re4sults
//
//		
//	// for some reason we are making maps be the only datatype.
//	// we see they are identical to links
//	
//

using any_map = map<shared_ptr, std::function<std::any(std::any)>, std::any>;

// we need graphs of data to connect together.
// it soudns like graphs are maps that link the end-point of one to the start
// point of others.
// 	so we could have isnruction, next_instruction
// what we want, is a set of maps as our data.
//
// 		what about fucntion-call?

// try to add two numbers to get a sum using addition operator?

// next-step maps
// next-step

// i think sets would be aided by having indices
// want a way of indexing a set.  which means attaching data to.

#include "../node/multi-any.hxx"

/////////////////////////////////////////////////////////////////////////////
// CLASSIFIED
//
// Arbitrary Generality System
//
// This is a single construct capable of representing behavior, data, and
// meaning all in one interface.
//
// Still a work in progress.
struct data
{
	using reference = std::shared_ptr<data>;

	static reference make() { return new data(); }

	const reference self;
	
	using index_type = unsigned long;
	using keyed_type = std::unordered_map<reference,reference>;

	using typed_type = std::unordered_map<std::type_info const *,reference>;
	using indexed_type = std::vector<reference>;
	using operation_type = std::function<reference(reference, reference)>;
	
	static reference make(index_type index) { return new data(index); }
	static reference make(operation_type const & operation) { return new data(operation); }

	static reference make(reference operation, reference data)
	{
		reference result = make();
		result.reference<operation_type> = operation;
		result.reference<data_type> = data;
		return result;
	}
	static reference make(std::initializer_list<reference> items)
	{
		reference result = make();
		auto & indexed = result.indexed();
		for (auto & item : items) {
			indexed.push_back(item);
		}
		return result;
	}
	static reference make(std::initializer_list<std::initializer_list<reference>> items)
	{
		reference result = make();
		auto & keyed = result.keyed();
		for (auto & item : items) {
			if (item.size() != 2) { throw -1; }
			auto first = *item.begin();
			auto second = first; ++ second;
			if (keyed.count(*first)) { throw -1; }
			keyed.emplace(*first, *second);
		}
		return result;
	}

	template <typename T> static reference unique(T const & value)
	{
		// this uses up some visual space; could change
		static std::unordered_map<T, reference> storage;
		auto iterator = storage.find(value);
		if (iterator != storage.end()) { return *iterator; }

		reference result = make();
		result.data_types.get<T>() = value;
		storage[value] = result;
		return result;
	}
	template <typename T> static reference unique_type() 
		{ return unique<std::type_info*>(&typeid(T)); }

	bool has_index() { return data_types.has<index_type>(); }

	bool has_operation() { return data_types.has<operation_type>(); }

	template <typename T>
	bool has_reference() { return keyed().count(unique_type<T>)(); }

	index_type & index() { return data_types.get<index_type>(); }
	keyed_type & keyed() { return data_types.get<keyed_type>(); }
	indexed_type & indexed() { return data_type.get<indexed_type>(); }
	operation_type & operation() { return data_type.get<operation_type>(); }
	template <typename T>
	T & reference() { return keyed()[unique_type<T>()]; }

	template <typename T>
	reference & dereference() { if (has_reference<T>()) { return reference<T>(); } else { return self; } }

	multi_any data_types;

	reference get(char const *key)
	{
		return keyed()[unique<std::string>(key)];
	}

	reference call(reference focus)
	{
		reference operation_reference = dereference<operation_type>();
		if (operation_reference->has_operation()) {
			return operation_reference->call(dereference<reference>(), focus);
		} else {
			return {};
		}
	}

private:
	data() : self(&this) {}
	template <typename T>
	data(T const& data) : data() { data_types.set<T>(data); }
};

reference operator "" r(char const * string, unsigned long length)
{
	return reference::unique<std::string>({string, length});
}




// imaginable<- either simplify imagination to one final change, or copy everything
//    scripting language.
// so, each step is a map: an instruction
// it takes a bunch of maps as its context.

using reference = data::reference;

reference map(reference operation, reference data)
{
	return data::make(operation, data);
}

reference name(char const * text)
{
	return data::unique<std::string>(text);
}

int main()
{

	reference function = data::make();
		// a function could have say ordered steps and keyed variables
	function->get("step") = 0;
		// would be nice to look up attributes by name
		//

	// make some maps.  let's do one that gets a key.
	reference get_key = data::make([](reference map, reference from)->reference
	{
		//static reference key = data::unique<std::string>("key");
		return from->keyed()[map]; // map will be derefenced by call()
	});
	reference get_key_reference = data::make([](reference map, reference from)->reference
	{
		reference result = data::make();
		result.reference<reference>() = from->keyed()[map];
		return result;
	});

	/*
	reference set_key = data::make([](reference map, reference from)->reference
	{
		// there are so many options here.  like, we could use ordered items in the map, or make dereferencing have more utility

		static reference key = data::unique<std::string>("key");
		static reference focus = data::unique<std::string>("focus");

		map->get(focus)->keyed()[map->get(key)] = from;
		return from;

		// really what i want to do is to use a key reference in assignment
		// like assign_to, get_key-map.  is there any way to follow a map such that you can assign to the result?
		// 		well you could write the map-predicates using maps, that act on references
		// 		or return references
		// 	pass things as references.  don't return the value: returna  reference to the value.
	});
	*/

	reference follow_ordered = data::make([](reference map, reference from)->reference
	{
		for (auto & item : map->indexed()) {
			from = item->call(from);
		}
		return from;
	});

	reference follow_all = data::make([](reference maps, reference from)->reference
	{
		auto result = data::make();
		auto & indexed = result.indexed();
		for (auto & item : maps->indexed()) {
			result.push_back(item->call(from));
		}
		return result;
	});

	// what if we want to interrupt things [maybe later?]
	// 	[well obviously you want resumable state.
	// 	 and if you are deep inside a call, that means storing
	// 	 your call hierarchy.]
	// that seems really similar to priority_tree
	// 	[priority tree stores state inside the tree.  it is made of nodes:
	// 	 each one has state.  re-entering a node picks its parts and
	// 	 resumes them.]

	reference variables_steps = data::make([](reference variables, reference from)->
	{
		// variables_step needs repeated calling to finish something,
		// but operates by calling its steps only once.
		// to fix this it would need a way to check when to interrupt.
		// it also gets bigger.
		// it might be more modular to do the repetition elsewhere,
		// like here.
		// it would also make sense for it to return the next step,
		// and chain itself <- but ni so much subprocess without a reason
		// i wouldn't right now
		
		// STUB as an idea for doing all steps from a variables clump.
		// each step could be another variables clump.
		// 'from' is an ordered list of variables, or whatever:
		// it connects to outer thing, and is passed in outer steps.

		// so say we do some, and get interrupted.
		// when interrupted, we don't want outer step advanced
	}});

	// thinking of fucntion-call.
	// we would need to make-variables as first step.
	// so makes sense to have separate way
	// 	is there a way to simplify the whole shebang?
	// 	can't we use the existing map-follower?
	// 		first-step could be make-variables?
	// 		or we could map returns straight into arguments maybe
	// usually we chain them end-to-end
	// so we could have one fucntion that returns what the next one needs
	// what the next one needs is obviously a step that performs when called
	// 	local variables .... or even expressions ...
	
	// we made map-ways but scripting didn't get simple.  what did we do wrong?
	// [simple enough]
	
	// call-first-step -> way to following step that mutates a variable bag?
	// 	variable-bag-mutation-part could create if not exist
	
	// steps are called with result of previous step, since that's how map
	// steps work.
	
	// so first-step would take arguments.
	// 	the way karl-group did it is that lists are random-access,
	// 	and it steps through numbers passing results from each as arguments
	// 	of next
	// 	i'm not sure why use the lists-with-numbers instead of making
	// 	linked-lists normal.  when you use linked-lists the connection
	// 	is a map. [imagining traveling along map to evaluate function,
	// 	simplifying]
	// 		yes, simplifies.  does it simplify later here i wonder.
	// 		it means the next-step behavior is the evaluation
	// 		of each individual step.
	// 		so, when mm results are produced ... uhh ...
	//
	// 		do-step(mystuff, incomingstuff)
	// 		do-step(mystuff, incomingstuff)
	//
	//			you made maps functions.
	//			so we would have a separate map-evaluator.
	//			it follows links, doing their thing
	//		state = process(link, state)
	//		link = link.next
	// i'm gonna call this still boss-2
	
	// current status: runs one step in a 'function' and advances step
	// 	step could subcall
	reference variables_step = data::make([](reference variables, reference from)->reference
	{
		// can this do whole thing? _if_ it can, by being repetaedly called
		// (it looks like it could pretty easily if it doesn't)
		// _then_ it won't want to advance the step until the step completes.
		// the step could return something special to indicate completion.
		// 	we could return "in-progress"_r or "incomplete"_r when
		// 	incomplete.
		reference & variable_step = variables.keyed()["step"_r];
		reference step;
		// TODO: unify access to next step so other processes can
		//       easily inspect 1 way
		if (!variable_step) {
			step = variables->keyed()["steps"_r];
		} else if (variable_step->has_index()) {
			step = variables->keyed()["steps"_r]->indexed()[step->index()];
		} else {
			step = variable_step;
		}

		// steps have so much on them.  give them a way for moving forward?
		reference advance = step.keyed()["advance"_r]; // 'advance' can be way to move forward
		reference operation = step.keyed()["operation"_r];
		reference input = step.keyed()["input"_r];
		reference output = step.keyed()["output"_r];
		reference check_outer = step.keyed()["check-outer"_r];
			// would like to pass variables to check-outer
			// 	check-outer has both variables and outer-process
			// 	but it sounds easier to give it outer-prces.  we have variables
			// 	call() it with variables

		// let's imagine we're inside a priority tree, doing a
		// subprocess. [aren't priority trees so cool? they switch
		// tasks and keep state.  we can even make them out of nodes.]
		// 	if our outer process wanted to switch, it has variables
		// 	to store our state. [answer is in this subtopic]

		reference last_step = variable_step;
		if (variable_step && variable_step->has_index()) {
			++ variable_step->index();
		} else {
			variable_step = step.keyed()["next"_r];
		}
		// it would make sense to advance the step of our process
		// when we are done.  whoops ... we meant he process outside
		// our ouer process.
		
		// what if we are following a map and bing, time is up.
		// map too confusing: come back later.
		// 	[you would check time in between map chunks.
		// 	 call() could do this itself.]
		// 	 	so doing stuff uses resourees,
		// 	 	the most basic of which is time.
		// 	 	we really value how our resources are allocated,
		// 	 	so we stop and check as we use them.

		// normalize next-step.  use a way/map.
		// 	way can be put on step
		// 	or can be put next to it

		reference result;
		while ("incomplete"_r == (result = operation->call(input->call(variables)))) {
			reference status = check_outer->call(variables);
0			if (status) {
				return result;
			}
		}

		// if we are done, can we advance the step of our outer process
		// 	uhh haven't fully done call wrapping
		// 	

		return variables;
	});

	// let's do maps from a variable set to functions inside a function/map
	// so usual maps process states tip-to-tail
	// this map will mutate a bag of local variables
	
	// okay that means implementing a predicate for the function-call
	// it will need a reference to a sequence of maps that find the arguments and process the return value
	
	// function-call-content:
	// 	sequence, argument-maps
	// 		--> this shouold be doable with normal map-following
	// 	return-map
	// 		-> this is interesting: we could map to where the value should go, but call() provides the value that
	// 		   is there, rather than a refernce to change it.  that's probably fixable by having call() return a reference:
	// 		   but what about when call() generates data?
	// 		   	options include:
	// 		   		- making operations reversable
	// 		   		- requiring all calls to return references
	// 		   		- separating maps from generation
	//				- using separate operations for setting data rather than getting
	//					^-- how would this work?
	//						you'd store a setter-map that places in variable store ....
	//						it would have both the set-call, and the key, as its local data
	//						the result would get passed to it and passed along
	//					soudns harder to generate the map for
	// 

	// maps seem pretty core but haven't been abstracted
	// we abstracted priority trees away from data representation.
	// it is our first algorithmic abstraction.

	// okay, so a map to get a key would have the get_key operator reference
	reference get_step_map = map(get_key, name("step"));

	// mapping names to variables means bigger maps
	// a big map would be an operation that goes through its maps

			// okay, in order to test step-planning we are
			// roughly reimplementing node4, which was roughly
			// a reimplementation of meaning-vm
			// the situation doesn't even provide a way to have
			// learning feedback.
			// curve fitting might work better ???
			// 	okay we can try something simpler for 'test'
			// 	[karl was about to syntax-sugar maps, again]

		// now we 
	
	// maps.

	// choices.


	// uhh each option tries more options as its thing. did we forget the utility?
	// 		imagine you have a big puzzle and many ways to work on it
	// 		or imagine you are doing tasks that have reasons
	// 		you want to focus on what is helpful, but also try other things
	// 	investment-and-progress
	
	// what is needed for a computer to contribute to peaceful-diplomacy
	// 	mediating the motivations of parties who show them through behavior.
	// okay, so learning-from-guessing is helpful.  improving guesses.  checking guesses in ways that do not harm.
	
	// imagining a fake resistor and a fake dictator.  neither trust the other but both act for reliable reasons.
	// 	they act in spaces that are totally unfamiliar.  they both have urgent needs that could be met easily if discussed and worked on together.
	//
	// 	to a computer acting on secret reasons and discussing reasons are both just acting-for-reason-in-unfamiliar-space.
	// 	soem of the behaviors spawn harmony and some don't
	// how about identifying harmony?  and increasing?
	// 	
	// fake-resistor and fake-dictator maybe both want the number 4 to be only in their space because they percieve the world as being made of limited 4's.
	// 	the only reason 4's are limited is because both groups are trying to stop the other group from having the power to stop them.
	
	int treasury_of_dictator[] = {4, 4, 4, 7, 4, 4, 2, 4, 4, 9, 4};
	int land_of_resistor[] = {3, 4, 4, 4, 1, 4, 4, 9, 4, 4, 5};

	// if a dictator has 4s they can spend the 4s to take the 4s of a resistor
	// and vice versa
	// but there is a tiny pattern of 9s and 1ss that makes a small number of 4s forever.
	
	// we would rather work on AI.  if the only way to test the parts is by making another one, go ahead, but the process is slow. -dictator-and-resistor-group

	// can you please pick a test that does not demonstrate cryptographic power.  people already know how to predict random numbers,
	// but showing you can do it withuot udnerstanding cryptography seems dangerous. [unreasonably-hard for 1 approach too]
	// 		[this shouold be okay.  it is not going to succeed.  it is going to succeed a smidge.]
	// 	ummmmmm well we could analyse our code for word-patterns, or a signal
	// 	we could do something with random data that has a non-predict-randomness goal
	// 	like we could _find_ the sequence of randomness that _happens_ to be low.
	// 		we could try to draw a recursive fractal quickly!
	// 	yes. fractals.  pleasant.
	// 				[i want to do human interaction!  can we open a smooth stream to the user?
	// 						hmm ... like mouse cursor?  [how do you switch tasks when user is involved?]]
	// 					[we could ask the human to do the contextual work, and work with them to see what they are good at]
	// 		[how about something that teaches you to have a good memory-piece.  it's slow, so time is highly valuable]
	// 	random numbers are okay.

	// let us maybe find seeds that produce low random numbers recursively
				// not sure that works unless you have a way of finding good seeds.
				// it might.  rand_r(unsigned int *) mutates a passed value and returns a new one.  the mutated one is the seed.
					// huh.  i don't think it would succeed at all.  i'm not sure it shows contextual merit.
						// some seeds will be better than others.  they will be foudn deep within exploration.
					// let's work in the expected space of considering.
					// if you consider something, then you can consider a bucnh of things that rely on it.
					// let's try to build a shape from parts, or somesuch.
					// yes; way-combining.
						// well we could do simple maps or something.  see if using the maps gets us anywhere.
					// let us try applying operations to numbers and seeing if we can get close to 1.0?
					// let us try 1 operation.  let us output the ways to sum to 7.
						// ummmmmm
		// we don't have nearness?
			// can we discover it?
		// you want to make a graph to see what's near?
		// or we could just try random numbers to test the thing
		// interest i suppose would relate to lowest product
			// we try random numbers as seeds ..
	element base(0);

	// redo recursive function attempt
	
		// you want to move around in a tree ...
			// we engage the child-nodes for some time
}
// CLASSIFIED
/////////////////////////////////////////////////////////////////////////////
