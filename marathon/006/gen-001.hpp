#pragma once
#include "ref.hpp"

#define self (*this)

namespace sym {
	symbol(gen);
	symbol(setup);
}

class gen; template<>
il<il<ref>> assumes_has<gen> = {
	{sym::is, sym::gen},
	{sym::setup},
	{sym::next}
};

// a generator of values.  has a setup and a next method.
class gen : public ref
{
public:
	// setup's first output is the context that is repeatedly passed to generate
	gen(cxxhabit setup, cxxhabit next)
	: ref({
		{sym::is, sym::gen},
		{sym::setup, setup.as<cxxhabit>()},
		{sym::next, next.as<cxxhabit>()}
	})
	{ }

	// start could return an iterable
	ref start_with_ctx(ref ctx)
	{
		verify_has(assumes_has<gen>);
		cxxhabit setup = self[sym::setup].as<cxxhabit>();
		setup.call_with_ctx(ctx);
		return ctx[setup.output()];
	}
	ref start(il<ref> inputs)
	{
		verify_has(assumes_has<gen>);
		cxxhabit setup = self[sym::setup].as<cxxhabit>();
		return setup(inputs);
	}

	ref next(ref ctx)
	{
		cxxhabit generate = self[sym::next].as<cxxhabit>();
		generate.call_with_ctx(ctx);
		return ctx[generate.output()];
	}
};
