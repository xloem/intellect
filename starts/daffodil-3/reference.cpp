#include "node.hpp"

reference reference::touch(reference way)
{
	return (*value)(way);
}

reference::~reference() noexcept(false)
{
	if (value == 0) { throw "CORRUPTION: MULTIPLE FREE"; }
	remove();
}

reference & reference::operator=(reference const & other)
{
	remove();
	set(other);
	return *this;
}

bool reference::operator!=(reference const & other) const
{
	return value != other.value;
}

bool reference::operator==(reference const & other) const
{
	return value == other.value;
}

void reference::remove()
{
	if (value != 0) {
		-- value->reference_count;
		if (value->reference_count < 0) {
			throw "CORRUPTION: EXCESSIVE DEREFERENCE";
		}
		if (value->reference_count == 0) {
			if (value->reference_delete) {
				delete value;
			}
			value = 0;
		}
	}
}

void reference::set(reference const & to_what)
{
	if (this != &globals.setup_done && globals.setup_done != globals.YES) {
		// globals_t::storage has unconstructed refcounted nodes
		throw "LOGIC ERROR: reference used statically";
	}
	set(to_what.value);
}

void reference::set(node * to_what)
{
	if (to_what == this || to_what == 0) {
		throw "LOGIC ERROR: REFERENCE TO NOTHING";
	}
	value = to_what;
	++ value->reference_count;
}

// i'd like a way for getting the constructed value from the touch.
// 	left open is a plan for how to get particular values.
// 	we'll want to be able to make nodes for the members of a type.  and maybe not have them all be in the globals object.
// 		you could make an object that includes them, statically, like globals
// 		and maybe later a way to get it.
//
// 			okay, static node creation is fine.  I forgot that.

static node CONSTRUCTED;
	// let's go to the touch class and see if there is a way to get its node and its way.

struct test_node : public node
{
public:
	test_node()
	: constructed(true) { }

	reference touch(reference way) override {
		if (way == globals.GET) {
			return constructed ? globals.YES : globals.NO;
		}
		return *this;
	}

	bool constructed;
};

#include <iostream>
reference reference::_test()
{
	// to return touches with full references we'll want to be able to refer to things on the stack, I suppose.
	// so, it would make sense for them to be shared pointers.  references to the heap.
	test_node n;
	// i.e. reference n = new test_node();
	// and looking at how that might work, it could be nice to have reference be a template or there to a templated reference.
	// ALTERNATIVELY we could step straight into the touching-ways.
	if (n.constructed != true) { return globals.FAILURE; }
	reference r(n);
	if (r(n) != n) { return globals.FAILURE; }
	return globals.SUCCESS;
}
