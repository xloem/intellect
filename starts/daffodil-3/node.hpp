#pragma once

#include <cstddef>
#include "platform.hpp"

// - [ ] do not let reference alter objects with static storage, (see TODO at bottom of this file)
// 			might make sense to add a static storage detector to platform.hpp
// - [ ] link access
// - [ ] storable-and-copyable-bubbles
// - [ ] make node virtual-abstract so that each derived class must implement the important functions

class reference;

// In case self-binary-modification is desired, all functions
// are made virtual or inline, for ease of relocation.  More in platform.hpp .

class node
{
public:
	virtual reference touch(reference way);
	virtual reference _test();

	virtual reference operator() final (reference way);
	virtual operator reference() final;

	virtual ~node() noexcept(false);

	// make this protected to do abstract-virtual approach
	inline node(char const *note = 0) { construct_node(false, note); }
protected:
	void * operator new(std::size_t size);

private:
	virtual void construct_node(bool heap, char const * note) final;

	char const * note;

	friend class reference;
	std::size_t reference_count;
	bool reference_delete; // TODO: this could be true or false before initialization.
				// reference counting should properly only impact values allocated to use it.
				// we also want to refer to values that aren't.
};


define(node, VOID);

define(node, SUCCESS);
define(node, FAILURE);

define(node, YES);
define(node, NO);

define(node, GET);
define(node, SET);
define(node, SET_VALUE);
define(node, TYPE);

define(node, NODE);
define(node, WAY)
