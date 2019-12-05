#include "structure.hpp"

#include "../level-1/helpers.hpp"

using namespace intellect;
using namespace level1;
using namespace level2;
using namespace concepts;

// implement only what is needed now

ref intellect::level2::structure(ref name, std::initializer_list<ref> attributes)
{
	ref ret = a(structure, name);

	ref lastfill = nothing;

	// we use [is-linked]->{link-source link-type link-target}
	// to note the links, because I actually think it might be good to upgrade concepts
	// themselves to store links this way.  that way actual links are fully referencable.
	
	// All that means is making concepts have virtual link access, and checking the type
	// in a subclass that implements the links.  Just make concepts virtual and implement
	// link-fetching with one central virtual function.

	// Later we may want a way of mixing in links to any/all concepts.  A handler that gets
	// called when a specific link type is asked for.
	// But remember if this handler triggers others it may create infinite recursion.
	// 		a quick solution was to return too-complex randomly at depth.

	int varnum = 1;
	for(auto it = attributes.begin(); it != attributes.end();) {
		auto attrtype = *it++;
		// add conditions for special attribute types prior to this block and put it in an else
		{
			// default: attrtype specifies single raw link type
			auto attrtarget = *it++;
			if (attrtarget.isa(fill)) {
				attrtarget = a(attrtarget);
				if (lastfill == nothing) {
					ret.set(first-fill, attrtarget);
				} else {
					lastfill.set(next-fill, attrtarget);
				}
				lastfill = attrtarget;
			}
			ret.link(linked, a(link).link(
				link-source, topic,
				link-type, attrtype,
				link-target, attrtarget
			));
		}
	}
}

ref intellected::level2::structured(ref structure, std::initializer_list<ref> fills)
{
	std::map<ref, ref> values;
	if (structure.linked(first-fill)) {
		ref fill = structure.get(first-fill);
		auto it = fills.begin();
		while (true) {
			if (it == fills.end()) { throw std::invalid_argument("missing structure fills"); }
			values.emplace(fill, *it++);
			if (!fill.linked(next-fill)) { break; }
			fill.ptr() = fill.get(next-fill);
		}
		if (it != fills.end()) {
			throw std::invalid_argument("too many structure fills");
		}
	}

	ref ret = a(structure);
	for (ref l : structure.getAll(linked)) {
		if (l.get(link-source) != topic) { throw std::invalid_argument("TODO nontopic source link"); }
		ref typ = l.get(link-type);
		ref targ = l.get(link-target);
		if (targ.isa(fill)) {
			ret.link(typ, values[targ]);
		} else {
			ret.link(typ, targ);
		}
	}
	return ret;
}

void intellect::level2::muststructured(ref topic, ref structure)
{
	//std::map
	// umm if a link is listed twice, we want to ensure that it is present twice.
	// hard. without. link. objects.
	for (ref l : structure.getAll(linked)) {
		ref src = l.get(link-source);
		if (src == concepts::topic) { src.ptr() = topic; }

		ref typ = l.get(link-type);
		ref targ = l.get(link-target);
		...fixif ((t).isa(fill)) {
			ret.link(link-type, values[t]);
		} else {
			ret.link(link-type, t);
		}
	}
}

// monologues below: DELETEME unless something crucial inside

// structures

// define promises around structure, in terms of pattern matching
// if node A is a link-expression, it should match the structure in link-expression.
// so, link-expression is a structure-promise, maybe
// 		or we could state that link-expression graph-structured <graph-structure>
// 		doesn't really matter.
// link-expression is a group, all members of which have a given graph structure
// intend for all link-expressions to have same structure
// group-member-structure
// 	i'd like to have a bunch of promises or expectations around attributes of the group members
// these are promises, not expectations.  when broken something is up.
// group-member-promise
// 
// groups: concepts targeted by link-type 'is'
// groups can-link-by group-member-promise any number of times
//
// a group-member-promise can state that the member matches a structure pattern
//	promise-kind topic-matches-structure
//	[topic-variable TOPIC]
//	structure <- propose a structure with some items being variables.
//	   variables are what is matched against.
//
// what about things that can be linked as many times as we want?
// what about separate restraints on the structure of the variables?
// what about alternatives?
//
// 		it could also make more sense to discuss link attributes of the topic
// 		has-link, link-type-promise, link-target-promise
// [!!!!!! ARGH!]
// - X is linked way1
// - X may be linked by way2
// - X is not linked way3
// - X is linked by either way1, or way2, but not both
//
// - relevent topic data:
// 	[always, sometimes, never] // handles optionals and exclusions
// 	[is-exactly, is-a] // possibly handles alternatives vs is-a
//	or: AND, OR, and NOT are very normal ways of doing this.
//	
//	what about multipleness?
//	say there can be as many of a link-type as we want
//	linked-by [#-times, any-times]
//		link-type
//
//	topic link-type link-target
//		we will define some special hardcoded link-types to handle further meaning we happen to need.
//		[always, sometimes, never] can somehow be applied to substatement?
//		topic color mauve
//		topic is-a color <- is-a is special, checks parents
//		
//		or(topic color mauve, topic color chartreuse) ? how?
//		topic color (mauve or chartreuse)
//		topic one-of list(topic color mauve, topic color chartreuse)
//		topic nothing-but-and-groups
//		these are roughly expressions
//		seems like it would kind of work
//
//		let's define general structure norms.
//		it seems it would be nice if a structure were in charge of particular link-types that
//		are used with its members only in the ways it dictates
//		so no apple->green->link-type, just confuses everyone
//
//		we'll also want a way to instantiate structures quick -> means variables provided
//		on outer unknown topic things.
//
//
//	here we move around graph reviewal.
//		for pattern-matching later, we care about the structure of the whole universe, not just
//		the topic.  we may want to make sure that at least 3 people know the topic, which could
//		be expressed by saying linked-by 3-times a-person,know,topic.
//		without pattern searching, this would require reviewing entire database each request.
//		let's stay topic focused and let a habit expand that to be topic focused if needed
//			hum then we would need to say that all the 3 people were different
//			which could be done by linking the group of them to a parts-all-different tag
//			which would mean referencing the whole result with a variable.
//				i want to exlore this, but i suppose it is for later.
//				variable rereference
//		complex promises need to rereference their parts
//			the most basic part is saying a topic has a prticular target to a prticular linktype
//			this type or target could also use a variable, and the variable used as a further topic
//			we will want some special things, since we don't expand relevent habits yet.
//
//	then we care often about link-types
//	often we have some set link-types
//	propose alternative link-types are grouped under a link group
//	so we could say precisely-is vs is-in-group
//
// - X may only be linked these ways? <- don't like this, auxiliary data is always helpful
//
// this seems way more general


