#include "bootstrap-perser.hpp"

namespace intellect {
namespace level3 {

// must support syntax-sugar

// LATER, UPDATE LEVEL-1: believe name lookup should happen by a function stored on thread context
// this function will likely use the thread context to do lookup, possibly hierarchically.

/*
 * PATTERNS ARE CODE.  If a result-map fails, an assumption was false.
 * We believe the core structure is built fully by including side-effects in patterns as
 * relations over time.
MISSING: goal-pursuit pattern matching as translation from a document to life-behavior.
we are planning to write core and parsing together.  parsing then moves through code towards
language, as has been expected.
-> interruptible pattern generator
	produces parts of a concept set that could be patterns relevent to it, as code-patterns
	is easy to discard ones not matching other sets.
-> match-checker
	given one or more assumed-similarities, identifies if other parts of a pattern match
	data, by running the pattern.
-> match-finder
	iterates parts of data to sje if matches are found
	this may require trying different predicates (steps with maps) inside the pattern, when
	those predicates/steps are variables the pattern implies.
	takes relevency function that may provide always/never/better relationship between checks
	can feed a queue
 */

// To generalize all parsing and facilitate efficient communication, we propose
// "information spaces".
// An information space is the domain in which a focus rests.
// It depicts the ways the focus may be referenced and moved, and its topic accessed.
// Notably, an information space may have arbitrary ways of moving a focus, and arbitrary
// incidences of those ways.  We would like to call this wayness, if that is helpful.
// information-space:
// 	- focus
// 	- focus-motion-wayness
// 	- focus-topic
// the information space is what the focus moves within ...  there could be multiple
// focuses within the same space.
//	an information space relates focuses to topics and each other
// focus:
// 	- information-space
// 	- motion-wayness (concept includes information-space)
// 	- topic

// character-stream
// line-stream
// whitespace-word-stream
// word-stream
// word-stream-within-parentheses
//
// word-stream-within-parentheses
// 	we value going through each word
// 	and for each word considering nearby parts of the information space,
// 	depending on what the word is.
// 	we have ways for going through each word, which depend on the surrounding information space
//
// 	considering whole-line and document-of-lines as information spaces
// 	we want to normalize a way to view these information spaces for text files
// 	and we want to generalize how that choice-to-view is recorded wrt being-an-information-space
//
//	the simple answer is, design for documents, label the document functions as
//	information-space-wayness for document-information-space
//	
//	so, here in parsing, we have word-meaning-context and parsing-context
//	both are information spaces
//	word-meaning is a map of text to conceptual references
//	parsing is a linear information space of text
//
//	word-meaning is an information space of concepts located within by textual words
//	the only way to find a concept is with a textual word.  there is no focus-motion
//		but a textual word generally spawns more, given the context of access
//
//	linear-text information-space
//		has-nearness
//		has-bounds
//		has-relative-traversal
//		does-not-have absolute-traversal
//		
//			and the use of hyphens above shows how spaces morph in different
//			contexts.  handling this will likely be needed.
//			has A
//			not-has B
//			is easy, learn has and not-has
//			with does-not-have and has-bounds
//			must learn parse-hyphen and does-not-A
//			and then evaluate with wayness to translate from one space to another.
//				this brings in the idea of process translation.
//					(karl has promises and structures)
//
//					we imagine the small information space of a list of
//					has-A
//					and the small information space of a list of
//					does-not-have B
//					these two spaces must come to agreement, mediate,
//					translate between each other, or both translate to
//					somebody else, when their combined information is
//					needed.
//						we ask for parallelism: a uniform translation
//						of what is relevent about those two spaces,
//						roughly.
//
//					uniform concept is <way: have-ness, topic>
//					we want a list of these
//					the processes handling the two spaces likely
//					know nothing about have-ness.
//					they likely know about the english language
//					a specific part, has and what
//					and that does-not-have is the opposite of has
//					really "does not have" is the opposite of "hash"
//					in one subcontext, words are separated by hyphens.
//					this hyphen separation
//						1. informs relevence
//						2. produces a new subcontext, like parentheses
//					has-alpha <- hyphen makes subcontext between words
//						parser knows has, attaches to alpha, makes
//						verb-phrase
//					does-not-have beta
//						parser can do work more rapidly due to
//						hyphen-means-subcontext
//						does-not-have 
//
//					what seems relevent is with space-parsing these
//					would form separate whole concepts.
//					the concept of does-not-have paied with topic
//					and the concept of has-topic
//
//					how we relate these appears at first informed
//					by use. but we'd like to make a bubble for this
//					kind of stuff, so arbitrary uses can often engage
//					just the bubble.  this will mean an information
//					space of the meaning of concepts.
//					then we can think be mediating these spaces, and
//					thought is formed rapidly.
//					
//					say we want to label the outer topic with all the
//					attributes, and remove al the ones that are not-has
//
//					we have assumptions and goal.
//					assume: each item can make <<has or not-has>, topic>
//					        no items cannot be fully representd by that
//					so we came up with a proposed language
//					for all the parts.  if the parts are not fully
//					represented, language will need to be improved.
//
//					someone else may come by and be all,
//						give me how many times you use 'not' in the file
//							assumption: in-a-file
//							goal: count nots
//					or
//						if i am described by you, give me what you know i am not
//							this likely will need another process to mediate, that understands the meaning of all the sides
//							assumption: each-item can-describe-same-thing
//							goal: has-not topics
//					This is CORE and involves making PROCESSES that TRANSLATE INFORMATION SPACES when GOALS ARE HELD.
//						reread a little, and try to continue.
//						we're generalizing parsing to thought concisely.
//						it looks valuable and promising.
//							suspect process-space may arise.
//							is goals a space?
//								goal is held focus within a space of goals
//							is goal needed/relevent?  or is it just a context?
//						contextual translation.
//						consider thinking kind of is translation
//						when pursuing a goal, you need a decision,
//						which is rooted in some kind of language of
//						reasons, whyness.  let's just assume that
//						thought is behavior and maybe even data is action
//						to make the decision, you need to translate
//						the relevent information to your reason-language
//						assumption: following-instructions
//						(assumption: holding-goal)
//						assumption: have-file
//						assumption: file-has-instructions
//						do-what-file-says-to-do
//							assumption: holding-goal
//							assumption: goal-is-get-keys
//							assumption: can-walk-to-keys
//								assumption: keys-in-walkable-space
//							get-keys
//						have-file-reference-in-local-information-space
//						have-way-to-access-file-information-space
//							have key-reference-in-thought-space
//							have nearby-pictures-in-vision-space
//							we find association between
//							what-is-near-me and where-my-keys-are
//								goal: find-keys
//								reason: not-guessed-relevent,can-find
//							
//							we consider goal-pursuit processes.
//							these processes value information of
//							the form goal-for-reason, where
//							reason is some other goal, such as be-careful-with-everthing.
//							
//							process needs behavior-helps-reach-goal
//							and information-helps-reach-goal.
//
//							one behavior that helps reach goals
//							is finding goals that might help,
//							poviding them with information that might
//							help them help,
//							and trying them out.
//							this is in a space of what-helps-reach-goals
// we want to translate between these spaces
// to reach our goals, providing the capacity for any to reach their goals,
// so long as they mediate with surrounding goals.
// way-to-reach-any-goal may be communicate-with-goal-management-process or with other goals,
// to form way-to-reach-this-goal.
// 	goals, reasons, and behaviors i suppose are roughly the processes that meet them,
// 	and the processes or information that spawn those processes.
// 	we could call them life, or life-parts.
// 	everyone is a life-part.
// 		we want to form a way-to-reach-this-goal, using parts from hte space of
// 		process-parts.
// 		this is mediation between our goals, and the rest of the world.
// 		we are satisfied the communication is complete, when our goal is met, or
// 		its reason is met.
// 			goal-pursuit is way-to-behave that is only relevent within bounds.
// 			goal-pursuit must only be used if could reasonably be relevent.
// 				goal-pursuit would mediate with that behavior space
// 					way-to-discern-relevency: ask-human
// 					never-do-something-always,all-behavior-has-bounds
//
//							we will need to discern where we believev the keys might be
//							move our body to that space
//							then discern where in that area the keys are exactly
//							and pick them up
//							to do this.
//							we'll want to relate with the reasons as we continue this process, in case it takes too long, in case we encounter something that makes the keys unneccessary, or in case it is incredibly hard.
//
//							we might consider some abstract assumptions such as
//							[when it is too hard or takes too long, stop <reason relates to when-too-hard, and what-to-do-instead>]
//							[information we encounter on our way, may make the goal no longer relevent <reason relates to when-to-stop and inforation-encountered-on-our-way>]
//							let's consider relating information-encountered-on-our-way to the other processes
//							we've never heard of information-encountered-on-our-way, we only know about keys, and about nearby spaces, and finding-things thoughts, our body and vision.
//							we'll need to find a process that can translate between information-encountered-on-our-way and _us_, if we learn this assumption is part of our experience, which it should be.
//							we exist in the space of processes; we are a process for finding keys
//							information-encountered-on-our-way is in the space of hyphenated text, or abstract concepts.
//							we'll need a way to convert the abstract concept into something we can work with.
//								so it's kind of like from the process environment, a work-specification is formable, and this work-specification would include must-relate-(information-encountered)-with-(reason-for-work)-for-work-to-be-valid
//								relating would mean communicating to satisfaction of both parties, here.
//								reason-for-work may be a secret process environment that holds need-to-get-groceries, car-is-way-to-grocery-store, need-keys-to-use-car.  if have-groceries changes, then all goals are met and everybody is given what they needed.
//								reason-space.  reasons are for
//								other reasons, but it takes time
//								to come up with all of them.
//									this space is locally navigable, and includes everything we do, possibly.  exploring the space of reasons seems a good way to function.  (how do goals relate to reasons?  goals are we hold as pursuing a task; reasons are why we hold it.  i'm getting keys to drive car.  i'm driving car to get groceries.  i'm getting groceries to eat.) (it sounds like goals are reasons, and have reasons) (goals are reasons for behavior; holding a goal and/or acting on it, is a behavior). behaviors are reasons because holding a goal is a reason.
//									goal/reason/behavior become interchangeable words when in a space of acting on goals.  since we exist in time, we propose living in that space.
//									acting-on-goal
//								when goal-pursuit, every goal
//								is to meet another goal.  the
//								reason.  if the other goal is
//								met, this one is.
//								so each goal could be labeled
//								state-met-or-reason-met
//								foremost goal is nurture-wellness,
//								listen-for-feedback, roughly
//	
//
//									to mediate the reason we need a shared language of relevency.  this is often simplified to trust.  but would expand to references to experiences of past success or failure that are similar to this one, roughly.
//
//								so, we may hvae some language of
//								what-is-relevent, maybe how-or-why
//								and some language of what-to-do, maybe how-or-why
//								we might have some processes that are treated as useful to try whenever something relevent to them is deemed relevent to a goal.
//								that's an information space of processes that act on abstract relevency, located within by the abstract relevency.  they use a shared language of relevency.
//							an extreme of the space of processes with shared languag is processes with shared signature, a smaller space.
//		we see a shared language is a context.
//		processes assume contexts, shared languages
//		everything assumes multiple contexts of shared languages
//			to think, we must translate these, by matching the assumptions of
//			other processes.
//				in the context of pursuing-a-goal, we always hold a goal
//			translating assumes a goal.
//				this goal relates to the assumed languages of the parts
//		processes with a shared language
//		everything has very basic assumptions, and these must be mediated between
//		to work between them
//		successful translation appears to imply all-parties-say-satisfied
//	we are expanding this long process with focus on quickly learning to think, after
//	expansion.
//
//
//
//	like all highly generaly things, there is no way to decribe all information spaces
//	with parallel data alone, kinda
//		'space' <- has location, roughly
//		'information' <- what is in the space
//
//
//
// 		this will let us reuse the concept of information space, in a way that is completely different, with analogy to textfiles.
// 		^-- this sounds too smart to easily provide for.
// 		(there is no need to do that for itself, but we are guessing it provides he best-guess at most-efficient relevent choices here.  it does not include smar-sounding-inhiited.)
//			this experienced pattern is based on karl's history of writing computer programs.  he was able to design complex reusable systems focused on a domain, in his head.
//				karl is assuming reusing 'information-space functions' is helpful
//					it looks like it could be, give slowness teaching
//
// let's not generalize to non-document domain anymore, for now, (eucalyptus-curve)
// 		it looks to me like the amount of non-textfile generality we've added now
// 		will spawn ideas around more generality
// 		in a way that is more relevent to continuing the work effectively
// 		than continuing to generalize now would be.
// 			we need few enough ideas for our time spent thinking about them
// 			to produce helpful enough conclusions


// BASELINE parsing;
// idea: read word, word is habit that is switched to.  continues handling itself until done.
// must have contextual ref to mutually influnce

// BOOTSTRAP parsing:
// propose expression-based, for intuition.  we can use '[' to open a sub-expression.
// statement expressions produce lists of statements.
// WHEN, HABIT, or PROCESS assign to values

// handlers for these words can handle the raw stream, or a set of words.  propose raw stream
// to reduce layers, for now.

// make above


ref parsevalue(ref stream)
{
	istream & ss = *stream.val<istream*>();
	string word;
	ss >> word;
	if (word.size() > 0 && (word[0] == '"' || word[0] == '\'' || word[0] == '`')) {
		char delim = word[0];
		string accum = word;
		if (accum[accum.size()-1] != delim || accum.size() == 1) {
			char c;
			while ((c = ss.get()) != delim) {
				accum += c;
			} 
			accum += c;
		} else {
			//accum.resize(accum.size() - 1);
		}
		word = accum;
	}
	return word;
}

// let's do contextual word lookup.  it will really ease stuff later.


// we use text for two things:
// 1. symbols and text
// 2. bootstrap-context references and concepts
std::string ref2txt(ref r)
{
	return r.vget<std::string>("name");
}
ref txt2ref(std::string t)
{
	return ref(t).get("name");
}

// we'll want a way to swap off expression-ctx vs statement-ctx in a way that doesn't
// provide for both at once.
// context-type parsing-category
// we can have special context-groups in the context hierarchy
// each one allowed only once
ref ctxlookup(ref context, ref item)
{
	std::map<ref,ref> exclusive_ctx_groups;
	std::set<ref> encountered;
	std::deque<ref> nesteds;
	encountered.push_back(context);
	nesteds.push_back(context);

	while (!nesteds.empty()) {
		context = nesteds.pop_front();
		for (auto link : context.links()) {
			if (link.type == item) {
				return link.target;
			} else if (link.type == "is" || link.type == "outer-context") {
				if (encountered.count(link.target)) { continue; }
				nesteds.push_back(link.target);
				encountered.push_back(link.target);
			} else if (link.type.isan("exclusive-context-group")) {
				if (!exclusive_ctx_groups.count(link.type)) {
					exclusive_ctx_groups[link.type] = link.target;
					nesteds.push_back(link.target);
					encountered.push_back(link.target);
				}
			}
		}
	}
	throw makeconcept().link("is", "item-not-in-context");
}

// what-distinguishes-words
ref parsewordtxt_ws(ref context, ref stream)
{
	string word;
	istream & ss = *stream.val<istream*>();
	ss >> word;
	if (!ss) throw makeconcept().link("is", "end-of-stream", "stream", stream);
	return txt2ref(word);
}

// this function can handle most stuff
void parseprefix(ref context, ref stream, ref prefix)
{
	try {
		while (true) {
			ref parsertxt = parsewordtxt_ws(context, stream);
			if (!ss) break;
			ctxlookup(context, parsertxt)(context, stream, parsertxt);
		}
	} catch(ref r) {
		if (r == "end-of-stream") { return; }
		throw r;
	}
}

// what-to-do-when-you-find-a-special-word
// what-to-do-when-you-find-a-nonspecial-word
// which-words-are-special
// context of specialness (expression ctx, statement ctx)

class ConceptException
{
public:
	template <typename Rs...>
	ConceptException(std::string type, Rs... rs)
	: r(makeconcept().link("is", type, rs...))
	{ }

