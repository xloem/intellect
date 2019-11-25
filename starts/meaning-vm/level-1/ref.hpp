#pragma once

#include "common.hpp"
#include "../level-0/baseref.hpp"
#include "../level-0/ref.hpp"

namespace intellect {
namespace level1 {

struct ref : public level0::baseref<ref,vref,level0::concept>
{
	using level0::baseref<ref,vref,level0::concept>::baseref;
	ref(level0::concept * p) : baseref(p) { }
	ref(level0::ref const & other) : baseref(other.ptr()) { }
	ref(std::string const & name);
	ref(const char *name) : ref(std::string(name)) { }
	ref(bool b) : ref(b ? "true" : "false") { }
	ref() : ref("nothing") { }

	bool isa(ref group) const;
	bool isan(ref group) const { return isa(group); }

	vref<std::string> name() const;

	level0::ref & l0() { return *reinterpret_cast<level0::ref*>(this); }
	ref & l1() { return self; }
	ref const & l1() const { return self; }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
};

}
}
