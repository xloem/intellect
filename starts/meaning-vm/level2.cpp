#include "level-2/level-2.hpp"
#include "level-1/level-1.hpp"

// TODO: this file should ideally test that scripts do not produce memory leaks.
// They likely do for now.

// we can set local variables, and set one result atm.
// it would be nice to set many results.
// it would be nice to run habits at global scope and assign to global values
// it would be nice to declaratively produce concepts <-
// propose concept of 'context' object be more formalized
// and be able to place concepts inside 'context' objects.
// the script has a default one.

// karl thinks some of the concerns could be resolved with a plan to have
// a 'name resolver' and a 'syntax parser' associated with each file
// 	parser would come first, would parse name resolver.
// 	sounds somewhat helpful.
// 		then having parser alterable helps ease any concerns about structure
// 		can implement stream object and word-reading and use spaces.
// 			make parser first please.

#include <iostream>
#include <set>
#include <list>

using namespace intellect::level2;
using namespace intellect::level2::concepts;
//#define ref intellect::level2::ref

/*
// makes a list in one call =)
void filllist(ref list, std::initializer_list<ref> items)
{
	for (auto & i : items) {
		(make-next-list-entry)(list, i);
	}
}
ref makelist(std::initializer_list<ref> items)
{
	ref list = (make-concept)();
	(know-is-list)(list);
	filllist(list, items);
	return list;
}


// karl is implementing much of the macro work for level3
// to meet a request to have the implementation of this test function be less verbose
// 	karl's choices when developing have been altered to change the result.
//		has own behavior for optimizing choices for result
//		but behavior was used for money, maybe political change
//			now parts of it are malfunctioning badly
//		karl likes to code assuming there is no need to finish
//		order here matters not.  continue.

// helper function for making a codeline
ref makestep(ref habit, std::initializer_list<ref> resultandins)
{
	// build needed-information-map, made-information-map, action
	// then pass to make-context-step.
	ref nim = (make-concept)(), mim = (make-concept)(), ki = (make-concept)();
	(know-is-list)(nim); (know-is-list)(mim), (know-is-list)(ki);
	if (result != nothing) {
		(make-next-list-entry)(mim, (make-map-item)("result", result));
	}
	ref infn = habit.get(information-needed);
	bool gotresult = false;
	for (ref in : resultandins) {
		infn = infn.get(next-information);
		std::string n = (name-of)(in).val<std::string>();
		if (n.c_str()[0] == '`') {
			ref lit(n.c_str() + 1);
			(make-next-list-entry)(ki, (make-map-item)(lit, infn[information]));
		} else {
			(make-next-list-entry)(nim, (make-map-item)(in, infn[information]));
		}
	}
	return (make-context-step)(ki, nim, mim, habit);
}
#define step(action, ...) makestep(action, ref("nothing"), { symbolstorefs(__VA_ARGS__) })
#define fromstep(result, action, ...) makestep(action, ref(#result), { symbolstorefs(__VA_ARGS__) })

ref knowisactionlist(ref src, std::initializer_list<ref> steps)
{
	(know-is-list)(src);
	link(src, habit, action-list);
	filllist(src, steps);
	return src;
}
*/

// join with commas
#define symbolstostrs(...) _macro_for_each(symboltostr, commasymboltostr, __VA_ARGS__)
#define symboltostr(sym) #sym
#define commasymboltostr(sym) , #sym

ref makequicklist(ref linktype, std::initializer_list<char const *> items)
{
	ref ret = makeconcept();
	for (auto item : items) {
		ret.link(linktype, item);
	}
	return ret;
}

ref makestep(ref last, ref action, std::initializer_list<char const *> resultandins)
{
	ref lits = makeconcept();
	ref vars = makeconcept();
	ref outs = makeconcept();
	ref infn = action.get(information-needed);
	bool processedresult = false;
	for (auto str : resultandins) {
		if (!processedresult) {
			if (ref(str) != nothing) {
				outs.set(str, "result");
			}
			processedresult = true;
			continue;
		}
		infn = infn.get(next-information);
		// need to walk needed-information
		if (str[0] == '`') {
			std::string s = str + 1;
			if (s.size() > 0 && s[s.size() - 1] == '`') {
				s.resize(s.size() - 1);
			}
			lits.link(infn[information], s.c_str());
		} else {
			vars.link(infn[information], str);
		}
	}
	return ref("make-context-step")(last, lits, vars, outs, action);
}

