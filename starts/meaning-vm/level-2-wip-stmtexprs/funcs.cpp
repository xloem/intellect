#include "funcs.hpp"

using namespace intellect;
using namespace level2;
using namespace concepts;

static ref refassigned(ref expr)
{
	ref lhs = ref.get(left-operand);
	ref rhs = ref.get(right-operand);
	if (lhs.isa(link) && lhs.get(link-target) == unknown) {
		// completes the target of a link, for a[b] = c
		lhs.unlink(link-target, unknown);
		lhs.set(link-target, rhs);
		ref.unlink(right-operand, rhs);
		ref src = lhs.get(link-source);
		if (lhs.get(link-type) != unknown && src != unknown) {
			src.set(lhs.get(link-type), rhs);
			return src;
		} else {
			throw std::logic_error("not sure what to do with incomplete link assignment");
		}
	} else if (isanonymous(rhs) && !isanonymous(lhs)) {
		// assignment of anonymous content to empty named concept
		ref.unlink(left-operand, lhs);
		return level1::movetoname(rhs, lhs);
	} else {
		throw std::logic_error("unexpected bare assignment");
	}
}
// maybe later we can have ref class itself do operators completely based on its own
// ref content.

statementref assignop(ref self, ref other)
{
	return statementref::makebinary(
		self, concepts::assign, other,
		refassigned, refassigned
	);
}
statementref commaop(ref self, ref other)
{
	if (self.isa(comma-expression)) {
		if (other.isa(comma-expression)) {
			for (auto & l : other.links()) { self.insert(l.first, l.second); }
			dealloc(other);
		} else {
			self.link(topic, other);
		}
		return self;
	} else if (other.isa(comma-expression)) {
		other.link(topic, self);
		return other;
	} else {
		return statementcallref::makebinary(
			self, comma, other,
			[](ref)->ref { return ref; },
			// um when we pass the comma-expression to
			// the [] operator that takes a ref
			// the destructor of statementref will deallocate it.
			[](ref) { throw std::logic_error("bare comma-expression"); }
			// something is wrong here.  some approach is wrong.
			// would it be better to have ref itself do it all?
		);
	}
}
ref subop(ref self, ref other)
{
}
