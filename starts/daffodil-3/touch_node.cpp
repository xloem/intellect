#include "touch_node.hpp"

reference touch_node::touch(reference way)
{
	if (way != globals.VOID && way != node && way != *this) {
		return globals.FAILURE;
	}
	return node.touch(this->way);
}

void touch_node::construct_touch(reference node, reference way)
{
	this->node = node;
	this->way = way;
}