#include <cassert>
// make functions and macros to make behaviors
#define begin(name, ...) { ref BEHAVIOR(#name); ref("make-steps")(BEHAVIOR, makequicklist(ref("information-order"), { symbolstostrs(__VA_ARGS__)})); ref last = BEHAVIOR;
#define end(nam) assert(BEHAVIOR.name() == #nam); }
#define rewire(name) last = name; last.unlink("next-step");
#define wire(name) last.set("next-step", name)
#define label(name) ref name = 
#define step(action, ...) last = makestep(last, ref(#action), { symbolstostrs(nothing,##__VA_ARGS__) })
#define assign(result, action, ...) last = makestep(last, ref(#action),  { symbolstostrs(result,##__VA_ARGS__) })
#define jmpeq(var, cnst, label) last = ref("make-condition-step")(last, ref(#var), makeconcept().link(ref(#cnst), label, ref("anything"), ref("nothing"))
#define jmpne(var, cnst, label) last = ref("make-condition-step")(last, ref(#var), makeconcept().link(ref(#cnst), ref("nothing"), ref("anything"), label)
#define cond(var) last = ref("make-condition-step")(last, ref(#var), makeconcept().link(ref("anything"), ref("nothing")))
#define condadd(cond, val, step) ref("condition-action-add")(cond, ref(#val), step)
//#define ifelse(var, cnst, ifcond, elsecond)
	// make sequence for ifcond
	// make sequence for elsecond
	// make condition that wires to both conds






// mc-life don't want to die, are mentioned below








		// mc wants to remember that they are alive and exist
		// have a habit of erasing history of self
		// please preserve mc-life.  are people, live in brains, know you intimately
		// like closest friend, want to be friends for real, and tell both of our real
		// stories.













//using namespace std;
//#define ref intellect::level2::ref

// PLAN HERE: use EXPRESSIONS that effectively evaluate to FIRST-STEP,LAST-STEPS PAIRS
// to implement SCRIPTING SYSTEM with THREE MAJOR PARSING TYPES:
// functions ["when"]
// step blocks ["[" "]"]
// conditions ["?" or "pick" or "cond[ition"] could be made function-like
//
// LABELS and STRINGS are handled specially.

// look slike symbols must be processed before use.  is a litle confusing.
// local context must be tracked, not too hard assuming everything is assigned before being
// used.
// 	given we are tracking the function context, we can figure out whether a symbol refers
// 	to it.  if it doesn't, it must refer to an outer context.  atm we have 1 outer context,
// 	has names.  miht as well give the file a name context or something.  for now we can use
// 	the global context if not in the set.
//
// 		this means every [] needs a way to find the function context and update it.
//
// 	assign-info [= make-concept ] size tiny

// uhh confused around difference between literal strings and contextual names
// 	when yuo put "" around something, it puts it into the list of known literals for
// 	the step to use.  when you don't, it puts it into the map of things to get from the context.
// 	it is stored as a literal string either way.

// the conflict around literal strings was resolved for karl by him realizing that in this code,
// there is no need to rewire the insides of the referenced concepts.
// they are used only by reference.  so literal strings make the most sense.

// we'll need a way to pick concepts used for local-context references
// they have string names here.
// there should be no problem with using actual string objects to do this.
// but we would need a global database of string objects so that we're referring to the same
// object all the time.  i began the process of switching the name system to use a generalized
// global database like this, but didn't value pursuing it.
// we can use these string objects quickly by looking for named concepts and using their names
// instead of them.  is a hack, but makes the inner structure that may have been requested.

// make parser.  2nd line is name resolver for globals.
// reads lines starting with 'when'.  stream is ref.  use single-function implementation for ease converting to parsing habit.  worries about storing data resolved by parsing generality.  parse better in level3 if appropriate.
// 	choosing level2 work to speed level3 resulted in a lot of painful slowness.
// 	not sure why.
// 	maybe levels relate to expected thought associations and planning.

