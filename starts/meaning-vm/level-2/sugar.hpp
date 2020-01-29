#pragma once

#include "ref.hpp"

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
	restorenotepad(ref ctx = "nothing")
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
		if (innernotepad == "nothing") { return; }
		if (intellect::level2::notepad() != innernotepad) { throw intellect::level2::noteconcept().link("is", "not-in-correct-subnotepad", "notepad", intellect::level2::notepad, "inner-notepad", innernotepad, "outer-notepad", outernotepad); }
		leavenotepad(ctx, innernotepad);
		intellect::level2::notepad() = outernotepad;
		innernotepad = "nothing";
	}
	void enter()
	{
		if (ctx == "nothing") { return; }
		if (!ctx.linked("notepad")) { return; }
		ref inner = ctx.get("notepad");
		if (inner == "nothing") { return; }
		innernotepad = intellect::level2::subnotepad(inner);
		intellect::level2::entersubnotepad(ctx, inner);
		if (innernotepad.get("outer") != outernotepad) { throw intellect::level2::noteconcept().link("is","not-subnotepad-of-outer","inner-notepad",innernotepad,"outer-notepad",outernotepad,"name",inner,"context",ctx); }
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
	intellect::level2::makehabit( \
		ref(#nam), \
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
			static ref const self(#nam);\
			ctx.set(intellect::level2::concepts::self_, self);\
			bool quiet = self.linked(intellect::level2::concepts::quiet); \
			result = ([&]() mutable ->ref {\
				if (!quiet) { std::cerr << "[habit " << self.name(); } \
				_macro_call(_macro_for_each_parens, _macro_habit_set_posarg, _macro_habit_set_posarg _macro_comma_remove_parens(argnametoklist)); \
				__VA_ARGS__ \
				return result; \
			})(); \
			if (intellect::level2::innotepad(result, intellect::level2::notepad()) && !intellect::level2::innotepad(result, intellect::level2::subnotepad("outer", true))) { \
				intellect::level2::entersubnotepad(result, "outer", true); \
			} \
			ctx.link(intellect::level2::concepts::result, result); \
			if (!quiet) { \
				std::cerr << " result:" << result.name(); \
				std::cerr << "]" << std::endl; \
			} \
		} catch(...) { \
			intellect::level2::rethrowref(); \
		} } catch(decltype(intellect::level2::notepad()) const & e) { \
			notepadrestoration.rethrow(e); \
		} \
	}); \
	{ \
		ref _macro_habit_name(#nam); \
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
