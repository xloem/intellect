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
	ref infn = a(habit-information-needed);
	habit.set(information-needed, infn);
	//habit.set(concepts::habit, concepts::habit);
	ref posinf = infn;
	for (auto argname : argnames) {
		ref nextinf = a(habit-information);
		nextinf.set(information, argname);
		posinf.set(next-information, nextinf);
		posinf = nextinf;
		if (!infn.linked(argname)) {
			infn.set(argname, nextinf);
		} else {
			if (!infn.get(argname).isa(habit-information)) {
				throw a(unexpected-concepts::habit-information-concepts::name)
					.link(concepts::name, argname)
					.link(concepts::habit, habit);
			}
		}
	}
	habit.fun(code);
	return habit;
}

void habitassume(ref habit, ref information, ref assumption)
{
	ref infn = habit.get(concepts::information-needed);
	infn.get(information).set(assume, assumption);
}

ref dohabit(ref habit, std::initializer_list<ref> args)
{
	using namespace concepts;
	ref posinf = habit.get(information-needed);
	for (ref const & arg : args) {
		if (!posinf.linked(next-information)) {
			throw an(unexpected-information).link
				(concepts::habit, habit,
				 information-value, arg);
		}
		posinf = posinf[next-information];
		// TODO: subcontexts or call instances
		ref::context().set(posinf[information], arg);
	}
	while (posinf.linked(next-information)) {
		posinf = posinf[next-information];
		if (!posinf.linked(assume)) {
			throw a(information-needed).link
				(concepts::habit, habit,
				 information, posinf);
		}
		ref::context().set(posinf[information], posinf[assume]);
	}
	habit.fun<ref>()(ref::context());
	posinf = habit.get(information-needed);
	while (posinf.linked(next-information)) {
		posinf = posinf[next-information];
		ref::context().unlink(posinf[information]);
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
	ref infn = habit.get(information-needed);
	std::map<ref, ref> provided;
	for (auto pair : pairs) {
		auto second = pair.begin(); ++ second;
		if (!infn.linked(*pair.begin())) {
			throw an(unexpected-information).link
				(concepts::habit, habit,
				 information, *pair.begin(),
				 information-value, *second);
		}
		if (provided.count(*pair.begin())) { throw "multiple instances same name not implemented here"; }
		provided[*pair.begin()] = *second;
	}
	ref nextinf = infn;
	while (nextinf.linked(next-information)) {
		nextinf = nextinf.get(next-information);
		ref inf = nextinf.get(information);
		if (!provided.count(inf)) {
			if (nextinf.get(assume)) {
				ctx.link(inf, nextinf.get(assume));
			} else {
				throw a(information-needed).link
					(concepts::habit, habit,
					 information, inf);
			}
		} else {
			ctx.link(inf, provided[inf]);
		}
	}
	habit.fun<ref>()(ctx);
	nextinf = infn;
	while (nextinf.linked(next-information)) {
		nextinf = nextinf.get(next-information);
		ref inf = nextinf.get(information);
		if (provided.count(inf)) {
			ctx.unlink(inf, provided[inf]);
		} else {
			ctx.unlink(inf, nextinf.get(assume));
		}
	}
	//for (auto pair : pairs) {
	//	auto second = pair.begin(); ++ second;
	//	ctx.unlink(pair.begin(), second);
	//}
	if (ctx.linked(result)) {
		ref ret = ctx.get(result);
		ctx.unlink(result, ret);
		return ret;
	}
	return nothing;
}

}
}
