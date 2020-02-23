#pragma once

#include "common.hpp"
#include "funcs.hpp"
#include "../level-1/baseref.hpp"

#include <functional>

namespace intellect {
namespace level2 {

template <typename ref>
struct baseref : private level1::baseref<ref>
{
	// constructor will need to check imagination
	baseref() : baseref("nothing") { }
	template <typename T>
	baseref(T arg) : level1::template baseref<ref>(arg)
	{

	}
	using level1::template baseref<ref>::baseref;
	/*
	baseref(std::string & name, bool create = false) : baseref(getnamed(name, create)) { }
	*/

	// thread-local context
	static ref & context() { return level2::context(); }

	template <typename... Refs>
	ref operator()(ref first, Refs... rest) const { return level2::dohabit(self, {first, rest...}); }
	ref operator()(std::initializer_list<std::initializer_list<ref>> pairs, bool extra_information = false) const { return level2::dohabit(self, pairs, extra_information); }
	ref operator()() const { return level2::dohabit(self); }

	template <typename... Refs>
	ref act(ref habit, Refs... rest) const { return level2::dohabit(habit, {self, rest...}); }

	void replace(ref other) { *self.ptr() = *other.ptr(); }
};

}
}
