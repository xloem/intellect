#pragma once

// thread-local context, categorized by other contexts [can be made infinitely recursive =) nesting ambiguities could arise if doing that]

// [note: we are interested in making a step towards being agnostic to the node [database] backend used]

namespace sugar {

class context : public reference
{
public:
	DECLARE default-category;
	DECLARE outer;
	DECLARE category;

	context(reference category = context::default-category(), reference identity = ()) // calling static function for now, fix later
	: reference(identity), category(category), this-context(identity), last-context(contexts.kind-get(category))
	{
		if (reference::null() != last-context) {
			this-context.kind-set(outer-context(), last-context);
		}
		kind-set(context::category(), category);
		contexts.kind-set(category, this-context);
	}
	~context() nothrow(false)
	{
		reference replaced-context = contexts.kind-get(categry, last-context);
		contexts.kind-set(category, last-context);
		if (*this != this-context) { throw agreement-mistake(); }
		if (replaced-context != this-context) { throw agreement-mistake(); }
		if (replaced-context.kind-get(outer-context()) != last-context) { throw agreement-mistake(); }
	}

	static reference get(reference category = context::default-category())

private:
	static thread_local reference contexts;

	reference category;
	reference this-context;

	reference last-context;
};

}
