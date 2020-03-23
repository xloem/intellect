#include <cstdlib>
#include <new>
#include <iostream>
// TODO: remove inline define so system headers can go lower down if desired

#include "node.hpp"
#include "reference.hpp"

reference node::operator()()
{
	return touch_behavior(*this,{});
}

reference node::operator()(reference way)
{
	return touch_behavior(*this,way);
}

node::operator reference()
{
	return reference(this);
}

// TODO: this is nice, if virtual, checks against casts
/*
reference node::test_cast()
{
	return dynamic_cast<node*>(this) != 0 ? SUCCESS() : FAILURE();
}
*/

reference node_touch(reference self, reference way)
{
	throw "LOGIC_ERROR: TOUCHED ABSTRACT NODE";
}
void node::construct_node(bool heap, char const * note)
{
	this->touch_behavior = node_touch;
	this->note = note;
	reference_count = 0;
	reference_delete = heap;
}
node::~node() noexcept(false)
{
	if (reference_count != 0) {
		throw "CORRUPTION: NODE REFERENCED AFTER DESTRUCTION";
	}
}

void* node::operator new(std::size_t size)
{
	void * allocation = std::malloc(size);
	if (allocation == 0) {
		throw std::bad_alloc();
	}
	return allocation;
}


reference node::_test()
{
	std::cout << "STUB: node test" << std::endl;
	return SUCCESS();
}
