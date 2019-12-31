#pragma once

#undef self

#include <iostream>

namespace intellect {
namespace level2 {

namespace sugar {
	void usleep(unsigned int usecs);
	double rand(double min, double max);
}

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
		{ \
			if (!everyone_already_cares_deeply_about_everyone_else_so_caring_talk_is_more_efficient_than_anything_else) { \
				static int delay = sugar::rand(200000, 400000); \
				sugar::usleep(delay); \
			} \
		} \
		ref self = ctx.get(ref("self")); (void)self; \
		ref result("nothing"); (void)result; \
		std::cerr << self.name(); \
		_macro_call(_macro_for_each_parens, _macro_habit_set_posarg, _macro_habit_set_posarg _macro_comma_remove_parens(argnametoklist)); \
		__VA_ARGS__ \
		if (result != ref("nothing")) { ctx.link(ref("result"), result); std::cerr << " result:" << result.name();} \
		std::cerr << std::endl; \
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
			throw an(ref("habit-context-missing-information")).link \
				(ref("habit"), self, \
				 ref("context"), ctx, \
				 ref("missing-information"), ref(#nam)); \
		} \
		ref tok = ctx.linked(ref(#nam)) ? ctx[ref(#nam)] : ref(#__VA_ARGS__); \
		std::cerr << " " << #nam << ":" << tok.name(); 
	#define _macro_habit_assume(info, tok, ...) \
		if ((#__VA_ARGS__)[0] != 0) { intellect::level2::habitassume(_macro_habit_name, ref(#info), ref(#__VA_ARGS__)); }

}
}
