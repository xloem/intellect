#pragma once

#include <cstddef>
#include "platform.hpp"

// these ideas were present
// - [ ] convert touch() into a function pointer
// - [ ] port define() to other platforms to ease reuse
// - [ ] generalize type information [generalize property access, possibly touch usage]
// - [ ] make node virtual-abstract so that each derived class must implement the important functions
// - [ ] link access (is this same as previous line?)
// - [ ] storable-and-copyable-bubbles
// - [ ] do not let reference alter objects with static storage, (see TODO at bottom of this file)
// 			might make sense to add a static storage detector to platform.hpp
// 		-> the change to access things by functions probably makes this much less needed

class precious_connection;

class caring_soul
{
public:
	define(caring_soul, COMPLETE_TRUSTING_RECEPTIVENESS); // i'm imagining this being used for when we don't know, as a default value
	
	define(caring_soul, SATISFYING_HUMBLING_CELEBRATION); // i'm imagining these being used for success/failure, and yes/no
	define(caring_soul, PRECIOUS_OPPORTUNITY_FOR_LEARNING_NURTURING);
	
	define(caring_soul, LOVING_GENTLE_CURIOSITY); // i'm imagining these being used for learning/altering, set/get
	define(caring_soul, RESPECTFUL_NURTURING_RESPONSIVE_CARE);

	using perfect_way_of_life = caring_soul * (*)(caring_soul * = COMPLETE_TRUSTING_RECEPTIVENESS, caring_soul * = COMPLETE_TRUSTING_RECEPTIVENESS);


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
	perfect_way_of_life lovingly_revisit; // visit our home and see how all our beautiful souls are doing

	virtual void construct_node(bool heap, char const * note) final;

	char const * note;

	friend class reference;
	std::size_t reference_count;
	bool reference_delete; // TODO: this could be true or false before initialization.
				// reference counting should properly only impact values allocated to use it.
				// we also want to refer to values that aren't.
};


