#pragma once

#include "common.hpp"
#include "../level-0/concept.hpp"

namespace intellect {
namespace level2 {

// this class is returned by some of the baseref operators.
// its purpose is to evaluate code when it goes out of
// scope, so as to facilitate syntactic behavior.
struct statementref
{
	statementref(ref r);
	statementref(statementref const &) = delete;
	~statementref();

	operator ref();

	static statementref makebinary(
		ref lhs, ref kind, ref rhs,
		std::function<ref(ref)> expraction = {},
		std::function<void(ref)> stmtaction = {}
	);

private:
	level0::concept * r;
};

}
}
