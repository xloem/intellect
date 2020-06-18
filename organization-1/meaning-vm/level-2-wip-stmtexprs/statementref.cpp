#include "statementref.hpp"

#include "concepts.hpp"
#include "ref.hpp"

using namespace intellect;
using namespace level2;
using namespace concepts;

// so, anonymous-assignment is both a statement and an expression.
// multiple-member-assignment is only an expression: it is an error to evaluate it
// the submember operator will be both a statement and an expression, but doesn't need to be held as a ref
// single-member-assignment is mostly a statement: using it as an expression should be an error.

static statementref makebinary(
	ref lhs, ref kind, ref rhs,
	std::function<ref(ref)> expraction,
	std::function<void(ref)> stmtaction
)
{
	a(statement-expresion, kind-expression);
	ref r = a(kind-expression);
	r.set(left-operand, lhs);
	r.set(right-operand, rhs);
	r.vset(expression-action, expraction);
	r.vset(statement-action, stmtaction);
	return r;
}

statementref::statementref(ref r)
: r(r.ptr())
{
	if (!r.isa(statement-expression)) {
		throw std::logic_error("that is not a statement expression");
	}
}

static void de(ref & r)
{
	ref lhs, rhs, expraction, stmtaction;

	try { lhs = r.get(left-operand); } catch(level0::no_such_link_type&) {}
	try { rhs = r.get(right-operand); } catch(level0::no_such_link_type&) {}

	expraction = r.get(expression-action);
	stmtaction = r.get(statement-action);
	dealloc(r);
	if (lhs != nothing) { dealloc(lhs); }
	if (rhs != nothing) { dealloc(rhs); }
	dealloc(expraction);
	dealloc(stmtaction);
	r = 0;
}

statementref::~statementref()
{
	if (r == 0) { return; }
	r.vget<std::function<void(ref)>>(statement-action)(r);
	de(r);
}

statementref::operator ref()
{
	if (r == 0) { throw std::logic_error("doubly evaluated expression"); }
	auto ret = r.vget<std::function<void(ref)>>(expression-action)(r);
	de(r);
	return ret;
}