	ConceptException(ConceptException &) = delete;
	ConceptException(ConceptException && other)
	{
		r = other.r;
		other.r = 0;
	}

	operator ref() { return r; }

	~ConceptException()
	{
		if (r) { conceptunmake(r); }
	}

	ref r;
}

class StreamSentinel
{
public:
	StreamSentinel(ref S)
	: ct(0), S(S)
	{ }

	ref value() { return S["do-value"](S); }
	void next() { S["do-next"](S); ++ ct; }
	void previous() { S["do-previous"](S); -- ct; }

	~StreamSentinel(ref S)
	{
		while (ct > 0) { previous(); }
		while (ct < 0) { next(); }
	}

private:
	ref S;
	int ct;
};

void loadhabits()
{
	// GOAL: provide for syntax sugar with ease

	// can we parse into a stream for infix and postfix operator handling that is loose
	// then can review previous items in stream.
	// 	let's make a stream construct.
	// 	we currently have a character stream.
	// 	we can pass it to a word stream.  previous-word, next-word.
	// 	for now it will load all words into memory to emulate this.
	// streams seem to enjoy get-entry, previous, and next
	
	// c++ streams have multiple ways of being parsed
	// keep-stream is for rewindable and peekable streams.
	// tokenization streams don't need this, nor do c++ sstreams

	aHabit("make-c++-stream", ((source, stm)), {
		result = makeconcept();
		result.link("source", stm);
		result.link("do-value", "stream-value");
		result.link("do-next-letter", "c++-stream-next-letter");
		result.link("do-next-word", "c++-stream-next-word");
	});

	aHabit("make-c++-word-stream", ((source, stm)), {
		result = ref("make-c++-stream");
		result.link("do-next", result.get("do-next-word"));
	});

	aHabit("make-c++-letter-stream", ((source, stm)), {
		result = ref("make-c++-stream");
		result.link("do-next", result.get("do-next-word"));
	});

	// We HAVE a stream, and we want to CHANGE what the delimiter is.
	// We want to GET the delimited values as a stream.
	// 1. make delimiting by hand
	// parsing generates parsed substreams, we're proposing.
	// 	to provide for midway work we'll want to allow for a bound to be undefined.
	// 	and to generate words from what-a-word-is as we move forward.
	//
	// parsing hierarchy ideas
	// 	letters
	// 	space-separated-words
	// 	endline-separated
	// 	until-a-delimiter
	// 	suddenly-stop-when-word-encountered
	//
	// 	we'll want to be able to define the bounds at any time.
	//
	// relation to contextual parsing (what set of word-handlers do i use)
	// 	we could make a parsing context match a bounded stream/list
	// 		it looks like we will have more bounded lists than contexts
	// 		e.g. to-end-of-line
	//
	// 		these bounded lists look a lot like expressions, statements, and
	// 		words [files, directories, steps in a conceptul task, function args].
	//
	// 		i-am-parsing,-what-keywords-are-interesting,-how-do-i-use-them
	// 			this-is-bounded
	// 		i-am-parsing,-what-defines-the-edges-of-a-word, where-does-that-apply
	// 			stream-interprets-other-stream
	//
	// 			context of expression in statement
	// 			expression-keywords like parentheses bound subcontexts
	// 			we're also in a file, which has letters
	// 			and in a process, which has instructions, roughly, each a line
	//
	// 			different behaviors
	// 			different data
	// 			different structures for the behaviors and data
	//
	//			contexts represent what is relevent to something within the context.  they say what stuff means for it.
	//			this is similar to a set of assumptions.
	//			we also have data that is relevent.
	//			roughly this forms a mapping of sorts
	//
	//			discrete context-part, is a little fluid itself
	//			context-part: assumption
	//			context-part: document,physical object
	//			document wayness:
	//				- location
	//				- focus-on-location
	//				- get-subpart-at-location
	//				- nearby-locations
	//
	//			information-space.
	//			reference to something within information-space.
	//			ways to transform that reference to get other things.
	//				may succeed, fail
	//			bounded-information-space.
	//			a text file is an information space of letters.
	//			a letter space.
	//			information within an information space.
	//			now all of the mind can be viewed the same as a file.
	//
	//			a stream is a focus within an information space.
	//			the information space is a linear view of a set of data,
	//			with a focus that can be advanced or stepped backwards.
	//			the focus has no inherent index, but it has a relative
	//			distance from the start.
	//					"information space" is a simple phrase
	//					for a simple concept
	//					that appears complex.
	//					it means how you can say where you are working
	//					within your work, and what it is you are working
	//					with there.
	//
	//				given we do not always know our bounds, using relative
	//				locations foremost is helpful.  can always use relative-to-start for absolute.
	//
	//			contexts should have type categories to their parts, eventually
	//				documents can be treated as a part of a context
	//				
	// 			information / document
	// 			bounds of relevence within the document
	//
	// 			we might have a document in our context
	// 			or a document-part
	// 			information, that the context is about
	// 				this information is complex
	// 				it lets you focus on a part of it
	// 				and it has a system for moving that focus to anywhere
	// 				additionally, this information is an interpretation,
	// 				a view, of other information, which has the same
	// 				property.  
	//
	// 			file-has-letters-and-file-functions
	// 			file-with-words
	// 			file-interpreted-as-white-space-words-context
	// 				consider the contexts like conceptual subcontexts,
	// 				when you start interpreting the file as having words,
	// 				you are imagining that this might be true.
	// 					maybe you know it is, in advance.
	// 					whether it is true or not, the new assumption
	// 					opens a subcontext where different behaviors
	// 					are used.
	// 			line-has-start-position, end-position, words
	//
	// 			propose using context for how-to-handle-words
	// 			and using subdocuments for handling lines, words, files,
	// 			brace groupings.  subdocuments have subparts that are near
	// 			each other, and bounds that may be unknown, and always a
	// 			reference to some part of the subdocument spcae to work
	// 			with.
	//
	// 		contextual information only applies to relevent domains.
	//
	// 		secretly, they're just another kind of context, when you can think that.



	aHabit("c++-stream-next-letter", ((source, stm)), {
		istream & ss = *stm.get("source").val<istream*>();
		char c[2] = { ss.get(), 0 };
		if (!ss) { throw makeconcept().link("is", "end-of-stream", "stream", stm); }
		stm.set("value", txt2ref(c));
	});

	aHabit("c++-stream-next-word", ((source, stm)), {
		istream & ss = *stm.get("source").val<istream*>();
		std::string s;
		ss >> s;
		if (!ss) { throw makeconcept().link("is", "end-of-stream", "stream", stm); }
		stm.set("value", txt2ref(s));
	});

	aHabit("make-parser-stream", ((source, stm), (parser, p)), {
		ref ret = makeconcept();
		ret.link("is", "parser-stream");
		ret.link("is", "stream");
		ret.link("source", stm);
		ret.link("parser", p);
		ret.link("do-next", "parser-stream-next");
		ret.link("do-value", "stream-value");
		ret.get("do-next")(ret);
		result = ret;
	});
	
	aHabit("parser-stream-next", ((source, stm)), {
		try {
			ref item = stm.get("parser")(stm.get("source"));
		} catch(r) {
			if (r.linked("stream") || r.isa("end-of-stream")) {
				r.set("stream", stm);
			}
			throw r;
		}
		stm.set("value", item);
	});
	
	aHabit("stream-value", ((source, stm)), {
		result = stm.get("value");
	});
	
	aHabit("make-keep-stream", ((source, stm)), {
		ref ret = makeconcept();
		ret.link("is", "keep-stream");
		ret.link("is", "stream");
		ret.link("is", p-ref("keep-stream"));
		ret.link("source", stm);
		ref entry = makeconcept();
		wordentry.link("value", stm.get("do-value")(stm));
		ret.link("entry", entry);
		ret.link("first", entry);
		ret.link("do-value", "keep-stream-value");
		ret.link("do-next", "keep-stream-next");
		ret.link("do-previous", "keep-stream-previous");
		return ret;
	});
	aHabit("keep-stream-value", ((keep-stream, stm)), {
		return stm.get("entry").get("value");
	});
	aHabit("keep-stream-next", ((keep-stream, stm)), {
		ref entry = stm.get("entry");
		if (entry.linked("next")) {
			stm.set("entry", entry.get("next"));
		} else {
			ref next = makeconcept();
			next.link("previous", entry);
			entry.link("next", next);
			stm.set("entry", next);
			ref src = stm.get("source");
			src.get("do-next")(stm));
			next.link("value", src.get("do-value")(stm));
		}
	});
	aHabit("keep-stream-previous", ((keep-stream, stm)), {
		ref cur = stm.get("entry");
		if (!linked(cur, "previous")) {
			throw makeconcept().link("is", "start-of-stream", "stream", stm);
		}
		stm.set("entry", cur.get("previous"));
	});

