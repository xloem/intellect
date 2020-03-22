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

struct test_node : public node
{
public:
	test_node()
	: constructed(true) { }

	reference touch(reference way) override {
		return *this;
	}

	bool constructed;
};

#include <iostream>
reference reference::_test()
{
	test_node n;
	if (n.constructed != true) { return globals.FAILURE; }
	reference r(n);
	if (r(n) != n) { return globals.FAILURE; }
	return globals.SUCCESS;
}
