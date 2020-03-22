#include "touch.hpp"

touch_ & globals_t::touch(reference node, reference way)
{
	return *new touch_(node, way);
}

reference touch_::touch(reference way)
{
	if (way != globals.VOID && way != node && way != *this) {
		return globals.FAILURE;
	}
	return node.touch(this->way);
}

void touch_::construct_touch(reference node, reference way)
{
	this->node = node;
	this->way = way;
}
