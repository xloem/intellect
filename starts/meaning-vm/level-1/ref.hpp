#pragma once

#include "common.hpp"
#include "../level-0/ref.hpp"
#include "../level-0/ref-mixin.hpp"

namespace intellect {
namespace level1 {

struct ref : public level0::refmixin<ref,vref>
{
	ref(level0::ref const & other) : ref0(other) { }
	ref(std::string const & name);
	ref(const char *name) : ref(std::string(name)) { }
	ref(bool b) : ref(b ? "true" : "false") { }
	ref() : ref("nothing") { }

	bool isa(ref group) const;
	bool isan(ref group) const { return isa(group); }

	vref<std::string> name() const;

	level0::ref & l0() { return ref0; }
	ref & l1() { return self; }
	ref const & l1() const { return self; }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
private:

	level0::ref ref0;
};

}
}