	// keep-stream of letters.  make keep-stream of words.
	// we have parser stream that can be wrapped in keep stream, but is confusing
	// is less confusing to make parser stream also be a keep stream.
	// 	we could use patterns here.
	
	// this doesn't note the bounds of the word, but does advance the stream from one end
	// of the word, to the other.  the bounds are kept in the before-state and after-state
	// due to the advancement, I guess.
	aHabit("whitespace-word", ((letter-stream, stm)), {
		StreamSentinel s(stm);
		std::string concat;
		ref letter;
		int count = 0;
		try {
			while (true) {
				letter = s.value();
				if (letter == " " || letter == "\r" || letter == "\n" || letter == "\t") {
					s.next();
					continue;
				}
				break;
			}
			while (true) {
				letter = s.value();
				if (letter == " " || letter == "\r" || letter == "\n" || letter == "\t") {
					break;
				}
				concat += ref2txt(letter);
				s.next();
				}
			}
		} catch(ref r) {
			if (r.isa("end-of-stream")) {
				conceptunmake(r);
				break;
			} else {
				throw r;
			}
		}
		if (concat.size() == 0) {
			throw makeconcept().link("is","pattern-failure", "pattern", self);
		}
		result = txt2ref(concat);
	});

	aHabit("parse-relative-index", ((index-text, txt)), {
		std::stringstream ss(ref2txt(txt));
		int64_t ires;
		ss >> ires;
		result = makeconcept();
		result.val(ires);
	});

