#include "bootstrap-parser.hpp"

#include "../level-1/sugar.hpp"
#include "../level-2/habits.hpp"
#include "../level-2/sugar.hpp"

#include <deque>
#include <fstream>
#include <list>
#include <set>

namespace intellect {
namespace level3 {

// 2020-01-28
// i made a bootstrap-word habit that handles quotes and considers line-endings
// a word.  it has disappeared, i must have accidentally erased it.  remaking.

// I'm noticing a lot of confusion around the use of text strings and names.
// It could be worthwhile to make use of name contexts, using text, globally, in
// level 2 or 3.  That could mean a ton of fixups.  We could also drop text and just
// use bootstrap names entirely.  We could also switch from names to text without
// contexts: we'd ned a way to get a unique ref for text.

// must support syntax-sugar <- outdated, but doesn't look hard to provide for

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
	std::iostream & ss = *stream.val<std::iostream*>();
	std::string word;
	ss >> word;
	if (word.size() > 0 && (word[0] == '"' || word[0] == '\'' || word[0] == '`')) {
		char delim = word[0];
		std::string accum = word;
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

static ref doprevious("do-previous"), donext("do-next"), dovalue("do-value"), dogo("do-go"), dowhere("do-where"), word("word"), informationorder("information-order"), nothing("nothing"), bracedwords("braced-words"), word_("word"), name_("name"), text_("text"), outer("outer"), bootstraplistwordcontext("bootstrap-list-word-context"), wordcontext("word-context"), openbrace("open-brace"), closebrace("close-brace"), informationneeded("information-needed"), is("is"), filecontext("file-context"), parsecontext("parse-context"), file("file"), value("value"), focus("focus"), space("space"), result_("result");

// let's do contextual word lookup.  it will really ease stuff later.


// we use text for two things:
// 1. symbols and text
// 2. bootstrap-context references and concepts
// instead of nothing or everything >_>
std::string ref2txt(ref r)
{
	//return r.vget<std::string>(name_);
	return r.val<std::string>();
}
bool istxtref(ref r)
{
	return r.isa(text_);
}
ref txt2ref(std::string t)
{
	return ref(t).get(name_);
}
ref txtref2bootstrap(ref txt)
{
	return ref(ref2txt(txt));
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
	encountered.insert(context);
	nesteds.push_back(context);

	while (!nesteds.empty()) {
		context = nesteds.front(); nesteds.pop_front();
		for (auto link : context.links()) {
			if (link.first == item) {
				return link.second;
			} else if (link.first == "is" || link.second == "outer-context") {
				if (encountered.count(link.second)) { continue; }
				nesteds.push_back(link.second);
				encountered.insert(link.second);
			} else if (link.first.isan("exclusive-context-group")) {
				if (!exclusive_ctx_groups.count(link.first)) {
					exclusive_ctx_groups[link.first] = link.second;
					nesteds.push_back(link.second);
					encountered.insert(link.second);
				}
			}
		}
	}
	throw intellect::level2::noteconcept().link("is", "item-not-in-context");
}

// what-distinguishes-words
ref parsewordtxt_ws(ref context, ref stream)
{
	std::string word;
	std::iostream & ss = *stream.val<std::iostream*>();
	ss >> word;
	if (!ss) throw intellect::level2::noteconcept().link("is", "end-of-stream", "stream", stream);
	return txt2ref(word);
}

// this function can handle most stuff
void parseprefix(ref context, ref stream, ref prefix)
{
	try {
		while (true) {
			ref parsertxt = parsewordtxt_ws(context, stream);
			if (!*stream.val<std::iostream*>()) break;
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
	template <typename... Rs>
	ConceptException(std::string type, Rs... rs)
	: r(intellect::level2::noteconcept().link("is", type, rs...))
	{ }

	ConceptException(ConceptException &) = delete;
	ConceptException(ConceptException && other)
	{
		r = other.r;
		other.r.ptr() = 0;
	}

	operator ref() { return r; }

	~ConceptException()
	{
		if (r.ptr()) { intellect::level2::conceptunmake(r); }
	}

	ref r;
};

class ConceptUnmaker
{
public:
	ConceptUnmaker(ref C)
	: C(C) { }
	~ConceptUnmaker()
	{
		conceptunmake(C);
	}
private:
	ref C;
};
class StreamSentinel
{
public:
	StreamSentinel(ref S)
	: S(S)
	{
		returntohere();
	}

	ref value() { return S[dovalue](S); }
	void next() { S[donext](S); }
	void previous() { S[doprevious](S); }
	void go(ref w) { S[dogo](S, w); }
	ref where() { return S[dowhere](S); }
	void returntohere() { origin = where(); }

	~StreamSentinel()
	{
		go(origin);
	}

private:
	ref origin;
	ref S;
};

ref bootstraplookup(ref text, bool create = false)
{
	std::string str = ref2txt(text);
	if (str[0] == '\'' || str[0] == '\"' || str[0] == '`') {
		if (str[str.size()-1] == str[0]) {
			std::string temp = str.c_str()+1;
			temp.resize(temp.size()-1);
			if (str[0] == '\"') {
				return txt2ref(temp);
			}
			str = temp;
		}
	}
	return intellect::level2::getnamed(str, create);
}

ref bootstrapmatchingbrace(std::string brace1)
{
	static ref nothing("nothing");
	//std::string brace1 = ref2txt(bracetxt);
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
	else { return nothing; }
	return txt2ref(brace2);
}

ref bootstrap_word(ref self, ref stm, ref spc)
{
	StreamSentinel s(stm);
	std::string concat;
	std::string quote;
	std::string letter;
	try {
		while (true) {
			letter = ref2txt(s.value());
			if (letter == " " || letter == "\r" || letter == "\t") {
				std::cerr << "(" << letter << ")";
				s.next();
				continue;
			}
			break;
		}
		if (letter == "'" || letter == "\"" || letter == "`") {
			std::cout << letter;
			quote = concat = letter; s.next();
		}
		while (true) {
			letter = ref2txt(s.value());
			if (!quote.size()) {
				if (letter == " " || letter == "\r" || letter == "\t") {
					break;
				}
				if (concat.size() != 0 && (letter == "\n" || letter == "[" || letter == "{" || letter == "(" || letter == "<" || letter == "]" || letter == "}" || letter == ")" || letter == ">") ) {
					// second token should end processing, but keep for next processing
					s.previous();
					break;
				}
			}
			std::cout << letter;
			concat += letter;
			if (quote.size()) {
				if (letter == quote) {
					break;
				}
			} else if (concat == "\n" || concat == "[" || concat == "{" || concat == "(" || concat == "<" || concat == "]" || concat == "}" || concat == ")" || concat == ">") {
				break;
			}
			s.next();
		}
		std::cerr << "(" << letter << ")";
	} catch(ref r) {
		if (r.isa("end-of-stream") && !quote.size()) {
			conceptunmake(r);
			// just another way to terminate the loop
		} else {
			throw r;
		}
	}
	if (concat.size() == 0) {
		throw intellect::level2::noteconcept().link("is","pattern-failure", "pattern", self);
	}
	ref result = txt2ref(concat);
	s.returntohere();
	return result;
}

ref bootstrap_parse_brace(ref bracetxt, ref file, ref ws, ref self)
{
	ref brace2ref = bootstrapmatchingbrace(ref2txt(bracetxt));
	if (brace2ref == "nothing") { throw intellect::level2::noteconcept().link("is", "unexpected-word", "word-space", ws, "habit", self); }
	std::string brace2 = ref2txt(brace2ref);
	ref result = intellect::level2::noteconcept();
	result.link(is, bracedwords);
	result.link(openbrace, ws.get(dowhere)(ws));//bracetxt);
	auto & vec = intellect::level2::data<intellect::level2::vector>(result);
	while (true) {
		ws.get(donext)(ws);
		ref sym = ws.get(dovalue)(ws);
		if (istxtref(sym) && ref2txt(sym) == brace2) {
			result.link(closebrace, ws.get(dowhere)(ws));//sym);
			break;
		}
		vec.push_back(sym);
		//if (bootstrapmatchingbrace(sym) != nothing) {
		//	self(file, ws);
		//}
	}
	return result;
}

ref bootstrap_parser(ref stm, ref spc)
{
	ref fctx = stm.get(filecontext);
	ref wctx = fctx.get(wordcontext);
	ref pctx = fctx.get(parsecontext);
	ref f = fctx.get(file);
	ref result;
	// i'd really like to donext all but the first time.
	// 	... we don't wnt to donext if we haven't processed the first value yet.
	// 	
	while (true) {
		ref word = stm.get(dovalue)(stm);
		if (!wctx.linked(word)) {
			return word;
		}
		spc.set(value, word);
		result = wctx.get(word)({{focus, word}, {space, spc}, {wordcontext, wctx}, {parsecontext, pctx}, {filecontext, fctx}, {result_, f}}, true);
		if (result != nothing) {
			return result;
		}
		stm.get(donext)(stm);
	}
	return result;
}

#undef self
// uhh for comments, thinking about processing them inside the word-parser.
// so, that means the stream offers tokens that _aren't_ processed by a wctx.
// meanwhile, it calls wctx for every one that is.
// 	does that work?  uhhhh
// 	so, we start off, say run into 'habit'.  the main parser is a word parser,
// 	offering a stream of unparsed words, mysteriously skipping ones that are
// 		i think that might work.
// 			it may have an unaddressed problem that may crop up later.
// is good for comments.  we have comments, concept, and habit atm.
// 	so we are planning to make a stream where the parser checks wctx.
// 	and replace the main parser with it.
// 	okay!
// 		okay, i found an unaddressed issue, as was predicted, luckily soon
// 		the [ parser is used to handle ['s.
// 		results of parsing aren't put anywhere, so with it not being called manually,
// 		braced content will disappear from parsing now.
// 		we should output the results of our parsing into the stream made by the parser.  comments don't output anything, so they're skipped.
// 			incidentally, this implements expressions!  and is small and easy!
ref bootstrap_parse_habit(ref tokennameref, ref file, ref ws, ref ctx, ref self, ref wctx, ref fctx)
{
	//StreamSentinel s(ws);
	auto tokenname = ref2txt(tokennameref);
	if (tokenname != "habit" && tokenname != "parser") { throw intellect::level2::noteconcept().link("is", "unexpected-word", "word-space", ws, "habit", self); }
	ref habitname = (ws.get(donext)(ws), ws.get(dovalue)(ws));
	ref habit = wctx.get(txt2ref("lookup"))(habitname, true);
	bootstraplistwordcontext.set(outer, fctx.get(wordcontext));
	fctx.set(wordcontext, bootstraplistwordcontext);
	ref argtext = (ws.get(donext)(ws), ws.get(dovalue)(ws));
	ref results = (ws.get(donext)(ws), ws.get(dovalue)(ws)); (void)results;
	ref steps = (ws.get(donext)(ws), ws.get(dovalue)(ws));
	fctx.set(wordcontext, bootstraplistwordcontext.get(outer));
	ref args = intellect::level2::noteconcept();
	//ConceptUnmaker argsdel(args), resultsdel(results),stepsdel(steps);
	auto & targets = argtext.val<intellect::level2::vector>();
	auto & argvec = intellect::level2::data<intellect::level2::vector>(args);
	for (auto it = targets.begin(); it != targets.end();) {
		auto target = *(it++);
		if (ref2txt(target) == "\n") { continue; }
		// txtref2bootstrap is used because there are still name values
		// like "self" and "context" coming from level2
		argvec.push_back(txtref2bootstrap(target));
	}
	ref("set-steps")(habit, args);
	if (tokenname == "parser") { wctx.set(habitname, habit); }
	// now we copy from level2.cpp, noting that \n is a symbol now
	std::map<std::string, ref> labels;
	std::set<std::string> values;
	values.insert("context");
	values.insert("self");
	values.insert("result");
	for (auto target : args.val<intellect::level2::vector>()) {
		values.insert(ref(target).name());
	}
	ref laststep = habit;
	labels["return"] = nothing;
	ws.get(dogo)(ws, steps.get(openbrace));
	while (true) {
		ws.get(donext)(ws);
		if (ws.get(dowhere)(ws) == steps.get(closebrace)) { break; }
		ref wordref = ws.get(dovalue)(ws);
		std::string label, result;
		std::string word = ref2txt(wordref);
		if (word == "\n") { continue; }
		if (word[word.size()-1] == ':' || word[word.size()-1] == ',') {
			label = word;
			label.resize(label.size() - 1);
			if (label == "return") { throw intellect::level2::noteconcept().link("is", "return-label-used"); }
			do {
				wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
				word = ref2txt(wordref);
			} while (word == "\n");
		}
		if (word == "=" || word == "set") {
			fctx.set(wordcontext, bootstraplistwordcontext);
			do {
				wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
				result = ref2txt(wordref);
			} while (result == "\n");
			values.insert(result);
			fctx.set(wordcontext, bootstraplistwordcontext.get(outer));

			do {
				wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
				word = ref2txt(wordref);
			} while (word == "\n");
		}
		if (word[word.size()-1] == '.') {
			// is goto
			word.resize(word.size() - 1);
			if (!labels.count(word)) {
				labels.emplace(word, intellect::level2::noteconcept());
			}
			labels[word].link("label", word);
			if (laststep.isa("condition-step")) {
				if (ref("condition-step-get")(laststep, "anything") != "nothing") {
					throw intellect::level2::noteconcept().link("is", "condition-already-has-anything-branch,-and-jump-follows", "condition", laststep, "label", labels[word]);
				}
				ref("condition-step-set")(laststep, "anything", labels[word]);
			} else {
				if (laststep.linked("next-step")) {
					throw intellect::level2::noteconcept().link("is", "jump-from-nowhere", "label", word);
				}
				laststep.link("next-step", labels[word]);
			}
			laststep = ref("nothing");
			continue;
		}
		if (word == "if") {
			fctx.set(wordcontext, bootstraplistwordcontext);
			wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
			word = ref2txt(wordref);
			fctx.set(wordcontext, bootstraplistwordcontext.get(outer));

			ref cond = wctx.get(txt2ref("lookup"))(wordref);
			wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
			word = ref2txt(wordref);
			if (word[word.size()-1] != '.') {
				throw intellect::level2::noteconcept().link("is", "condition-is-not-label", "action", word, "cond", cond);
			}
			if (!laststep.isa("condition-step")) {
				throw intellect::level2::noteconcept().link("is", "if-not-following-condition", "cond", cond, "action", word);
			}
			if (label.size()) {
				throw intellect::level2::noteconcept().link("is", "if-case-has-label", "cond", cond, "action", word, "label", label);
			}
			word.resize(word.size()-1);
			if (!labels.count(word)) {
				labels.emplace(word, intellect::level2::noteconcept());
				labels[word].link("label", word);
			}
			ref("condition-step-set")(laststep, cond, labels[word]);
			// if this improves from being  jump, remember to
			// update laststep to end of any 'anything' branch
			continue;
		}
		if (laststep == "nothing" && label.size() == 0) { throw intellect::level2::noteconcept().link("is", "no-path-to-code"); }
		if (label.size() && !labels.count(label)) {
			labels[label] = intellect::level2::noteconcept();
			labels[label].link("label", label);
		}
		ref nextstep = label.size() ? labels[label] : intellect::level2::noteconcept();
		if (word == "?" || word == "pick") {
			fctx.set(wordcontext, bootstraplistwordcontext);
			wordref = (ws.get(donext)(ws), ws.get(dovalue)(ws));
			word = ref2txt(wordref);
			fctx.set(wordcontext, bootstraplistwordcontext.get(outer));

			if (!values.count(word)) {
				throw intellect::level2::noteconcept().link("is", "condition-must-be-in-context", "condition", word);
			}
			laststep = ref("set-condition-step")(nextstep, laststep, word, intellect::level2::noteconcept().link("anything", "nothing"));
		} else {
			// otherwise, is an action, and we have to read the args
			if (laststep.isa("condition-step")) {
				if (ref("condition-step-get")(laststep, "anything") != "nothing") {
					if (label.size() == 0) {
						throw intellect::level2::noteconcept().link("is", "condition-already-has-anything-branch,-and-steps-follow", "condition", laststep);
					}
				} else {
					ref("condition-step-set")(laststep, "anything", nextstep);
				}
			} else if (laststep != "nothing") {
				laststep.link("next-step", nextstep);
			}
			ref order;
			intellect::level2::vector orderitems;
			decltype(orderitems.begin()) orderit;
			ref neededmap = intellect::level2::noteconcept();
			ref knownmap = intellect::level2::noteconcept();
			ref subhabit = wordref;
			if (values.count(ref2txt(subhabit))) {
				neededmap.link(intellect::level2::concepts::self_, txtref2bootstrap(subhabit));
			} else {
				subhabit = wctx.get(txt2ref("lookup"))(subhabit);
				if (!subhabit.linked(informationneeded)) {
					conceptunmake(neededmap);
					conceptunmake(knownmap);
					throw intellect::level2::noteconcept().link(
							"is", "habit-not-found",
						        "subhabit", subhabit,
							"habit", habit);
				}
				knownmap.link(intellect::level2::concepts::self_, subhabit);
		       		order = makehabitinformationorder(subhabit);
				orderitems = order.val<decltype(orderitems)>();
				orderit = orderitems.begin();
			}
			while (true) {
				if (order != nothing && orderit == orderitems.end()) {
					conceptunmake(order);
					order = nothing;
				}
				fctx.set(wordcontext, bootstraplistwordcontext);
				ref arg = (ws.get(donext)(ws), ws.get(dovalue)(ws));
				fctx.set(wordcontext, bootstraplistwordcontext.get(outer));
				if (arg.isa(bracedwords)) {
					auto words = arg.val<intellect::level2::vector>();
					auto it = words.begin();
					ref label = txtref2bootstrap(*it);
					if (order != nothing) {
						if (*orderit != label) {
							throw intellect::level2::noteconcept().link("is", "out-of-order-information", "information", label, "expected", *orderit, "habit", habit, "subhabit", subhabit);
						}
						++ orderit;
					}
					++it;
					ref val = *it;
					if (values.count(ref2txt(val))) {
						neededmap.link(label, txtref2bootstrap(val));
					} else {
						knownmap.link(label, wctx.get(txt2ref("lookup"))(val));
					}
					++it;
					if (it != words.end()) { throw intellect::level2::noteconcept().link("is", "extra-word-in-label-information-pair", "space", ws); } 
					continue;
				}
				std::string argname = ref2txt(arg);
				if (argname == "\n") { break; }
				if (order == nothing) { throw intellect::level2::noteconcept().link("is","ordered-information-for-contextual-subhabit", "subhabit", subhabit, "habit", habit, "information", arg); }
				if (values.count(argname)) {
					neededmap.link(*orderit, txtref2bootstrap(arg));
				} else {
					knownmap.link(*orderit, wctx.get(txt2ref("lookup"))(arg));
				}
				++ orderit;
			}
			// TODO: could check to make sure all ordered information is provided.
			if (order != nothing) { conceptunmake(order); }
			ref mademap = intellect::level2::noteconcept();
			if (result.size()) {
				mademap.link("result", txtref2bootstrap(txt2ref(result)));
			}
			ref("set-context-step")(nextstep, "nothing", knownmap, neededmap, mademap);
			laststep = nextstep;
		}
	}
	return habit;
}
/*
ref bootstrap_parse_call(ref file, ref ws, ref ctx, ref self, ref wctx)
{
	// ran into issue: word-array is not a wordspace.
	// propose making the brace parser be a stream constructor.
	// returns stream at a location in a keep-stream, that runs
	// into end at end of braces.
	// 	what do we use for subbraces.
	// 		uhh it looks like implementing will make this obvious
	// 		streams? looks like streams.
}
*/

ref bootstrap_parse_concept(ref f, ref file, ref ws, ref ctx, ref self, ref wctx, ref fctx)
{
	std::string focus = ref2txt(f);
	if (focus != "concept") { throw intellect::level2::noteconcept().link("is", "unexpected-word", "word-space", ws, "habit", self); }
	bootstraplistwordcontext.set(outer, fctx.get(wordcontext));
	fctx.set(wordcontext, bootstraplistwordcontext);
	ref conceptname = (ws.get(donext)(ws), ws.get(dovalue)(ws));
	ref concept = wctx.get(txt2ref("lookup"))(conceptname, true);
	checknotepad(concept);
	ref parts = (ws.get(donext)(ws), ws.get(dovalue)(ws));
	fctx.set(wordcontext, bootstraplistwordcontext.get(outer));
	//ConceptUnmaker partsdel(parts);
	auto allparts = parts.val<intellect::level2::vector>();
	for (auto it = allparts.begin(); it != allparts.end();) {
		if (ref2txt(*it) == "\n") { ++ it; continue; }
		ref type = (*it);
		++ it;
		while (ref2txt(*it) == "\n") { ++ it; }
		ref target = (*it);
		concept.link(wctx.get(txt2ref("lookup"))(type), wctx.get(txt2ref("lookup"))(target));
		++ it;
	}
	return concept;
}


void loadhabits()
{
	intellect::level2::createhabits();

	ref("condition").link("quiet", true);

	decls(entry, previous, next, source, value, word, parser, nothing, file, focus, space, is);
	ref doprevious("do-previous"), donext("do-next"), streammoverelative("stream-move-relative"), openbrace("open-brace"), closebrace("close-brace"), dogo("do-go"), dowhere("do-where"), informationorder("information-order"), filecontext("file-context"), wordcontext("word-context"), parsecontext("parse-context"), result_("result"), bracedwords("braced-words"), word_("word"), true_("true"), false_("false");

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
	
	ahabit(make-c++-stream-from-filename, ((filename, fn)), {
		std::iostream * stm = new std::fstream(ref2txt(fn));
		result = ref("make-c++-stream")(intellect::level2::noteconcept(stm));
	});

	ahabit(make-c++-stream, ((source, stm)), {
		result = intellect::level2::noteconcept();
		result.link("source", stm);
		result.link(dovalue, "stream-value");
		result.link("do-next-letter", "c++-stream-next-letter");
		result.link("do-next-word", "c++-stream-next-word");
		result.link("do-next-line", "c++-stream-next-line");
	});

	intellect::level1::givename(ref("make-c++-stream")(intellect::level2::noteconcept((std::iostream*)&std::cin)), "c++-stdin");

	ahabit(c++-stream-unmake, ((stream, stm)), {
		ref src = stm.get("source");
		std::iostream * ss = src.val<std::iostream*>();
		intellect::level2::conceptunmake(stm);
		intellect::level2::conceptunmake(src);
		delete ss;
	});

	/*
	ahabit(make-c++-word-stream, ((source, stm)), {
		result = ref("make-c++-stream")(stm);
		result.link(donext, result.get("do-next-word"));
	});

	ahabit(make-c++-letter-stream, ((source, stm)), {
		result = ref("make-c++-stream")(stm);
		result.link("do-next", result.get("do-next-letter"));
	});
	*/

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



	ahabit(c++-stream-next-letter, ((space, stm)), {
		std::iostream & ss = *stm.get(source).val<std::iostream*>();
		char c[2] = { (char)ss.get(), 0 };
		if (!ss) { throw intellect::level2::noteconcept().link("is", "end-of-stream", "stream", stm); }
		stm.set(value, txt2ref(c));
	});
	ref("c++-stream-next-letter").link("quiet", true);

	ahabit(c++-stream-next-word, ((space, stm)), {
		std::iostream & ss = *stm.get(source).val<std::iostream*>();
		std::string s;
		ss >> s;
		if (!ss) { throw intellect::level2::noteconcept().link("is", "end-of-stream", "stream", stm); }
		stm.set(value, txt2ref(s));
	});

	ahabit(make-parser-stream, ((source, stm), (parser, p)), {
		ref ret = intellect::level2::noteconcept();
		ret.link("is", "parser-stream");
		ret.link("is", "stream");
		ret.link("is", p-ref("parser-stream"));
		ret.link("source", stm);
		ret.link("parser", p);
		ret.link(donext, "parser-stream-next");
		ret.link(dogo, "parser-stream-go");
		ret.link(dowhere, "parser-stream-where");
		ret.link(dovalue, "stream-value");
		ret.set(value, p(stm, ret));
		result = ret;
	});
	
	ahabit(parser-stream-go, ((space, stm), (where, w)), {
		ref src = stm.get(source);
		src.get(dogo)(src, w);
		return stm;
	});
	ahabit(parser-stream-where, ((space, stm)), {
		ref src = stm.get(source);
		return src.get(dowhere)(src);
	});
	ahabit(parser-stream-next, ((space, stm)), {
		try {
			ref src = stm.get(source);
			src.get(donext)(src);
			ref item = stm.get(parser)(src, stm);
			stm.set(value, item);
		} catch(intellect::level2::ref r) {
			if (r.linked("stream") || r.isa("end-of-stream")) {
				r.set("stream", stm);
			}
			throw r;
		}
	});
	ref("parser-stream-next").link("quiet", true);
	
	ahabit(stream-value, ((space, stm)), {
		result = stm.get(value);
	});
	ref("stream-value").link("quiet", true);

	ahabit(make-keep-stream, ((source, stm)), {
		ref ret = intellect::level2::noteconcept();
		ret.link("is", "keep-stream");
		ret.link("is", "stream");
		ret.link("source", stm);
		ref ntry = intellect::level2::noteconcept();
		ntry.link(value, stm.get(dovalue)(stm));
		ret.link(entry, ntry);
		ret.link("first", ntry);
		ret.link(dovalue, "keep-stream-value");
		ret.link(donext, "keep-stream-next");
		ret.link(doprevious, "keep-stream-previous");
		ret.link(dogo, "keep-stream-go");
		ret.link(dowhere, "keep-stream-where");
		return ret;
	});
	ahabit(keep-stream-unmake, ((keep-stream, stm)), {
		conceptunmake(stm.get(entry));
		conceptunmake(stm);
	});
	ahabit(keep-stream-value, ((space, stm)), {
		return stm.get(entry).get(value);
	});
	ref("keep-stream-value").link("quiet", true);
	ahabit(keep-stream-where, ((space, stm)), {
		return stm.get(entry);
	});
	ref("keep-stream-where").link("quiet", true);
	ahabit(keep-stream-go, ((space, stm), (where, w)), {
		stm.set(entry, w);
		return stm;
	});
	ref("keep-stream-go").link("quiet", true);
	ahabit(keep-stream-next, ((space, stm)), {
		ref ntry = stm.get(entry);
		if (ntry.linked(next)) {
			stm.set(entry, ntry.get(next));
		} else {
			ref nxt = intellect::level2::noteconcept();
			nxt.link(previous, ntry);
			ntry.link(next, nxt);
			stm.set(entry, nxt);
			ref src = stm.get(source);
			src.get(donext)(src);
			nxt.link(value, src.get(dovalue)(src));
		}
	});
	ref("keep-stream-next").link("quiet", true);
	ahabit(keep-stream-previous, ((space, stm)), {
		ref cur = stm.get(entry);
		if (!cur.linked(previous)) {
			throw intellect::level2::noteconcept().link("is", "start-of-stream", "stream", stm);
		}
		stm.set(entry, cur.get(previous));
	});
	ref("keep-stream-previous").link("quiet", true);

	// keep-stream of letters.  make keep-stream of words.
	// we have parser stream that can be wrapped in keep stream, but is confusing
	// is less confusing to make parser stream also be a keep stream.
	// 	we could use patterns here.
	
	// this actually functions to return the stream to where it was at start.
	// probably the intention was to make a mutated stream somehow.
	// for now, I've simply disabled that, using returntohere().
	ahabit(whitespace-word, ((source, stm), (space, mystrm)), {
		StreamSentinel s(stm);
		std::string concat;
		std::string letter;
		try {
			while (true) {
				letter = ref2txt(s.value());
				if (letter == " " || letter == "\r" || letter == "\n" || letter == "\t") {
					s.next();
					continue;
				}
				break;
			}
			while (true) {
				letter = ref2txt(s.value());
				if (letter == " " || letter == "\r" || letter == "\n" || letter == "\t") {
					break;
				}
				concat += letter;
				s.next();
			}
		} catch(ref r) {
			if (r.isa("end-of-stream")) {
				conceptunmake(r);
				// just another way to terminate the loop
			} else {
				throw r;
			}
		}
		if (concat.size() == 0) {
			throw intellect::level2::noteconcept().link("is","pattern-failure", "pattern", self);
		}
		result = txt2ref(concat);
		s.returntohere();
	});
	//ahabit(bootstrap-matching-brace, ((brace-text, bracetxt)), {
	//auto bootstrapmatchingbrace = [](std::string brace1)
	//{
	//};//);
	//ref bootstrapmatchingbrace("bootstrap-matching-brace");
	ahabit(bootstrap-word, ((source, stm), (space, spc)), {
		return bootstrap_word(self, stm, spc);
	});
	ref("bootstrap-word").link("quiet", true);

	// we are passing the source stream to the habit parser as the wordpace.
	// fix.
	ahabit(bootstrap-parser, ((source, stm), (space, spc)), {
		return bootstrap_parser(stm, spc);
	});
	ref("bootstrap-parser").link("quiet", true);

	ahabit(parse-relative-index, ((index-text, txt)), {
		std::stringstream ss(ref2txt(txt));
		int64_t ires;
		ss >> ires;
		result = intellect::level2::noteconcept();
		result.val(ires);
	});

	ahabit(stream-move-relative, ((space, stm), (where, txt)), {
		ref offsetref = ref("parse-relative-index")(txt);
		int64_t offset = offsetref.val<int64_t>();
		conceptunmake(offsetref);
		if (offset != 0) {
			ref dir;
			if (offset < 0) {
				dir = stm.get(doprevious);
				offset = -offset;
			} else {
				dir = stm.get(donext);
			}
			while (offset) {
				dir(stm);
				-- offset;
			}
		}
	});

	ahabit(stream-peek-relative, ((space, stm), (where, txt)), {
		streammoverelative(stm, txt);
		result = stm.get(dovalue)(stm);
		streammoverelative(stm, txt2ref("-" + ref2txt(txt)));
	});

	// stream can be generated with parsing function that takes a stream
	// and outputs a word
	
	// we have contextual lookup in these parsing functions
	// but not in the stream functions.
	// but it would make sense to alter streams to fit the context.
	
	// the parsing function takes a context (ignoring the ahabit context),
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
	
	/*
	ahabit(parse-contextual-stream-word, ((stream, stm), (word-context, wctx)), {
		wctx.get("parse-word")(stm);
	});
	*/

	ahabit(bootstrap-make-file-stream, ((filename, fn), (parse-context, pctx)), {
		ref cxxstm = ref("make-c++-stream-from-filename")(fn);
		cxxstm.link(donext, cxxstm.get("do-next-letter"));
		pctx.set("c++-stream", cxxstm);
		ref letterspace = ref("make-keep-stream")(cxxstm);
		pctx.set("letterspace", letterspace);
		ref parserstm = ref("make-parser-stream")(letterspace, "bootstrap-word");
		pctx.set("parser-stream", parserstm);
		ref wordspace = ref("make-keep-stream")(parserstm);
		pctx.set("wordspace", wordspace);
		
	});
	ahabit(bootstrap-file-stream-unmake, ((parse-context, pctx)), {
		ref("keep-stream-unmake")(pctx.get("wordspace"));
		conceptunmake(pctx.get("parser-stream"));
		ref("keep-stream-unmake")(pctx.get("letterspace"));
		ref("c++-stream-unmake")(pctx.get("c++-stream"));
		pctx.unlink("wordspace");
		pctx.unlink("parser-stream");
		pctx.unlink("letterspace");
		pctx.unlink("c++-stream");
	});
	ahabit(bootstrap-file-stream-next-value, ((parse-context, pctx)), {
		ref wordspace = pctx.get("wordspace");
		wordspace.get(donext)(wordspace);
		return wordspace.get(dovalue)(wordspace);
	});

	ref("bootstrap-file-context").link("word-context", "bootstrap-word-context");

	ahabit(bootstrap-parse-brace, ((focus, bracetxt), (result, file), (space, ws)),
	{
		return bootstrap_parse_brace(bracetxt, file, ws, self);
	});
	ref("bootstrap-list-word-context").link(txt2ref("["), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("{"), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("("), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("<"), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("[["), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("{{"), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("(("), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("<<"), "bootstrap-parse-brace");
	ref("bootstrap-list-word-context").link(txt2ref("begin"), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("["), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("{"), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("("), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("<"), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("[["), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("{{"), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("(("), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("<<"), "bootstrap-parse-brace");
	ref("bootstrap-word-context").link(txt2ref("begin"), "bootstrap-parse-brace");

	ahabit(bootstrap-lookup, ((text, txt), (create, c, false)),
	{
		return bootstraplookup(txt, c == true_);
	});
	ref("bootstrap-word-context").link(txt2ref("lookup"), "bootstrap-lookup");

	ahabit(bootstrap-parse-habit, ((focus, word), (result, file), (space, ws), (word-context, wctx), (file-context, fctx)), {
		return bootstrap_parse_habit(word, file, ws, ctx, self, wctx, fctx);
	});
	ref("bootstrap-word-context").link(txt2ref("habit"), "bootstrap-parse-habit");
	ref("bootstrap-word-context").link(txt2ref("parser"), "bootstrap-parse-habit");

	ahabit(bootstrap-parse-concept, ((focus, f), (result, file), (space, ws), (word-context, wctx), (file-context, fctx)), {
		return bootstrap_parse_concept(f, file, ws, ctx, self, wctx, fctx);
	});
	ref("bootstrap-word-context").link(txt2ref("concept"), "bootstrap-parse-concept");
	//ref("bootstrap-word-context").link(txt2ref("link"), "bootstrap-parse-concept");

	ahabit(parse-file, ((notepad, fn), (file-context, fctx, bootstrap-file-context)), {
		// notepad quick-implemented by passin the filename as the notepad name!  recommend keeping for safety, and copying data to outer notepad intentionally.
		ref wctx, pctx;
		ref file = intellect::level2::noteconcept();
		file.link("notepad", intellect::level2::notepad());
		fctx.set("file", file);
		if (!fctx.linked("word-context")) {
			wctx = intellect::level2::noteconcept();
			fctx.link("word-context", wctx);
		} else {
			wctx = fctx.get("word-context");
		}
		if (!fctx.linked("parse-context")) {
			pctx = intellect::level2::noteconcept();
			fctx.link("parse-context", pctx);
		} else {
			pctx = fctx.get("parse-context");
		}

		// TODO: implement just-do-one-step in level-2, to move towards using
		// relevence here
		
		ref cxxstm = ref("make-c++-stream-from-filename")(fn);
		cxxstm.link(donext, cxxstm.get("do-next-letter"));
		cxxstm.get(donext)(cxxstm);
		ref letterspace = ref("make-keep-stream")(cxxstm);
		ref parserstm = ref("make-parser-stream")(letterspace, "bootstrap-word");
		ref wordspace = ref("make-keep-stream")(parserstm);
			// note: rewinding wordspace won't rewind letterspace at this time
			// too bad!
		wordspace.set(filecontext, fctx);
		ref parsedspace = ref("make-parser-stream")(wordspace, "bootstrap-parser");

		// to fully generalize we would just move the parsing behavior
		// into the parse context.  this would probably be 'setup',
		// 'teardown', and 'next-value'.  we would love a universe where
		// we implement that as recreation.
		// NOTE: believe this was implemented but haven't put time into using it
		while (true) {
			ref word;
			word = parsedspace.get(dovalue)(parsedspace);
			parsedspace.get(donext)(parsedspace);
			// we are changing parser to call habits in the file parser itself. [previously we called a 'habit' parser.  this will still be used.  but behavior from it will be copied into this function.  it will make sense to abstract it into a third function.  we can link it as 'anything' from the word context. <- never did this.  instead 'parser' is a special 'habit'.
			
			// TODO: store as free text?
			// TODO: store references to underlying data.
			// 	this means including locations in the
			// 	spaces.
		} 

		// we have file context, word context, and parse context.

		
		
		conceptunmake(parserstm);
		ref("keep-stream-unmake")(letterspace);
		ref("c++-stream-unmake")(cxxstm);

		result = file;
	});
	ref("bootstrap-parse-file").set("quiet", "true");
/*
	ahabit(parse-contextual-stream-word, ((stream, stm), (word-context, wctx)), {
		wctx.get("parse-word")(stm);
	});

	// we hvae parsing contexts with abstract functions like parse-until-delimiter.
	// parse-until-delimiter will need to know how-to-handle word
	// parse-word

	// spaces separate words
	ahabit(parse-word, ((context, ctx), (stream, stm), (word, w)), {
			// why is word an argument here?
	});
	ahabit(parse-spaces-until-delimiter, ((context, ctx), (stream, stm), (delimiter, delim)), {
		
	});
	*/
}

void parsespaces(ref context, ref stream, ref parsertxt)
{
	//context.set("parse-until-delimiter");///.....
	
	// we'll want to load parsers into contexts for the parsespaces context.
	// we have ahabit but it doesn't use contexts
		// ahabit loads into bootstrap context. we can reference from there.
}

ref spacesuntildelimiter(ref context, ref stream, ref delimiter)
{
	std::string word;
	while (true) {
		
	}
}

void parseopenbrace(ref context, ref stream, ref parsertxt)
{
	ref bracetxt = stream.get(dovalue);
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
	else { throw intellect::level2::noteconcept().link("is", "unexpected-word", "word-space", stream); }
	//ref contents = ctxlookup(context, txt2ref("parse-until-delimiter"))(stream, context, brace2);

	// i'd like to be able to evaluate things in either an expression context or a statement context.
	// this would mean having a contextual evaluator, I suppose
	// here, let's produce a simple list of words
}

void parsebootstrap(ref stream, ref context)
{
	std::iostream & ss = *stream.val<std::iostream*>();
	std::string lookupstr;
	ss >> lookupstr;
	ref lookup = lookupstr;
	std::list<std::string> comments;
	while (true) {
		std::string cmd;
		ss >> cmd;
		if (!ss) { break; }
		if (cmd == "//") {
			std::string comment;
			std::getline(ss, comment);
			comments.push_back(comment);
		} else if (cmd == "concept") {
			std::string name;
			ss >> name;
			ref c = lookup(parsevalue(name));
			std::string tok;
			ss >> tok;
			if (tok != "[") { throw intellect::level2::noteconcept().link("is", "missing-[-after-concept"); }
			while (true) {
				std::string type, target;
				ss >> type;
				if (type == "]") { break; }
				ss >> target;
				c.link(lookup(parsevalue(type)), lookup(parsevalue(target)));
			}
		} else if (cmd == "habit") {
			std::string name;
			ss >> name;
			std::string tok;
			ss >> tok;
			if (tok != "[") { throw intellect::level2::noteconcept().link("is", "missing-[-after-habit-name"); }
			ref args = intellect::level2::noteconcept();
			auto & argvec = intellect::level2::data<intellect::level2::vector>(args);
			while (true) {
				std::string arg;
				ss >> arg;
				if (arg == "]") { break; }
				argvec.push_back(ref(arg).ptr());
			}
			ss >> tok;
			if (tok != "[") { throw intellect::level2::noteconcept().link("is", "missing-[-after-habit-needs"); }
			std::set<std::string> values;
			values.insert("context");
			values.insert("self");
			while (true) {
				std::string arg;
				ss >> arg;
				if (arg == "]") { break; }
				args.link("information-made", arg);
				// need to seed values with argument names
				values.insert(arg);
			} // information-made is unused.  is a 'stub' for if-needed-later.
			ref("set-steps")(name, args);
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			ss >> tok;
			if (tok != "[") { throw intellect::level2::noteconcept().link("is", "missing-[-after-habit-makes"); }
			std::map<std::string,ref> labels;
			ref laststep = name;
			labels["return"] = "nothing";
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			while (true) {
				std::string label, action, result;
				ss >> action;
				if (action == "]") { break; }
				if (action == "//") {
				}
				if (action[action.size()-1] == ':' || action[action.size()-1] == ',') {
					label = action;
					label.resize(label.size() - 1);
					if (label == "return") { throw intellect::level2::noteconcept().link("is", "return-label-used"); }
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
						labels.emplace(action, intellect::level2::noteconcept());
					}
					labels[action].link("label", action);
					if (laststep.linked("next-step")) { throw intellect::level2::noteconcept().link("is", "jump-from-nowhere", "label", action); }
					laststep.link("next-step", labels[action]);
					laststep = "nothing";
					continue;
				}
				if (action == "if") {
					ref cond = lookup(parsevalue(stream));
					ss >> action;
					if (action[action.size()-1] != '.') {
						throw intellect::level2::noteconcept().link("is", "condition-is-not-label", "action", action, "cond", cond);
					}
					if (!laststep.isa("condition-step")) {
						throw intellect::level2::noteconcept().link("is", "if-not-following-condition", "cond", cond, "action", action);
					}
					if (label.size()) {
						throw intellect::level2::noteconcept().link("is", "if-case-has-label", "cond", cond, "action", action, "label", label);
					}
					action.resize(action.size()-1);
					if (!labels.count(action)) {
						labels.emplace(action, intellect::level2::noteconcept());
						labels[action].link("label", action);
					}
					ref("condition-step-set")(laststep, cond, labels[action]);
					// if this improves from being  jump, remember to
					// update laststep to end of any 'anything' branch
					continue;
				}
				if (laststep == "nothing" && label.size() == 0) { throw intellect::level2::noteconcept().link("is", "no-path-to-code"); }
				if (label.size() && !labels.count(label)) {
					labels[label] = intellect::level2::noteconcept();
					labels[label].link("label", label);
				}
				ref nextstep = label.size() ? labels[label] : intellect::level2::noteconcept();
				if (action == "?" || action == "pick") {
					std::string cond;
					ss >> cond;
					if (!values.count(cond)) {
						throw intellect::level2::noteconcept().link("is", "condition-must-be-in-context", "condition", cond);
					}
					laststep = ref("set-condition-step")(nextstep, laststep, cond, intellect::level2::noteconcept().link("anything", "nothing"));
				} else {
					// otherwise, action is an action, and we have to read the right number of args
					if (laststep.isa("condition-step")) {
						if (ref("condition-step-get")(laststep, "anything") != "nothing") {
							if (label.size() == 0) {
								throw intellect::level2::noteconcept().link("is", "condition-already-has-anything-branch-and-steps-follow", "condition", laststep);
							}
						} else {
							ref("condition-step-set")(laststep, "anything", nextstep);
						}
					} else if (laststep != "nothing") {
						laststep.link("next-step", nextstep);
					}
					ref habit = values.count(action) ? action : lookup(action);
					ref order = makehabitinformationorder(habit);
					ref neededmap = intellect::level2::noteconcept();
					ref knownmap = intellect::level2::noteconcept();
					ref informationnames = intellect::level2::noteconcept();
					std::string linerest;
				       	std::getline(ss, linerest);
					std::stringstream ss2(linerest);
					ref stream2 = alloc(intellect::level0::concepts::allocations(), (std::iostream*)&ss2);
					auto args = order.val<intellect::level2::vector>();
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
					ref mademap = intellect::level2::noteconcept();
					if (result.size()) {
						mademap.link("result", values.count(result) ? result : lookup(result));
					}
					ref("set-context-step")(nextstep, "nothing", knownmap, neededmap, mademap, habit);
					laststep = nextstep;
				}
			} 
		} else {
			throw intellect::level2::noteconcept().link("is", "parse-error", "stream", stream, "unexpected-word", cmd);
		}
	}
}

}
}
