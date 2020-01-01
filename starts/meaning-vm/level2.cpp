#include "level-2/level-2.hpp"
#include "level-1/level-1.hpp"

#include <iostream>
#include <set>

using namespace intellect::level2;
using namespace intellect::level2::concepts;

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













using namespace std;
void parse(string script)
{
	stringstream ss(script);
	ss >> cmd;
	if (cmd == "when") {
		ref args = makeconcept();
		string name;
		ss >> name;
		while (true) {
			string arg;
			ss >> arg;
			if (arg == "[") { break; }
			args.link("information-order", arg);
		}
		ref result = (set-steps)(name, args);
		ref laststep = result;
		map<string,ref> labels;
		labels["return"] = nothing;
		// when dump group [
		// 	= is-in-set in-set group
		// 	? is-in-set if true go return.
		// 		period-at-end: goto.
		// 		comma-or-colon-at-end: label
		// 	output-name group
		// 	output-name ":" // quotes mean always-global
		// ]
		//
		// proposing expression-based now.
		// haven't resolved inherited name-contexts with literal strings fully.
		// working on conditions.
		// 	propose if tracks last step
		// 	when if ends, adds last step to condition's set of last steps
		// 	then next step after condition can wire to all steps in the set.
		// 	can use 1-element set for normal steps.
		// 	change step-creation to not automatically wire.
		// 	and reconsider condition-step to not use its next-step attribute.
		// 	instead its conditions decide what the next step is.
		// 		looks good for conditions.  fix names and update whole thing.
		while (true) {
			string label, action, result;
			ss >> action;
			if (action == "]") { break; }
			if (action[action.size()-1] == ':' || action[action.size()-1] == ',') {
				label = action;
				label.resize(label.size() - 1);
				if (label == "return") { throw makeconcept.link(is, "return-label-used"); }
				ss >> action;
			}
			if (action == "=" || action == "set") { ss >> result; ss >> action; }
			if (action[action.size()-1] == '.') {
				// is goto
				action.resize(action.size() - 1);
				if (!labels.count(action)) {
					labels.emplace(action, makeconcept());
				}
				labels[action].link("label", action);
				if (laststep.linked("next-step")) { throw makeconcept().link(is, "jump-from-nowhere", "label", action); }
				laststep.link("next-step", labels[action]);
				continue;
			}
			ref nextstep = label.size() ? labels[label] : makeconcept();
			if (action == "if") {
				ref cond;
				ss >> cond;
				ss >> action;
				if (action[action.size()-1] != '-') {
					throw makeconcept().link(is, "condition-is-not-label", "action", action);
				}
				action.resize(action.size()-1);
				if (!labels.count(action)) {
					labels.emplace(action, makeconcept());
				}
				(condition-step-add)(laststep, cond, labels[action]);
			}
			if (action == "?" || action == "pick") {
				string cond;
				ss >> cond;
				laststep = (set-condition-step)(nextstep, laststep, cond, makeconcept().link("anything", "nothing"));
				// todo: make a noop for 'anything' and use it in following actions
				// todo: handle if
			} else {
				// otherwise, action is an action, and we have to read the right number o args
				// if last-step is condition, connect to 'anything'
				// oooogh
			}
		} 
	} else {
		throw ref("parse-error").link("script", script, "unexpected-word", cmd);
	}
}

int main()
{
	createhabits();
	decls(dump, name, of, is, nothing);
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
				intellect::level1::givename(c, ss.str());
				result = c.get(name);
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
		result = linked("the-set", c);
	});
	ahabit(put-in-set, ((concept, c)),
	{
		link("the-set", c, true);
	});
	// dump changes to expand from a different node
	
	// propose we make script interpreter. much faster in longer term.
	
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

	ref memoryfile("memory-000.txt");
	decls(responsiblefor, responsibility, interest);
	link(responsibility-of-interest, responsiblefor, dump);
	for (ref a = dump; a.linked("next-step"); a = a.get("next-step")) {
		(responsibility-of-interest).link(responsiblefor, dump);
	}
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
		// for some reason it's only running the second loop
		// nothing is output on the first
		std::cerr << intellect::level1::dump(dump, makeconcept()) << std::endl;
		dump(responsibility-of-interest);
	} catch(intellect::level1::ref r) {
		std::cerr << intellect::level1::ref(r.ptr()).dump(makeconcept()) << std::endl;
		throw;
	}
}