	aHabit("stream-move-relative", ((stream, stm), (index-text, txt)), {
		ref offsetref = ref("parse-relative-index")(txt);
		int64_t offset = offsetref.val<int64_t>();
		conceptunmake(offsetref);
		if (offset != 0) {
			ref dir;
			if (offset < 0) {
				dir = stm.get("do-previous");
				offset = -offset;
			} else {
				dir = stm.get("do-next");
			}
			while (offset) {
				dir(stm);
				-- offset;
			}
		}
	});

	aHabit("stream-peek-relative", ((stream, stm), (index-text, txt)), {
		ref("stream-move-relative")(stm, txt);
		result = stm.get("do-value")(stm);
		ref("stream-move-relative")(stm, txt2ref("-" + ref2txt(txt)));
	});

	// stream can be generated with parsing function that takes a stream
	// and outputs a word
	
	// we have contextual lookup in these parsing functions
	// but not in the stream functions.
	// but it would make sense to alter streams to fit the context.
	
	// the parsing function takes a context (ignoring the aHabit context),
	// which is used for looking up parts of the parsing process  ... i think ...
	
	// please combine contexts of streams of contextualized parsing.
	// 	parsing operates in terms of a larger context that says what-to-do
	// 	when something encountered.
	// 	notably, we encounter new words, and we want to act on keywords
	// 	but there may be different keywrods in different contexts
	// 	would like to be flexible enough to provide a wordparser that
	// 	can tokenize other than wih spaces
	// 
	// due to diffiulty combining inoration, propose using general tokenizer.
	// token-stream.  word-stream.  let's take tokenizing out of contextual parsing.
	// 		this means always-same-tokens?
	//
	// 		all we need is for the tokenizer to know what context it's in
	// 		this provides suitable generality to not investigate other parts of
	// 		this process.
	// 		atm it takrs a stream, so use the context as the stream
	// 	so much thanks, above solution works so well.
	// 	we found with enough generality on a small scale, we can provide a gift
	// 	to larger scale that does not create burden.
	
