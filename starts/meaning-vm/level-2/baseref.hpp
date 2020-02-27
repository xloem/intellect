#pragma once

#include "common.hpp"
#include "funcs.hpp"
#include "../level-1/baseref.hpp"

#include <functional>

namespace intellect {
namespace level2 {

template <typename ref>
struct baseref : public/*private*/ level1::baseref<ref>
{
	/*
	// constructor will need to check imagination
	baseref() : baseref("nothing") { }
	template <typename T>
	baseref(T arg) : level1::template baseref<ref>(arg)
	{
		this->ptr() = level2::imagineget(level2::notepad(), *this);
	}*/
	using level1::template baseref<ref>::baseref;
	/*
	baseref(std::string & name, bool create = false) : baseref(getnamed(name, create)) { }
	*/

	// thread-local context
	static ref & context() { return level2::context(); }

	ref imagineget(ref type, bool reset = false) {
		auto orig = this->get(type);
		auto imag = level2::imagineget(intellect::level2::notepad(), orig);
		if (orig != imag && reset) {
			level2::checknotepad(*this);
			this->set(type, imag);
		}
		return imag;
	}

	template <typename... Refs>
	ref operator()(ref first, Refs... rest) const { return level2::dohabit(self, {first, rest...}); }
	ref operator()(std::initializer_list<std::initializer_list<ref>> pairs, bool extra_information = false) const { return level2::dohabit(self, pairs, extra_information); }
	ref operator()() const { return level2::dohabit(self); }

	template <typename... Refs>
	ref act(ref habit, Refs... rest) const { return level2::dohabit(habit, {self, rest...}); }

	void replace(ref other) { *self.ptr() = *other.ptr(); }

	/*
	// TODO: undo this work and move work to steps() habit
	// progress identifying this as more effective path
	// by working on them, is thanked.
	// full review of effectiveness not verified.  so,
	// implement steps() parts before deleting these.
	// the other approach looks much easier.
	void imagine_update()
	{
		// how do we want this to behave?
		//
		// 1. when i am in a notepad, and i imagine a change, i want all instsances of that ref to change to the imagined change.
		// 2. when i am outside of a notepad, and i reference their imagination from another context, i do not want it to change unless i am imagining changes specifically to it.
		// 	this may not matter, but is one way to handle making use of the imagined situation after deciding it is good
		// 3. when i enter a notepad, i do not want to retain the use of imagination-refs after i leave, if i was using them before i entered.
		// 	#3 is unaddressed.
		// 	and could conflict with #2 in some implementations
		// 	it sounds like it is best to move imaginations in
		// 		1. we want refs in a notepad to use its imagination.
		// 		2. we want code running with the notepad active, to use its imagination
		// 		3, we do not want code running with other notepads active, to use its imagination, unless it does so intentionally.
		// 	so, we should put imgination _not_ in the ref class, but rather in code-execution, steps-running.  i think.
		// 		yeah.
		// TODO: if we kept 
		// let's check the current notepad to see if we need to be rereferenced into imagination
		// this only need be done if we are _not_ in it already, but _must_ be if both are true
			// when do we unset the flag?
			// this is kinda confusing.
			// why not just keep a list of references to concepts?
			// then when one is replaced with imagination, we can update it.
		if (!level2::innotepad(*this, level2::notepad()) && notepad().linked(
	}
private:
	*/
};

}
}