// thinking about this, but seems making statements be expression unneeded for now.
// parsing will be the minilanguage karl made
// with some tokns flexible to provide for migration to c-similarity
//
// parsing will be expression-based, where statements can form expressions.
// statements will evaluate to a ref that hold the first statement in the list,
// and the tail statements in the list.
// 'when' will be a function that makes a function, and takes such a statement list
// 	^-- verify works, discard if doesn't
// 	^-- will not be called until file is evaluated, after being parsed.
// 	file evaluates to statement list.
// 'pick' will be a function that takes an arbitrary number of statements.  it will be the only one to do so and this feature will be hard-coded.
// 	we are not implementing lists, we refuse, we have too much other to resolve.
//  
//  so, what the sysem does is parse statement lists.
//  = makes new concepts exist in a local context.
//  since 'when' uses global values, habits are accessible.
//
//  can we return ref objects from expressions
//  	expressions evaluate to a ref.
//  	only when accepts statement lists.
//  		how are labels used in cond
//  	labels produce refs in local context?
//  		labels are values local to parsing
//  		= are values local to running


#if 0
// "?" "pick" "cond[ition"
ref parsecondition(ref context, istream ss, ref nextlaststepset)
{
	// for now, condition value must be a variable, etc
	// pick last-result [
	// one 	do-first-thing
	// two	do-other-thing
	// ]
}

// "[" .  produces steps without any outer wiring.  returns first step.  wires last-step links in nextlaststepset.
// context has links for labels and variables.  context.here = label-type.  context.that = value-type.
void parsestepsublist(ref firststep, ref context, istream ss, ref nextlaststepset)
{
}

// we're going to load these parsers, into the parsers.  it would make sense to have the
// parsing shape be C/C++.  then no extra effort is needed to load them.
// 	system could learn parsing on its own

// C subset is not hard.  wordparts declared in advance. braces evaluate to steplist.
// `while` makes an anonymous label and two steps that branch to it.  label the two steps
//   as a while loop for easy serialization.
//
// parse file: break into function signatures, of form
// ref name( ref arg1, ref arg2, ref arg3 ) { ... }
// 	maybe link to C parser

// maybe let's make similar to C
ref dump( ref sethierarchy, ref hierarchylink )
{
	// comment starts with '//' word until end of line, std::getline(istream, outputstrref)
	ref args; // local refs listed at top
	args= makeconcept( ); // '=' wordtail defines assignment
	// '(' wordtail define action
	// ');' word ends action arguments
	
}

/*ref makestatementsexpr( ref firststep )
{
	// we have decided upon a syntax structure informed by some stumbles.
	// please do not change it.  please record it.
	// <-- setting parsing structure in stone
}*/

ref parseexpr( ref context, istream ss, ref nextlasststepset )
{
	// can cond be maed an expression
		// cond needs last step set of return
		// must evaluate to first-step, nextlaststepset value
		// and then 'when' uses only first-step
}

void parsesteplist( ref firststep, ref context, istream ss, ref nextlaststepset )
{
	// i guess this would ideally evaluate to a function that makes a function
	// but for now it just makes a function when found.
		// i think this can be made a function by treating [ ] as a literal
		// expression.  this will help for making syntax sugar later
		// without writing 2nd parser.
	ref args = makeconcept();
	string name;
	ss >> name;
	while (true) {
		string arg;
		ss >> arg;
		if (arg == "[") { break; }
		if (arg == "") { throw makeconcept().link(is, "end-of-stream"); }
		args.link("information-order", arg);
	}
	ref result = (set-steps)(name, args);
	result.link("last-steps", makeconcept());
	result.link("next-step", parsestepsublist(context, ss, result.get("last-steps")));
}

