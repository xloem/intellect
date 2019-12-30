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
	// then pass to make-context-action.
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
	return (make-context-action)(ki, nim, mim, habit);
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

ref makestep(ref last, ref action, std::initializer_list<char const *> resultandins)
{
	ref lits = (make-concept)();
	ref vars = (make-concept)();
	ref outs = (make-concept)();
	ref infn = action.get(information-needed);
	bool processedresult = false;
	for (auto str : resultandins) {
		if (!processedresult) {
			if (ref(str) != nothing) {
				outs.set("result", str);
			}
			processedresult = true;
			continue;
		}
		infn = infn.get(next-information);
		// need to walk needed-information
		if (str[0] == '`') {
			std::string s = str + 1;
			if (s.size() > 0 && str[s.size() - 1] == '`') {
				s.resize(s.size() - 1);
			}
			lits.link(infn[information], str + 1);
		} else {
			vars.link(infn[information], str);
		}
	}
	return ref("make-context-action")(last, lits, vars, outs, action);
}

#include <cassert>
// make functions and macros to make behaviors
#define begin(name) { ref BEHAVIOR(#name); ref last = BEHAVIOR;
#define end(nam) assert(BEHAVIOR.name() == #nam); }
#define rewire(name) last = name; last.unlink("next-step");
#define wire(name) last.set("next-step", name)
#define label(name) ref name = 
#define step(action, ...) last = makestep(last, ref(#action), { symbolstostrs(nothing,##__VA_ARGS__) })
#define assign(result, action, ...) last = makestep(last, ref(#action),  { symbolstostrs(result,##__VA_ARGS__) })
#define jmpeq(var, cnst, label) last = ref("make-condition-action")(last, ref(#var), ref("make-concept")().link(ref(#cnst), label, ref("anything"), ref("nothing"))
#define jmpne(var, cnst, label) last = ref("make-condition-action")(last, ref(#var), ref("make-concept")().link(ref(#cnst), ref("nothing"), ref("anything"), label)
#define cond(var) last = ref("make-condition-action")(last, ref(#var), ref("make-concept")().link(ref("anything"), ref("nothing")))
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

int main()
{
	createhabits();
	decls(dump, name, of, is, nothing);
	ahabit(name-of, ((concept, c)),
	{
		if (linked(c, name)) {
			result = get(c, name);
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
			intellect::level1::givename(c, ss.str());
			result = get(c, name);
		}
	});
	ahabit(write-name, ((concept, c)),
	{
		std::cout << (name-of)(c);
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
	// I guess I'd better code dump as a behavior.
	begin(dump);
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
				assign(link-target, get, link-entry, `target);
				step(write-name, link-target);
				step(write-endl);
				step(next-link-entry, link-entry);
				wire(whilelabel);
			rewire(whilecond);
			step(concept-unmake, link-entry);
			assign(link-entry, make-first-link-entry, concept);
			label(whilelabel2) assign(has-target, linked, link-entry, `target);
			label(whilecond2) cond(has-target);
				label(ifhastarget2)
				assign(link-target, get, link-entry, `target);
				condadd(whilecond, true, ifhastarget2);
				// IT'S OKAY THAT THIS IS MESSY
				// ALL IT NEEDS IS TO WORK
				step(dump, link-target);
				step(next-link-entry, link-entry);
				wire(whilelabel2);
			rewire(whilecond2);
			step(concept-unmke, link-entry);
	end(dump);
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
	dump(dump);
}
