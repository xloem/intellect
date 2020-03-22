#pragma once

#include "node.hpp"

// let's try to use touch to make getters and setters
// so we could make a touch that says NODE->GET, and respond to it with the node.
// that works for getters.  for setters we need to say what they are.
// so, say we want to set the node.
// we have {node:NODE, way:SET} but that's not quite right
// we want more {node:NODE, way: {node:VALUE, way:SET}}
// make inner thing on heap

class touch_ : public node
{
public:
	virtual reference touch(reference way) override;
	virtual ~touch_() = default; // virtual destructor needed for arbitrary allocation patterns if class can be derived from

	inline touch_(reference node, reference way) { construct_touch(node, way); }

	reference node;
	reference way;

	using type_t = struct touch_type_t;

private:
	friend type_t;
	using node::operator new;
	virtual void construct_touch(reference node, reference way) final;
};

using touch = touch_;

struct touch_type_t {
	::node NODE;
	::node WAY;

	/*
	touch get_node;
	touch get_way;
	touch set_node;
	touch set_way;

	globals_t(::globals_t & globals);
	: get_node(globals.NODE, globals.GET),
	  get_way(globals.WAY, globals.GET),
	  set_node(globals.NODE, create(globals.SET_VALUE, globals.SET)),
	  set_node(globals.WAY, create(globals.SET_VALUE, globals.SET))
	{ }
	*/
	virtual touch & create(reference node, reference way) { return *new touch(node, way); }
};
