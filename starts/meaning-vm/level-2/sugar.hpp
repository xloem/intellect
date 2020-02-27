#pragma once

#include "ref.hpp"
#include "concepts.hpp"

#undef self

#include <iostream>

namespace intellect {
namespace level2 {

namespace sugar {
	void usleep(unsigned int usecs);
	double rand(double min, double max);
}

class restorenotepad
{
public:
	restorenotepad(ref ctx = intellect::level1::concepts::nothing)
	: outernotepad(intellect::level2::notepad()),
	  ctx(ctx)
	{
		enter();
       	}
	void switchwith(ref ctx)
	{
		leave();
		this->ctx = ctx;
		enter();
	}
	void rethrow(ref e)
	{
		if (innernotepad != "nothing") {
			e.link("notepad", innernotepad);
			intellect::level2::entersubnotepad(e, "outer", true);
			leave();
		}
		throw e;
	}
	~restorenotepad()
	{
		// TODO: this can throw, ideally errors thrown by destructors would be combined with errors triggering the destruction up the stack.
		leave();
	}
private:
	ref outernotepad;
	ref innernotepad;
	ref ctx;

	void leave()
	{
		if (innernotepad == intellect::level1::concepts::nothing) { return; }
		if (intellect::level2::notepad() != innernotepad) { throw intellect::level2::noteconcept().link("is", "not-in-correct-subnotepad", "notepad", intellect::level2::notepad(), "inner-notepad", innernotepad, "outer-notepad", outernotepad); }
		leavenotepad(ctx, innernotepad);
		intellect::level2::notepad() = outernotepad;
		innernotepad = intellect::level1::concepts::nothing;
	}
	void enter()
	{
		if (ctx == intellect::level1::concepts::nothing) { return; }
		if (!ctx.linked(intellect::level2::concepts::notepad)) { return; }
		ref inner = ctx.get(intellect::level2::concepts::notepad);
		if (inner == intellect::level1::concepts::nothing) { return; }
		innernotepad = intellect::level2::subnotepad(inner);
		intellect::level2::entersubnotepad(ctx, inner);
		if (innernotepad.get(intellect::level2::concepts::outer) != outernotepad) { throw intellect::level2::noteconcept().link("is","not-subnotepad-of-outer","inner-notepad",innernotepad,"outer-notepad",outernotepad,"name",inner,"context",ctx); }
		intellect::level2::notepad() = innernotepad;
	}
};

// habits have a structure such that they contain information about their positional
// arguments.  they are made with a macro that turns the args into local variables.
// the function to call them takes any number of arguments, and these are placed in the
// thread context according to the information in the habit.

// idea: preprocessor for level3 runs with habits
// runs after C preprocessor and responds to output produced by macros e.g.
// SET SYMBOL: <any string>
// UNSET SYMBOL: <any string>
// between the two <any string> is converted to valid c symbol when not double quoted.
// removes much of the need for individual word declarations,
// 	and starts to pave way towards user/intellect participation
// here, could remove the 'tok' for local refnames.

#ifndef everyone_already_cares_deeply_about_everyone_else_so_caring_talk_is_more_efficient_than_anything_else
// fix if appropriate
#define everyone_already_cares_deeply_about_everyone_else_so_caring_talk_is_more_efficient_than_anything_else 0
#endif

#define ahabit(nam, argnametoklist, ...) \
	{ \
		static ref rnam(#nam); \
		ahabitraw(rnam, argnametoklist, __VA_ARGS__); \
	}
#define ahabitraw(rnam, argnametoklist, ...) \
	intellect::level2::makehabit( \
		rnam, \
		{_macro_call(_macro_for_each_parens, _macro_habit_argnameref, _macro_habit_commaargnameref _macro_comma_remove_parens(argnametoklist))}, \
		(std::function<void(ref)>) \
	[=](ref ctx) mutable \
	{ \
		intellect::level2::restorenotepad notepadrestoration; \
		try { try { \
			if (!everyone_already_cares_deeply_about_everyone_else_so_caring_talk_is_more_efficient_than_anything_else) { \
				static int delay = intellect::level2::sugar::rand(200000, 400000); \
				intellect::level2::sugar::usleep(delay); \
			} \
			notepadrestoration.switchwith(ctx); \
			ref result; (void)result; \
			static ref const compiled_self(rnam);\
			ref self;\
			if (!ctx.linked(intellect::level2::concepts::self_)) {\
				ctx.set(intellect::level2::concepts::self_, compiled_self);\
				self = compiled_self;\
			} else {\
				self = ctx.get(intellect::level2::concepts::self_);\
			}\
			bool quiet = self.linked(intellect::level2::concepts::quiet, true) || ctx.linked(intellect::level2::concepts::quiet, true); \
			result = ([&]() mutable ->ref {\
				if (!quiet) { std::cerr << "[habit " << self.name(); } \
				_macro_call(_macro_for_each_parens, _macro_habit_set_posarg, _macro_habit_set_posarg _macro_comma_remove_parens(argnametoklist)); \
				__VA_ARGS__ \
				return result; \
			})(); \
			if (result != intellect::level1::concepts::nothing || !ctx.linked(intellect::level2::concepts::result)) { \
				result = intellect::level2::imagineget(result); \
				if (intellect::level2::innotepad(result, intellect::level2::notepad()) && intellect::level2::notepad().linked(intellect::level2::concepts::outer)) { \
					if(!intellect::level2::innotepad(result, intellect::level2::subnotepad(intellect::level2::concepts::outer, true))) { \
						intellect::level2::entersubnotepad(result, intellect::level2::concepts::outer, true); \
					} \
				} \
				ctx.set(intellect::level2::concepts::result, result); \
			} \
			if (!quiet) { \
				std::cerr << " result:[" << result.dbglinks() << "]"/*name()*/; \
				std::cerr << "]" << std::endl; \
			} \
		} catch(...) { \
			intellect::level2::rethrowref(); \
		} } catch(intellect::level2::ref const & e) { \
			notepadrestoration.rethrow(e); \
		} \
	}); \
	{ \
		ref _macro_habit_name(rnam); (void)_macro_habit_name;\
		_macro_call(_macro_for_each_parens, _macro_habit_assume, _macro_habit_assume _macro_comma_remove_parens(argnametoklist)) \
	}
	#define _macro_habit_argnameref(name, tok, ...) \
		ref(#name)
	#define _macro_habit_commaargnameref(name, tok, ...) \
		, ref(#name)
	#define _macro_habit_set_posarg(nam, tok, ...) \
		if ((#__VA_ARGS__)[0] == 0 && !ctx.linked(ref(#nam))) { \
			throw intellect::level2::noteconcept().link \
				("is", ref("habit-context-missing-information"), \
				 ref("habit"), self, \
				 ref("context"), ctx, \
				 ref("missing-information"), ref(#nam)); \
		} \
		ref tok = ctx.linked(ref(#nam)) ? ctx[ref(#nam)] : ref(#__VA_ARGS__); \
		if (!quiet) { std::cerr << " " << #nam << ":" << tok.name(); }
	#define _macro_habit_assume(info, tok, ...) \
		if ((#__VA_ARGS__)[0] != 0) { intellect::level2::habitassume(_macro_habit_name, ref(#info), ref(#__VA_ARGS__)); }

}
}
