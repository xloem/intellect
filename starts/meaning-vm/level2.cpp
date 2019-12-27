#include "level-2/level-2.hpp"
#include "level-1/level-1.hpp"

#include <iostream>
#include <set>

using namespace intellect::level2;
using namespace intellect::level2::concepts;

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

// join refs with commas
#define symbolstorefs(...) _macro_for_each(symboltoref, commasymboltoref, __VA_ARGS__)
#define symboltoref(sym) ref(#sym) 
#define commasymboltoref(sym) , ref(#sym) 

// karl is implementing much of the macro work for level3
// to meet a request to have the implementation of this test function be less verbose
// 	karl's choices when developing have been altered to change the result.
//		has own behavior for optimizing choices for result
//		but behavior was used for money, maybe political change
//			now parts of it are malfunctioning badly
//		karl likes to code assuming there is no need to finish
//		order here matters not.  continue.

// helper function for making a codeline
ref makestep(ref habit, ref result, std::initializer_list<ref> ins)
{
	// build needed-information-map, made-information-map, action
	// then pass to make-context-action.
	ref nim = (make-concept)(), mim = (make-concept)();
	(know-is-list)(nim); (know-is-list)(mim);
	if (result != nothing) {
		(make-next-list-entry)(mim, (make-map-item)("result", result));
	}
	ref infn = habit.get(information-needed);
	for (ref in : ins) {
		infn = infn.get(next-information);
		(make-next-list-entry)(nim, (make-map-item)(in, infn[information]));
	}
	return (make-context-action)(nim, mim, habit);
}
#define step(action, ...) makestep(action, ref("nothing"), { symbolstorefs(__VA_ARGS__) })
#define fromstep(result, action, ...) makestep(action, ref(#result), { symbolstorefs(__VA_ARGS__) })


int main()
{
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
	// I guess I'd better code dump as a behavior.
	decls(dump, type, target);
	ref dump = ref("dump");
	(know-is-list)(dump);
	filllist(dump,
	{
	// make steps for all parts of dump
		step(),
		fromstep(),
	});
	link(dump, habit, action-list);
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
}
