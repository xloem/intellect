#pragma once

#include "common.hpp"
#include "ref.hpp"
#include "concepts.hpp"

namespace intellect {
namespace level2 {

void ensurehabitscreated(); // for running from static code, for construction order

namespace concepts {

	// make a link
	// link(source, type, target)
	static ref link("link");

	// get if a link is permanent
	// result = (link-crucial)(source, type, target)
	static ref crucial("crucial");
	
	// set a link to be permanent
	// (link-set-crucial)(source, type, target)
	static ref set("set");

	// get if a link exists
	// result = linked(source, type, target? = anything)
	static ref linked("linked");

	// remove a link
	// unlink(source, type, target)
	static ref unlink("unlink");

	// get a link target by type
	// result = get(source, type)
	static ref get("get");

	// set a link target, replacing an existing type if needed
	// set(source, type, target)

	// produce a totally new, empty concept, with no links to or from
	// result = (make-concept)()
	static ref make("make"), concept("concept");

	// produce a new concept by copying links and data from an old concept.
	// nothing will link to new concept.
	// result = (make-copy)(concept)
	static ref copy("copy");

	// destroy a concept forever, for handling temporary data.
	// this will fail if the concept is set to be crucial, or is in use.
	// (concept-unmake)(concept)
	static ref unmake("unmake");

	// get if a concept is set to be crucial
	// result = (concept-crucial)(concept)
	
	// set a concept to be crucial
	// r(concept-set-crucial)(concept)
	
	// place a concept in a new group.
	// forms a new link of type 'is', fails if one aleady exists
	// concept = (know-is)(concept, group)
	static ref know("know"), is("is")  ;

	// fill any concept with information about the first link entry in a concept
	// if there is a link in the entry, then [type] and [target] will be present
	// link-entry = (know-is-first-link-entry)(link-entry, concept)
	static ref first("first"), entry("entry");

	// fill any concept with information about the last link entry in a concept
	// link-entry = (know-is-last-link-entry)(link-entry, concept)
	static ref last("last");

	// change a link entry concept to be about the next link in the concept
	// link-entry = (next-link-entry)(link-entry)

	// change a link entry concept to be about the previous link in the concept
	// link-entry = (previous-link-entry)(link-entry)
	static ref previous("previous");

	// get if two link entry concepts refer to the same link entry
	// result = (same-link-entry)(link-entry-A, link-entry-B)
	static ref same("same");

	// remove a link entry from a concept
	// the entry will be about the next one, after the action
	// link-entry = (link-entry-unlink)(link-entry)
	
	// get if a link entry is set to be crucial
	// result = (link-entry-crucial)(link-entry)
	
	// set a link entry to be crucial & permanent
	// (link-entry-set-crucial)(link-entry)
	
	// add links to a concept facilitating use as a list
	// list = (know-is-list)(list)
	static ref list("list");

	// get the first list-entry of a list
	// result = (list-first-entry)(list)
	
	// get the last list-entry of a list
	// result = (list-last-entry)(list)
	
	// get a subsequent list-entry
	// result = (list-entry-next)(list-entry)
	
	// get a preceding list-entry
	// result = (list-entry-previous)(list-entry)
	
	// get the item associated with a list-entry
	// result = (list-entry-item)(list-entry)
	static ref item("item");

	// make a new entry in a list, holding an item concept
	// list = (make-next-list-entry)(list, item)
	
	// destroy forever an entry in a list.  for lists of temporaries.
	// the result is the next entry.
	// result = (list-entry-unmake)(list-entry, le)
	
	// action(list-entry, context) for every entry in a list until result made
	// result = (list-each-entry)(list, context, action)
	static ref each("each");

	// get if a list has an item in it
	// result = (list-has-item)(list, item)
	static ref has("has");

	// destroy forever an entry in its list, by the item it contains
	// for lists of temporaries
	// true = (list-item-entry-unmake)(list, item)
	
	// the .act level-2 member function can ease list creation.
	// it passes itself as the first argument to a habit
	// (make-concept)().act(know-is-list)
	// 	.act(make-next-list-entry, i1)
	// 	.act(make-next-list-entry, i2)
	// 	...;
	
	// make a concept holding an item in a translation map
	// result = (make-map-item)(source, target)
	static ref map("map");

	// make a contextual subcontext for an action
	// the maps are lists of translation map items
	// result = (make-context-action)(needed-information-map, made-information-map, action)
	static ref action("action");

	// condition do one of multiple possible actions
	// looks up actions[condition] and does that.
	// does actions[anything] if nothing matches.
	// result = (condition-action)(condition, actions)
	static ref condition("condition");

	// action handlers
	// calls anything with a 'habit' handler: act(action, context)
	// 	for action is habit: provides context as full context
	// 	for action has habit: calls habit with unordered (context, action)
	// 		recursively handles if habit itself has habit to handle it.
	// the habit of habits, calls a habit: habit(context, action)
	// the habit of context-actions, calls a subcontextual action: (context-action)(context, action)
	// the habit of action lists, calls a list of actions: (action-list)(context, action)
	// the habit of 'nothing', does nothing: (nothing)()
}

}
}
