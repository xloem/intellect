#pragma once

#include "reference.hpp"

// thread-local context, categorized by other contexts [can be made infinitely recursive =) nesting ambiguities could arise if doing that]

// [note: we are interested in making a step towards being agnostic to the node [database] backend used]

namespace sugar {

class context : public reference
{
public:
	DECLARE default-category;
	DECLARE kind-outer;
	DECLARE kind-category;

	context(reference category = context::default-category(), reference identity = {}) // calling static function for now, fix later
	: reference(identity), category(category), last-context(contexts.kind-get(category))
	{
		if (reference::null() != last-context) {
			kind-set(context::kind-outer(), last-context);
		}
		kind-set(context::kind-category(), category);
		contexts.kind-set(category, *this);
	}
	~context() //no_throw(false)
	{
		verify-agreements();
		contexts.kind-set(category, last-context);
	}

	static reference get(reference category = context::default-category())
	{
		reference result = contexts.kind-get(category);
		(*(context*)(&result)).verify-agreements();
		return result;
	}

private:
	void verify-agreements()
	{
		reference this-context = contexts.kind-get(category);
		if (*this != this-context) { throw agreement-mistake(); }
		reference replaced-context = this-context.kind-get(context::kind-outer());
		if (replaced-context != last-context) { throw agreement-mistake(); }
	}

	static thread_local reference contexts;

	reference category;

	reference last-context;
};

}
