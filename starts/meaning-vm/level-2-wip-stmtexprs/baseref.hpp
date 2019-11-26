#pragma once

#include "common.hpp"
#include "../level-1/ref.hpp"
#include "statementref.hpp"

namespace intellect {
namespace level2 {

template <typename ref>
struct baseref : public level1::baseref<ref>
{
	using level1::baseref<ref>::baseref;
	baseref(level1::ref other) : level1::baseref<ref>(other.ptr()) { }
	operator level1::ref() { return ptr(); }

	statementref operator=(ref other) { return assignop(self, other); }
	statementref operator,(ref other) { return commaop(self, other); }
	ref operator[](ref other) { return subop(self, other); }
}

}
}
