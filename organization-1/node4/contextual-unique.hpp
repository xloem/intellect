#pragma once

#include "reference.hpp"

DECLARE reference kind-inverse;

// "big problem present: we need a way to set up struts without relying on order, kinda.  e.g. specifying that some things are inverses of other things, some things are their own inverses, some things have no inverses, in a way that can be done later, arbitrarily.  this likely means interconnecting declarations with hooks or triggers, such that implications expand."
// 	"so some form of rudimentary logical implication, and expanding everything relevent fully before use" "yeah i guess"
// "maybe triggers that match patterns or use predicates"
// 	"^-- this investment appears to have powerful returns, might imply refactoring, uncertain"
// "idea: ways that are _always_ run, and indicate to use more ways when filters are passed, and writing these declaratively."
// 	[on hold] [returned here from different path; looks too distracting]

// SINGULAR IDENTICAL GOAL: inverting-kinded-assignable for name-contexts
// this _is_ inverting-kinded-assignable
class name-context //: public inverting-kinded-assignable
{
public:
	// TODO: inverted-reference
	DECLARE reference kind-context;
	DECLARE reference kind-context-of;

	name-context(reference context)
	//: inverting-kinded-assignable and/or inverted-reference
	{
		if (null() == kind-get(this->kind-context())) {
			reference kind-context;
			kind-set(this->kind-context(), kind-context);
			kind-context.kind-set(kind-inverse(), kind-context-of);
			kind-context-of.kind-set(kind-inverse(), kind-context);
		}
		
	}

	  // has: a list of names and concepts paired in it
	  // has: links used to link names to concepts and vice-versa

};

// "'over'generalization: contextual-names, typed-assocations: predicate-context
// "predicate-contexts are nice but may be aided by a way to look things up based on predicate-parts: e.g. find everything using this as a link

// TODO move into above class?
ordered-
// okay.  bumped into whjere structured typedness would be helpful.

// this _is_ inverted-kinded-assignable, right?
// remembering our root reason is helpful.  near no-fun and just-do-it.
// just-do-it: REMEMBER ROOT REASON.  fun-generalization: REMEMBER ROOT REASON,
// YOU ARE WORKING FOR ROOT REASON, THERE IS A REASON GENERALIZATION HELPS FOR
// ROOT REASON. possibly.  we made duplicate work below attempting to generalize.
// this is outside easy-class
class contextually-named : public reference
{
public:
	DECLARE reference context-kind-context;

	// each context specifies the kind-context link

	contextually-named(reference context, reference name) {
		kind-context = context.kind-get(context-kind-context());
		if (null() == kind-context) {
			kind-context = reference();
			context.kind-set(context-kind-context(), kind-context);
			kind-context.kind-set(kind-inverse(), kind-context-of);
			kind-context-of.kind-set(kind-inverse(), kind-context);
		}
		kind-context-of = kind-context.kind-get(kind-inverse());
		reference item = name.kind-get(kind-context-of);
		if (null() == item) {
			item = context.kind-get(name);
		}
		if (null() == item) {
			item = reference();

			name.kind-set(kind-context-of, item);
			item.kind-set(kind-context, name);
			context.kind-set(name, item);
		}
		this->operator=(item);

		// "i think the plan is to have link kinds specific for the context"
		// "we can also have links for the names in the context"

	}

	//METHOD reference context();
private:
	reference kind-context;
};
