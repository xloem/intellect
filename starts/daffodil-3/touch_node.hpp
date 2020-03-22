#pragma once

#include "node.hpp"

class touch_node : public node
{
public:
	virtual reference touch(reference way) override;
	virtual ~touch_node() = default; // virtual destructor needed for arbitrary allocation patterns if class can be derived from

	inline touch_node(reference node, reference way) { construct_touch(node, way); }

	reference node;
	reference way;

private:
	virtual void construct_touch(reference node, reference way) final;
};
