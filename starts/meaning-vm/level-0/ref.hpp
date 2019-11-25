#pragma once

#include "common.hpp"
#include "baseref.hpp"

#include <string>

namespace intellect {
namespace level0 {

struct ref : public baseref<ref, vref, concept>
{
	ref(concept *p) : baseref(p) { }
	ref & operator=(ref const & other) { self.p = other.p; return self; }

	ref & l0() { return self; }
	ref const & l0() const { return self; }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
};

}
}