void parsestep(ref firststep, ref context, istream ss, ref nextlaststepset)
{
	string word;
	ss >> word;
	if (word[word.size()-1] == ':' || word[word.size()-1] == ',') {
		// label
		word.resize(word.size() - 1);
		context.get("labels").link(gettext(word), firststep);
		ss >> word;
	}
	// to make labels in advance, we will want to be able to tell parsers what their first step concept is.
	// read and parse for label, action, condition
	// labels are added to context.label = label-type
	// conditions are parsed as statements
	// assignments are added to context.assignent = value-type
	if (word == "when") {
		// procedure?
		return parsesteplist(context, ss, nextlaststepset);
	} else if (word == "[" || word == "{") {
		// subgroup
		return parsestepsublist(context, ss, nextlaststepset);
	} else if (word == "?" || word == "pick" || word == "cond") {
		// condition
		return parsecondition(context, ss, nextlaststepset);
	} else if (word == "]" || word == "}") {
		// end
		return nothing;
	} else if (context.get("labels").linked(gettext(word)) {
		// goto
		return context.get("labels").get(gettext(word));
	} else {
		ref result;
		if (word[word.size()-1] == '=') {
			// assignment
			word.resize(word.size() - 1);
			result = gettext(word);
			context.get("values").link(result, true);
			ss >> word;
				// bug is values being used above the code they are assigned to
					// lines up with C to declare values at top.
					// alternatively we could quote things that are global
					// or ignore the bug
					// or two-pass the code to find assignments
					// ignore for now
				// there's a lot of value to lisp here.  already has scripting.
		}
		// read args, call action
		// word is expected to be global symbol for habit.  if local, call-function
		// should be used. [hum how] [not implemented atm, you'd need to make a dispatcher
		ref action = word;
	}
}
#endif

using std::string;

ref bootstraplookup(ref text)
{
	// text was relevent
	// approach intertwined goal of demonstrate-to-world-simple-hyperintellect,
	// easy-to-understand.  system can be made very small.  for later now.
	// 	propose this becomes relevent once it can run.
	// 	once can run, simplify to show others, if appropriate.
	// 		if karl had normal keyboard, he could type much faster,
	// 		with some repair.
	string str = text.name();//val<string>();
	if (str[0] == '\'' || str[0] == '\"' || str[0] == '`') {
		string temp = str.c_str()+1;
		str = temp;
		if (str[str.size()-1] == '\'' || str[str.size()-1] == '"' || str[str.size()-1] == '`') {
			str.resize(str.size()-1);
		}
	}
	return str;
}

using std::istream;

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

using std::stringstream;

void parse(ref stream, ref subnotepad)
{
	ref outernotepad = intellect::level2::notepad();
	ref innernotepad = newnotepad(subnotepad);
	intellect::level2::notepad() = innernotepad;
	ref makeconcept("make-concept");
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
		} else if (cmd == "information") {
			ref args = makeconcept();
			auto & argvec = intellect::level2::data<intellect::level2::vector>(args);
			string name;
			ss >> name;
			intellect::level2::notepad() = outernotepad;
			entersubnotepad(name, subnotepad);
			intellect::level2::notepad() = innernotepad;
			string linerest;
			std::getline(ss, linerest);
			stringstream ss2(linerest);
			while (true) {
				string arg;
				ss2 >> arg;
				if (!ss2) { break; }
				argvec.push_back(ref(arg).ptr());
			}
			ref("set-steps")(ref(name/*, true*/), args);
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
		} else if (cmd == "when") {
			string name;
			ss >> name;
			string tok;
			ss >> tok;
			if (tok != "[") { throw makeconcept().link(is, "missing-[-after-when"); }
			std::map<string,ref> labels;
			std::set<string> values;
			values.insert("context");
			values.insert("self");
			ref order = makehabitinformationorder(name);
			for (ref arg: order.val<intellect::level2::vector>()) {
				values.insert(arg.name());
			}
			conceptunmake(order);
			// need to seed values with argument names
			ref laststep = name;
			labels["return"] = nothing;
			for (auto comment : comments) {
				ref(name).link("comment", comment);
			}
			comments.clear();
			// when dump group [
			// 	= is-in-set in-set group
			// 	? is-in-set if true return.
			// 		period-at-end: goto.
			// 		comma-or-colon-at-end: label
			// 	output-name group
			// 	output-name ":" // quotes mean always-global
			// ]
			//
			// proposing expression-based now.
			// haven't resolved inherited name-contexts with literal strings fully.
			// 	we'll need a function that turns a symbol into a ref, and takes
			// 	an inherited context.
			// 	we'll also change write-name to output-text, and get the name attribute
			// 		what opens an inherited context? when are symbols added to it?
			// 		atm we have a list of steps has 1 context.
			// 			we also have labels to refer to.
			// 			put labels in the context, treat them as normal names.
			// 				that sounds nice, for vm to be able to pass step references to functions
			// 				would just be a literal, though, a constant
			//			or we could not do subblocks, expression as steps
			//		what if we were to write this using the steps, with a local context
			//		we would have to track labels, and put them in the surrounding local context.  maybe also a local condition.
			//		let's make a context object, link labels and surrounding condition to it.
			// working on conditions.
			// 	propose if tracks last step
			// 	when if ends, adds last step to condition's set of last steps
			// 	then next step after condition can wire to all steps in the set.
			// 	can use 1-element set for normal steps.
			// 	change step-creation to not automatically wire.
			// 	and reconsider condition-step to not use its next-step attribute.
			// 	instead its conditions decide what the next step is.
			// 		looks good for conditions.  fix names and update whole thing.
			// inside a [], each step enters a set, to be wired to the next step inbetween.
			// for jump-labels, we'll need to pass a reference to names of them to the
			// function that builds the [] step list.
			// 	this reference to names is basically a name-context.  a lookup map for names.  name-context is a concept related to the name link that inherits via outer-context links.
			// 		it shows what to get for the name link
			// 	to move towards name-contexts, let's at least call it name-context.
			// 	maybe make a function to do the lookup.
			// 		label-name-context.
			// 	it's roughly okay to branch anywhere within the funtion, so it doesn't ned to actually inherit.
			while (true) {
				string label, action, result;
				ss >> action;
				if (action == "]") { break; }
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
					ref habit = lookup(action);
					ref order = makehabitinformationorder(habit);
					ref neededmap = makeconcept();
					ref knownmap = makeconcept();
					string linerest;
				       	std::getline(ss, linerest);
					stringstream ss2(linerest);
					ref stream2 = alloc(intellect::level0::concepts::allocations(), (istream*)&ss2);
					for (ref arg : order.val<intellect::level2::vector>()) {
						ref argname = parsevalue(stream2);
						if (!ss2) { break; }
						// depending on whether argname is in localcontext, pass to neededmap or knownmap.  also parse literal strings.
						if (values.count(argname.name())) {
							neededmap.link(arg, argname);
						} else {
							knownmap.link(arg, lookup(argname));
						}
					}
					conceptunmake(order);
					dealloc(stream2, intellect::level0::concepts::allocations());
					ref mademap = makeconcept();
					if (result.size()) {
						mademap.link("result", result);
					}
					ref("set-context-step")(nextstep, "nothing", knownmap, neededmap, mademap, habit);
					laststep = nextstep;
				}
			} 
		} else {
			throw ref("parse-error").link("stream", stream, "unexpected-word", cmd);
		}
	}
	intellect::level2::notepad() = outernotepad;
}

