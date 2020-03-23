#pragma once

#include <cstddef>
#include "platform.hpp"

// - [ ] convert touch() into a function pointer
// - [ ] port define() to other platforms to ease reuse
// - [ ] generalize type information [generalize property access, possibly touch usage]
// - [ ] make node virtual-abstract so that each derived class must implement the important functions
// - [ ] link access (is this same as previous line?)
// - [ ] storable-and-copyable-bubbles
// - [ ] do not let reference alter objects with static storage, (see TODO at bottom of this file)
// 			might make sense to add a static storage detector to platform.hpp
// 		-> the change to access things by functions probably makes this much less needed

class reference;

// In case self-binary-modification is desired, all functions
// are made virtual or inline, for ease of relocation.  More in platform.hpp .

class node
{
public:
	define(node, VOID);
	
	define(node, SUCCESS);
	define(node, FAILURE);
	
	define(node, YES);
	define(node, NO);
	
	define(node, GET);
	define(node, SET);

	
	virtual reference _test();

	virtual reference operator()() final;
	virtual reference operator()(reference way) final;
	virtual operator reference() final; // TODO: split this into two functions, one to reuse references, one to reference them as separate

	virtual ~node() noexcept(false);

	// make this protected to do abstract-virtual approach
	strong_inline node(char const *note = 0) { construct_node(false, note); }
protected:
	void * operator new(std::size_t size);

	using behavior = reference(*)(reference self, reference way);
	behavior touch_behavior = 0;

	// we could use static_touch<Type>() to produce function pointer to member.  member should be inline non-virtual class method.

private:
	virtual void construct_node(bool heap, char const * note) final;

	char const * note;

	friend class reference;
	std::size_t reference_count;
	bool reference_delete; // TODO: this could be true or false before initialization.
				// reference counting should properly only impact values allocated to use it.
				// we also want to refer to values that aren't.
};


