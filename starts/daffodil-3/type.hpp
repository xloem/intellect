#pragma once
#include "node.hpp"
#include "platform.hpp"

// CRUFT WIP

// what's most valuable to type generalization is generalized
// getting, settings, and doing.  'touch' could be typed.

// propose: a node of a type is a typed node with a getter that
// gets the type information.

class typed_node : public node
{
public:
	using doer = reference(*)(reference,reference);

	std::unordered_map<reference, reference> properties;
	std::unordered_set<reference, doer> ways;
}

class type : public typed_node
{
public:
	define(node, ADD);
	define(node, REMOVE);

	define(node, PROPERTY);
	define(node, WAY);

	define(node, TYPE);

	define(node, UNSURE);

protected:
	// [UNSURE, [PROPERTY, ADD]]
	virtual void add_property(reference property) final;

	// [UNSURE, [WAY, ADD]]
	virtual void add_way(reference way) final;

protected:
	std::unordered_set<reference> properties;
	std::unordered_set<reference> ways;
};