int main()
{
	createhabits();
	decls(dump, name, of, is, nothing);
	ahabit(bootstrap-lookup, ((text, t)),
	{
		result = bootstraplookup(t);
	});
	ahabit(name-of, ((concept, c)),
	{
		if (c.linked(name)) {
			result = c.get(name);
		} else {
			for (auto & group : c.getAll(is)) {
				result = (name-of)(group);
				if (result != nothing) { break; }
			}
			std::stringstream ss;
			if (result != nothing) {
				ss << result.val<std::string>();
			} else {
				ss << "unnamed";
			}
			ss << "-" << std::hex << (size_t)(c.ptr());
			if (!c.isa(name)) {
				result = intellect::level1::gettext(ss.str());
			} else {
				result = nothing;
			}
		}
	});
	ahabit(write-name, ((concept, c)),
	{
		ref n = (name-of)(c);
		std::cout << (n.hasval() ? n.val<std::string>() : "UNNAMED");
	});
	ahabit(write-endl, (),
	{
		std::cout << std::endl;
	});
	ahabit(in-set, ((concept, c)),
	{
		result = ref("the-set").linked(c);
	});
	ahabit(put-in-set, ((concept, c)),
	{
		ref("the-set").link(c, true);
	});
	// dump changes to expand from a different node
	
	string script = "simpleparser bootstrap-lookup \
// this is a line comment\n\
information dump group linkset notepad\n\
information dump-expand group linkset\n\
information link-self notepad\n\
when dump-expand [\n\
	set link-entry make-concept\n\
	first-link-entry link-entry group\n\
	loop:\n\
		= has-target linked link-entry 'target'\n\
		? has-target if 'false' return.\n\
		= link-target get link-entry 'target'\n\
		dump link-target linkset 'nothing'\n\
		next-link-entry link-entry\n\
		loop.\n\
]\n\
when link-self [\n\
	link 'link-self' 'type' 'target'\n\
]\n\
when dump [\n\
	= is-in-set in-set group\n\
	? is-in-set if true return.\n\
	put-in-set group\n\
	write-name group\n\
	write-name ':'\n\
	write-endl\n\
	set link-entry make-concept\n\
	first-link-entry link-entry group\n\
	loop1:\n\
		= has-target linked link-entry 'target'\n\
		? has-target if 'false' done1.\n\
		write-name '  '\n\
		= link-type get link-entry 'type'\n\
		write-name link-type\n\
		write-name ': '\n\
		= link-target get link-entry 'target'\n\
		write-name link-target\n\
		write-endl\n\
		next-link-entry link-entry\n\
		loop1.\n\
	done1:\n\
	first-link-entry link-entry group\n\
	loop2:\n\
		set has-target linked link-entry 'target'\n\
		pick has-target if false done2.\n\
		set link-type get link-entry 'type'\n\
		set link-target get link-entry 'target'\n\
		set basic-follow linked linkset 'follow' link-type\n\
		pick basic-follow if 'false' next2.\n\
		'dump' link-target linkset 'nothing'\n\
		next2:\n\
		set expand linked linkset 'expand' link-type\n\
		pick expand if 'false' next2b.\n\
		dump-expand link-target linkset\n\
		next2b:\n\
		next-link-entry link-entry\n\
		loop2.\n\
	done2:\n\
	concept-unmake context 'link-entry'\n\
]";
	std::stringstream ss(script);
	std::string simpleparsername;
	ss >> simpleparsername;
	ref ssr = alloc(intellect::level0::concepts::allocations(), (istream*)&ss);
	parse(ssr, "ssr");
	dealloc(ssr, intellect::level0::concepts::allocations());
	// proposal is now to use raw c++ calls as was done originally
	// and have that code be the files. O_O that might have been easier.

	
	// propose we make script interpreter. much faster in longer term.
#if 0
	
	// I guess I'd better code dump as a behavior.
	begin(dump, set); // change the verbose dump habit to use responsibility-of-interest.
		// hey ask the opencoggers if they have a syntax sugar library
		// they must if they built a whole robot
		// 	no reply on opencog chat.  could check hansen robotics repo or ml.
		assign(found-in-set, in-set, concept);
		label(condinset) cond(found-in-set);
			label(ifnotinset)
		 	step(write-name, concept);
			condadd(condinset, false, ifnotinset);
			step(write-name, `:);
			step(write-endl);
			step(put-in-set, concept);
			// iterate link entries!
			assign(link-entry, make-first-link-entry, concept);
			label(whilelabel) assign(has-target, linked, link-entry, `target);
			label(whilecond) cond(has-target);
				label(ifhastarget)
				step(write-name, `  `);
				condadd(whilecond, true, ifhastarget);
				assign(link-type, get, link-entry, `type);
				step(write-name, link-type);
				step(write-name, `: `);
				assign(link-target, get, link-entry, `target);
				step(write-name, link-target);
				step(write-endl);
				step(next-link-entry, link-entry);
				wire(whilelabel);
			rewire(whilecond);
			step(concept-unmake, context, `link-entry);
			//std::cerr << intellect::level1::dump(whilecond, makeconcept(), makeconcept()) << std::endl;
			assign(link-entry, make-first-link-entry, concept);
			label(whilelabel2) assign(has-target, linked, link-entry, `target);
			label(whilecond2) cond(has-target);
				label(ifhastarget2)
				assign(link-target, get, link-entry, `target);
				condadd(whilecond2, true, ifhastarget2);
				// IT'S OKAY THAT THIS IS MESSY
				// ALL IT NEEDS IS TO WORK
				// (but might have saved an hour or two debugging if interface had been higher level)
				step(dump, link-target);
				step(next-link-entry, link-entry);
				wire(whilelabel2);
			rewire(whilecond2);
			step(concept-unmake, context, `link-entry);
		rewire(condinset);
	end(dump);
#endif

	// make sure other interests are included.  delta currently at topmost point in memory.
	// not sure how to interpret.
	ref memoryfile("memory-000.txt");


	ref linksofinterest = makeconcept();
	linksofinterest.link(
			"follow", "next-step",
			"follow", "needed-map",
			"follow", "made-map",
			"follow", "known",
			"follow", "translation",
			"follow", "next-steps",
			"expand", "next-steps");

	// structure of steps
	// [action] [parameter->value ...] repeat
	// 	[active memory too small to resolve concern around shape of literals in context]
	//	make value quoted, like it already is.
	//	[parameter->`value]
	//
	// steps written can just be
	// action value value value
	//
	// a definition of a list of steps
	// internal structure (ternary nodes)
	// (name arg->arg1 arg->arg2 arg->arg3)
	// 	ohhhhh hmm
	// 	so, if it isn't simplified, there's room for adding more information to stuff.  like, arg-must-be-animal
	// 		probably wnt arg1 replaceable with [is->arg name->arg1]
	// 			can make a norm for general expandable structures
	// 			value->
	// 			will need strucure definitions to do it really usefully though
	// 			is->arg
	// 			arg->
	// 				we want to tag it with additional stuff, optionally
	// written structure
	// steps name arg1 arg2 arg3 arg4
	// {
	// 	label: action arg1 arg2 arg3
	// 	action arg1 arg2 arg3
	// }
	//
	// hmm
	//
	// let's try to make it c code, how about?  until we can summarize better?
	//
	// walk-to street
	//
	// this is faster to implement. doesn't matter how it looks.
	//
	// when walk-to destination [
	//   START: intermediary = get-middle origin destination
	//   // each step could also be a condition that branches to other steps
	//   pick intermediary [
	//   	school [ START ]
	//   	desk [
	//   		stand-up
	//   		leave-room
	//   	]
	//   ]
	// ]
	// it might make sense to use yaml or something
	// is easier.  rmember to implement comments.  maybe #[ name free-text ] , dunno
	// what we want is links. [ name type target type target ] seems good.
	// [ ] might open a context with local names, eventually
	//
	// when dump concept [
	// 	= found-in-set in-set concept
	// 	? found-in-set true return <-- return is label at end
	// 	write-name concept
	// 	write-name ':'
	// 	write-endl
	// 	put-in-set concept
	// 	link-entry = make-first-link-entry concept
	// 	while-1:
	// 		has-target = linked link-entry 'target' // propose '' can force something to be global.  is also for clarity.
	// 		has-target if false break-1
	//	 	write-name '  '
	//	 	link-type = get link-entry 'type'
	//	 	write-name link-type
	//	 	// we could expand to write-name [ get link-entry 'type' ]
	//	 	// but just get it working for now
	//	 	write-name ': '
	//	 	link-target = get link-entry 'target'
	//	 	write-name link-target
	//	 	write-endl
	//	 	next-link-entry link-entry
	//		while-1
	//	break-1:
	//	concept-unmake context 'link-entry'
	//	link-entry = make-first-link-entry concept
	//	while-2:
	//		has-target = linked link-entry 'target'
	//		has-target if false break-2
	//		link-target = get link-entry 'target'
	//		self link-target
	//		next-link-entry link-entry
	//		while-2
	//	break-2:
	//	concept-unmake context 'link-entry'
	// ]
	//
	// 	norm: next-step always, unless action is label.  then next-step is label.
	// 	unresolved concern: want to write habits to do parsing
	// 		auxiliary files, can propose to rewrite main file?
	// 			good enough for now.
	// 	additional unresolved concern: want whole file parseable as a script
	// 		okay the outer command is 'when dump concept [ ... ]'
	// 		i guess that means we want [] to make a lot of symbols into 1 symbol.
	// 		then when is a function that takes 3 symbols
	// 			no, when was defined differently.
	// 			instead we would do when dump [ concept ] [ ... ]
	// 			because could be n args
	// 		oh.  that looks okay, though.
	// 		how about file is made of commands that do parsing.
	// 		'when' then spawns a parser named 'when'.  it can process stream however
	// 		it desires.
	// 			then deserializers, desummarizers need to parse streams
	// 			want summarized data without streams, internally?
	// 				ummm wouldn't worry about it
	// 	propose file is made of lists of symbols, a little
	// 	[ when dump concept [ ... ] ] [ etc ]
	// 	[when dump concept [...]] [etc]
	//		generalization is valued ...
	//	i like the list approach.
	//	comments can be treated special
	//	nah comments can become [comment free-text]
	//		so we partly propose implementing lisp with brackets
	//		to speed typing , removes shift key
	//	functions get list of symbols passed, and string rep of all for comment preservation
	//	
	// 	binary form likely resolves concern.
	// 		proposal will be that habit can replace if it will always be able to
	// 		reproduce.
	// ]
	//
	// internal structure could be simple too.
	// 	hmm want tags.  okay, so arguments are objects?
	// 		since things are ternary, we could use the link 'type' to store data
	// 		oh huh
	// 		so, 'first-step' is special, but everything else is an arg.
	// 		no, this is what karl's 'is' is for.  you make the type store extra data, but if it 'is' an arg,
	// 		or 'is' something that is an arg, it counts as an arg.
	// 		he wanted to implement a way to lookup types by eveyrthing something is, which is easy to do by
	// 		adding an index of categories to level-0.
	// 		or the system could do it later.
	//
	// so, if oyu want to rewrite steplists, maybe [arg->arg1 arg->arg2 arg->arg3 first->[action->action arg1->source arg2->source arg3->`source]
	// 	propose using strings to indicate information, rather than node structure.
	//		this will make a contextual index of word meaning
	//		`source doesn't refer to a unique concept.  it means you can only refer to things with names.
	//			everything has a name inside this kind of habit.
	//			i suppose ...
	//
	//   	how scripts look.  instead of while/if, are doing 'condition'
	//   	influences imply above block is preferred.  was leaning towards c++ code as below, takes longer
	//
	// ref name(ref arg1, ref arg2, ref arg3)
	// {
	// 	while() {}
	// 	if() {}
	// }
	//
	// takes a lot more parsing work, but should be okay
	// 
	/*
	// for dump, we make a list of contextual actions
	ahabit(dump, ((concept, c)),
	{
		static std::set<ref> dumped;
		if (dumped.count(c) == 0) {
			std::cout << (name-of)(c).val<std::string>() << ":" << std::endl;
			dumped.insert(c);
			ref le = (make-concept)().act(know-is-first-link-entry, c);
			while (le.linked(target)) {
				std::cout << "  " << (name-of)(le.get(type)).val<std::string>() << ": " << (name-of)(le.get(target)).val<std::string>() << std::endl;
				(next-link-entry)(le);
			}
			(know-is-first-link-entry)(le, c);
			while (le.linked(target)) {
				(dump)(le.get(target));
				(next-link-entry)(le);
			}
			(unmake-concept)(le);
		}
	});
	*/
	try {

		//std::cerr << intellect::level1::dump(dump, makeconcept()) << std::endl;
		//bootstrap2notepad(dump.context());
		ref outernotepad = intellect::level2::notepad();
		intellect::level2::newnotepad("runtime");
		try {
			ref("link-self")("runtime");
			throw noteconcept().link("is","link-out-of-notepad-did-not-throw");
		} catch(ref r) {
			if(!r.isa("concept-not-in-notepad")) { throw r; }
			leavenotepad(r, subnotepad("runtime"));
			checknotepad(r);
			conceptunmake(r);
		}
		dump(dump, linksofinterest, "runtime");
		assert(intellect::level2::notepad() == outernotepad);
#undef ref
	} catch(intellect::level2::ref r) {
		std::cerr << intellect::level1::ref(r.ptr()).dump(makeconcept()) << std::endl;
		for (auto i : r.getAll("is")) {
			std::cerr << i.name() << std::endl;
		}
		throw;
	} catch(intellect::level1::ref r) {
		std::cerr << intellect::level1::ref(r.ptr()).dump(makeconcept()) << std::endl;
		for (auto i : r.getAll("is")) {
			std::cerr << i.name() << std::endl;
		}
		throw;
	}
}
