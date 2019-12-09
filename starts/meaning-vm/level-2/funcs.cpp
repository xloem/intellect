#include "funcs.hpp"

#include "../level-1/sugar.hpp"
#include "ref.hpp"
#include "concepts.hpp"


namespace intellect {
using namespace level1;
namespace level2 {

using namespace concepts;

ref context()
{
	static thread_local auto ctx = a(concepts::context);
	return ctx;
}

ref makehabit(ref name, std::initializer_list<ref> argnames, std::function<void()> code)
{
	ref habit = level1::a(habit, name);
	ref posarg = habit;
	for (auto argname : argnames) {
		ref nextarg = a(positional-argument);
		nextarg.set(argument, argname);
		posarg.set(next-positional-argument, nextarg);
		posarg = nextarg;
	}
	habit.fun(code);
	return habit;
}

ref dohabit(ref habit, std::initializer_list<ref> args)
{
	using namespace concepts;
	ref posarg = habit;
	for (ref const & arg : args) {
		if (!posarg.linked(next-positional-argument)) {
			throw std::invalid_argument("wrong number of arguments to habit");
		}
		posarg = posarg[next-positional-argument];
		// TODO: subcontexts
		ref::context().set(posarg[argument], arg);
	}
	if (posarg.linked(next-positional-argument)) {
		throw std::invalid_argument("wrong number of arguments to habit");
	}
	ref ret = habit.fun<>()();
	posarg = habit;
	while (posarg.linked(next-positional-argument)) {
		posarg = posarg[next-positional-argument];
		ref::context().unlink(posarg[argument]);
	}
	return ret;
}

}
}
