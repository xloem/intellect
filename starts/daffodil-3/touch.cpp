#include "touch.hpp"

/*
touch_ & globals_t::touch(reference node, reference way)
{
	return *new touch_(node, way);
}
*/

reference touch_touch(reference _self, reference way)
{
	touch & self = *(touch*)_self.value();
	if (way != node::VOID() && way != self.node && way != self) {
		return node::FAILURE();
	}
	return self.node(way);
}

void touch_::construct_touch(reference node, reference way)
{
	this->touch_behavior = touch_touch;
	this->node = node;
	this->way = way;
}
