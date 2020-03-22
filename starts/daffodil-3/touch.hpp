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

	inline touch_ get_way() { return {globals.WAY, globals.GET}; };
	inline touch_ get_node() { return {globals.NODE, globals.GET}; };

	// new is called here in an inline function.  should only be called in virtual functions.
	// in globals, we will want a virtual function to allocate something.
	inline touch_ set_way(reference value) { return {globals.WAY, globals.touch(value, globals.SET)}; }
	inline touch_ set_node(reference value) { return {globals.NODE, globals.touch(value, globals.SET)}; }

	reference node;
	reference way;

private:
	virtual void construct_touch(reference node, reference way) final;
};

using touch = touch_;