	// two different streams.
	// raw stream, word stream.
	// 	investment says, make them both use 
	// 		yes this work will need to be done anyway.
	aHabit("parse-contextual-stream-word", ((context, ctx)), {
		ctx.get("parse-word"
	});

	// we hvae parsing contexts with abstract functions like parse-until-delimiter.
	// parse-until-delimiter will need to know how-to-handle word
	// parse-word

	// spaces separate words
	aHabit("parse-word", ((context, ctx), (stream, stm), (word, w)), {
			// why is word an argument here?
	});
	aHabit("parse-spaces-until-delimiter", ((context, ctx), (stream, stm), (delimiter, delim)), {
		
	});
}

void parsespaces(ref context, ref stream, ref parsertxt)
{
	context.set("parse-until-delimiter");///.....
	
	// we'll want to load parsers into contexts for the parsespaces context.
	// we have aHabit but it doesn't use contexts
		// aHabit loads into bootstrap context. we can reference from there.
}

ref spacesuntildelimiter(ref context, ref stream, ref delimiter)
{
	std::string word;
	while (true) {
		
	}
}

void parseopenbrace(ref context, stream, parsertxt)
{
	std::string brace1 = ref2txt(bracetxt);
	std::string brace2;
	if (brace1 == "[") { brace2 = "]"; }
	else if (brace1 == "{") { brace2 = "}"; }
	else if (brace1 == "(") { brace2 = ")"; }
	else if (brace1 == "<") { brace2 = ">"; }
	else if (brace1 == "[[") { brace2 = "]]"; }
	else if (brace1 == "{{") { brace2 = "}}"; }
	else if (brace1 == "((") { brace2 = "))"; }
	else if (brace1 == "<<") { brace2 = ">>"; }
	else if (brace1 == "begin") { brace2 = "end"; }
	ref contents = ctxlookup(context, txt2ref("parse-until-delimiter"))(stream, context, brace2);

	// i'd like to be able to evaluate things in either an expression context or a statement context.
	// this would mean having a contextual evaluator, I suppose
	// here, let's produce a simple list of words
}

void parsebootstrap(ref stream, ref context)
{
	istream & ss = *stream.val<istream*>();
	string lookupstr;
	ss >> lookupstr;
	ref lookup = lookupstr;
	std::list<std::string> comments;
	while (true) {
		string cmd;
		ss >> cmd;
		if (!ss) { break; }
		if (cmd == "//") {
			std::string comment;
			std::getline(ss, comment);
			comments.push_back(comment);
		} else if (cmd == "concept") {
			string name;
			ss >> name;
			ref c = lookup(parsevalue(name));
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-concept"); }
			while (true) {
				string type, target;
				ss >> type;
				if (type == "]") { break; }
				ss >> target;
				c.link(lookup(parsevalue(type)), lookup(parsevalue(target)));
			}
		} else if (cmd == "habit") {
			string name;
			ss >> name;
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-name"); }
			ref args = makeconcept();
			while (true) {
				string arg;
				ss >> arg;
				if (ss == "]") { break; }
				args.link("information-order", arg);
			}
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-needs"); }
			std::set<string> values;
			values.insert("context");
			values.insert("self");
			while (true) {
				string arg;
				ss >> arg;
				if (ss == "]") { break; }
				args.link("information-made", arg);
				// need to seed values with argument names
				values.insert(arg.name());
			} // information-made is unused.  is a 'stub' for if-needed-later.
			ref("set-steps")(name, args);
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-habit-makes"); }
			std::map<string,ref> labels;
			ref laststep = name;
			labels["return"] = nothing;
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			while (true) {
				string label, action, result;
				ss >> action;
				if (action == "]") { break; }
				if (action == "//") {
				}
				if (action[action.size()-1] == ':' || action[action.size()-1] == ',') {
					label = action;
					label.resize(label.size() - 1);
					if (label == "return") { throw makeconcept().link(is, "return-label-used"); }
					ss >> action;
				}
				if (action == "=" || action == "set") {
					ss >> result;
					ss >> action;
					values.insert(result);
				}
				// if (action == "concept") {
				// }
				if (action[action.size()-1] == '.') {
					// is goto
					action.resize(action.size() - 1);
					if (!labels.count(action)) {
						labels.emplace(action, makeconcept());
					}
					labels[action].link("label", action);
					if (laststep.linked("next-step")) { throw makeconcept().link(is, "jump-from-nowhere", "label", action); }
					laststep.link("next-step", labels[action]);
					laststep = nothing;
					continue;
				}
				if (action == "if") {
					ref cond = lookup(parsevalue(stream));
					ss >> action;
					if (action[action.size()-1] != '.') {
						throw makeconcept().link(is, "condition-is-not-label", "action", action, "cond", cond);
					}
					if (!laststep.isa("condition-step")) {
						throw makeconcept().link(is, "if-not-following-condition", "cond", cond, "action", action);
					}
					if (label.size()) {
						throw makeconcept().link(is, "if-case-has-label", "cond", cond, "action", action, "label", label);
					}
					action.resize(action.size()-1);
					if (!labels.count(action)) {
						labels.emplace(action, makeconcept());
						labels[action].link("label", action);
					}
					ref("condition-step-set")(laststep, cond, labels[action]);
					// if this improves from being  jump, remember to
					// update laststep to end of any 'anything' branch
					continue;
				}
				if (laststep == nothing && label.size() == 0) { throw makeconcept().link(is, "no-path-to-code"); }
				if (label.size() && !labels.count(label)) {
					labels[label] = makeconcept();
					labels[label].link("label", label);
				}
				ref nextstep = label.size() ? labels[label] : makeconcept();
				if (action == "?" || action == "pick") {
					string cond;
					ss >> cond;
					if (!values.count(cond)) {
						throw makeconcept().link(is, "condition-must-be-in-context", condition, cond);
					}
					laststep = ref("set-condition-step")(nextstep, laststep, cond, makeconcept().link("anything", "nothing"));
				} else {
					// otherwise, action is an action, and we have to read the right number of args
					if (laststep.isa("condition-step")) {
						if (ref("condition-step-get")(laststep, "anything") != "nothing") {
							if (label.size() == 0) {
								throw makeconcept().link(is, "condition-already-has-anything-branch-and-steps-follow", condition, laststep);
							}
						} else {
							ref("condition-step-set")(laststep, "anything", nextstep);
						}
					} else if (laststep != nothing) {
						laststep.link("next-step", nextstep);
					}
					ref habit = values.count(action) ? action : lookup(action);
					ref order = makehabitinformationorder(habit);
					ref neededmap = makeconcept();
					ref knownmap = makeconcept();
					ref informationnames = makeconcept();
					string linerest;
				       	std::getline(ss, linerest);
					stringstream ss2(linerest);
					ref stream2 = alloc(intellect::level0::concepts::allocations(), (istream*)&ss2);
					auto args = order.getAll("information-order");
					auto argsit = args.begin();
					while (true) {
						ref argname = parsevalue(stream2);
						if (!ss2) { break; }
						// depending on whether argname is in localcontext, pass to neededmap or knownmap.  also parse literal strings.
						informationnames.link("name", argname);
						if (argsit != args.end()) {
							if (values.count(argname.name())) {
								neededmap.link(*argsit, argname);
							} else {
								knownmap.link(*argsit, lookup(argname));
							}
							++ argsit;
						}
					}
					knownmap.link("information-names", informationnames);
					conceptunmake(order);
					dealloc(stream2, intellect::level0::concepts::allocations());
					ref mademap = makeconcept();
					if (result.size()) {
						mademap.link("result", values.count(result) ? result : lookup(result));
					}
					ref("set-context-step")(nextstep, "nothing", knownmap, neededmap, mademap, habit);
					laststep = nextstep;
				}
			} 
		} else {
			throw makeconcept().link("is", "parse-error", "stream", stream, "unexpected-word", cmd);
		}
	}
}

}
}
