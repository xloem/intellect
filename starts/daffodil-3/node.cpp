#include "node.hpp"
#include "daffodil.hpp"

reference node::touch(reference way)
{
	throw "LOGIC_ERROR: TOUCHED ABSTRACT NODE";
}

node::operator reference()
{
	return reference(this);
}

globals_t & node::_globals()
{
	return globals_t::storage;
}

void node::construct_node(bool heap)
{
	reference_count = 0;
	reference_delete = heap;
}
node::~node() noexcept(false)
{
	if (reference_count != 0) {
		throw "CORRUPTION: NODE REFERENCED AFTER DESTRUCTION";
	}
}

#include <cstdlib>
#include <new>
void* node::operator new(std::size_t size)
{
	void * allocation = std::malloc(size);
	if (allocation == 0) {
		throw std::bad_alloc();
	}
	return allocation;
}


#include <iostream>
reference node::_test()
{
	std::cout << "STUB: node test" << std::endl;
	return globals.SUCCESS;
}
