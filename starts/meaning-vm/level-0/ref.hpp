#pragma once

#include "common.hpp"
#include "ref-mixin.hpp"

#include <string>

namespace intellect {
namespace level0 {

struct ref : public refmixin<ref, vref>
{
	ref(concept *p);
	ref(ref const & other) : ref(other.ptr) { }
	ref & operator=(ref const & other) { self.ptr = other.ptr; return self; }
	operator concept*() const { return ptr; }
	concept* operator->() const { return ptr; }
	concept & deref() { return *ptr; }

	ref & l0() { return self; }
	ref const & l0() const { return self; }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);

private:
	concept * ptr;
};

}
}
