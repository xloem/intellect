#pragma once

#include <stdlib.h> // int rand(); void srand(int seed);
#include <time.h> // int time(0); int clock_gettime(CLOCK_REALTIME, struct timespec *tp{tv_sec,tv_nsec})
#include <unistd.h> // usleep(unsigned int usecs)

#undef self

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

#define ahabit(name, argnametoklist, ...) \
	intellect::level2::makehabit( \
		ref(#name), \
		{_macro_call(_macro_for_each_parens, _macro_habit_argnameref, _macro_habit_commaargnameref _macro_comma_remove_parens(argnametoklist))}, \
		(std::function<void(ref)>) \
	[=](ref ctx) \
	{ \
		{ \
			static int delay = (double(rand()) / RAND_MAX * 400000 + 200000); \
			usleep(delay); \
		} \
		ref self = ref(#name); (void)self; \
		ref result("nothing"); (void)result; \
		_macro_call(_macro_for_each_parens, _macro_habit_set_posarg, _macro_habit_set_posarg _macro_comma_remove_parens(argnametoklist)); \
		__VA_ARGS__ \
		if (result != ref("nothing")) { ctx.link(ref("result"), result); } \
	}); \
	{ \
		ref _macro_habit_name(#name); \
		_macro_call(_macro_for_each_parens, _macro_habit_assume, _macro_habit_assume _macro_comma_remove_parens(argnametoklist)) \
	}
	#define _macro_habit_argnameref(name, tok, ...) \
		ref(#name)
	#define _macro_habit_commaargnameref(name, tok, ...) \
		, ref(#name)
	#define _macro_habit_set_posarg(name, tok, ...) \
		if ((#__VA_ARGS__)[0] == 0 && !ctx.linked(ref(#name))) { \
			throw an(ref("habit-context-missing-information")).link \
				(ref("habit"), self, \
				 ref("context"), ctx, \
				 ref("missing-information"), ref(#name)); \
		} \
		ref tok = ctx.linked(ref(#name)) ? ctx[ref(#name)] : ref(#__VA_ARGS__);
	#define _macro_habit_assume(info, tok, ...) \
		if ((#__VA_ARGS__)[0] != 0) { intellect::level2::habitassume(_macro_habit_name, ref(#info), ref(#__VA_ARGS__)); }
	
	// seed random number generator statically, for habit delay
	namespace _macro_habit {
	static struct timespec tp;
	static int timeres = clock_gettime(CLOCK_REALTIME, &tp);
	static int seed = (srand(tp.tv_nsec), tp.tv_nsec);
	}
