#pragma once
#include "ref.hpp"
#include "habit.hpp"
#include "seq.hpp"

#define self (*this)

namespace sym {
	symbol(gen);
	symbol(gen_use);
	symbol(setup);
}

class gen; template<>
il<il<ref>> assumes_has<gen> = {
	{sym::is, sym::gen},
	{sym::setup},
	{sym::next}
};

// note: generators would be simpler if the outputs of setup were copied to the
//       input context of next by name.  many setups might not even need a name
// note: it would be nice if next could re-call setup.  could use a special symbol
//       to reference the gen itself in the context (like symbol::gen).  might expand
//       to simplifcation of state data, unsure.

// a generator of values.  has a setup and a next method.
class gen : public ref
{
public:
	// setup's first output is the context that is repeatedly passed to generate
	// MAYBE TODO: setup's outputs are copied into the context passed to next
	gen(cxxhabit setup, cxxhabit next)
	: ref({
		{sym::is, sym::gen},
		{sym::setup, setup.as<cxxhabit>()},
		{sym::next, next.as<cxxhabit>()}
	})
	{ }

	// start could return an iterable?
	ref start_with_ctx(ref ctx)
	{
		verify_has(assumes_has<gen>);
		cxxhabit setup = self[sym::setup].as<cxxhabit>();
		ctx = ctx.clone();
		setup.call_with_ctx(ctx);
		ref result;
		for (auto output : setup[sym::outputs].as<seq>()) {
			while (ref item = ctx.take(output)) {
				result.add(output, item);
			}
		}
		return result;
	}
	ref start(il<ref> inputs)
	{
		verify_has(assumes_has<gen>);
		cxxhabit setup = self[sym::setup].as<cxxhabit>();
		ref ctx = setup.call_making_ctx(inputs);
		ref result;
		for (auto output : setup.get(sym::outputs).as<seq>()) {
			while (ref item = ctx.take(output)) {
				result.add(output, item);
			}
		}
		return result;
	}

	ref next(ref ctx)
	{
		cxxhabit generate = self[sym::next].as<cxxhabit>();
		generate.call_with_ctx(ctx);
		return ctx[generate.output()];
	}
};

class gen_use; template<>
il<il<ref>> assumes_has<gen_use> = {
	{sym::is, sym::gen_use},
	{sym::state},
	{sym::gen}
};

class gen_use : public ref
{
public:
	gen_use(gen generator, ref ctx)
	: ref({
		{sym::is, sym::gen_use},
		{sym::context, ctx},
		{sym::gen, generator},
		{sym::state, generator.start_with_ctx(ctx)}
	})
	{ }

	gen_use(gen generator, il<ref> inputs)
	: gen_use(generator, generator[sym::setup].as<cxxhabit>().ctx_from_inputs(inputs))
	{ }

	gen_use make_new() const
	{
		return {self[sym::gen].as<gen>(), self[sym::context]};
	}

	void reset()
	{
		gen generator = self[sym::gen].as<gen>();
		ref state = generator.start_with_ctx(self[sym::context]);
		self <= r{sym::state, state};
	}

	ref next()
	{
		gen generator = self[sym::gen].as<gen>();
		return generator.next(self[sym::state]);
	}
};
