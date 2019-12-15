#include "funcs.hpp"

#include "../level-1/sugar.hpp"
#include "ref.hpp"
#include "concepts.hpp"


namespace intellect {
using namespace level1;
namespace level2 {

using namespace concepts;

ref & context()
{
	static thread_local auto ctx = a(concepts::context);
	return ctx;
}

ref makehabit(ref name, std::initializer_list<ref> argnames, std::function<void(ref)> code)
{
	ref habit = level1::a(concepts::habit, name);
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
		// TODO: subcontexts or call instances
		ref::context().set(posarg[argument], arg);
	}
	if (posarg.linked(next-positional-argument)) {
		throw std::invalid_argument("wrong number of arguments to habit");
	}
	habit.fun<ref>()(ref::context());
	posarg = habit;
	while (posarg.linked(next-positional-argument)) {
		posarg = posarg[next-positional-argument];
		ref::context().unlink(posarg[argument]);
	}
	if (ref::context().linked(result)) {
		ref ret = ref::context().get(result);
		ref::context().unlink(result, ret);
		return ret;
	}
	return nothing;
}

ref dohabit(ref habit, std::initializer_list<std::initializer_list<ref>> pairs)
{
	using namespace concepts;
	// TODO: subcontexts or call instances
	ref ctx = ref::context();
	for (auto pair : pairs) {
		auto second = pair.begin(); ++ second;
		ctx.link(pair.begin(), second);
	}
	habit.fun<ref>()(ctx);
	for (auto pair : pairs) {
		auto second = pair.begin(); ++ second;
		ctx.unlink(pair.begin(), second);
	}
	if (ctx.linked(result)) {
		ref ret = ctx.get(result);
		ctx.unlink(result, ret);
		return ret;
	}
	return nothing;
}

}
}
