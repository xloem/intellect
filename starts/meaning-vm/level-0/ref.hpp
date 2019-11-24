#pragma once

#include "common.hpp"

#include <string>

namespace intellect {
namespace level0 {

struct ref
{
	ref(concept *p);
	operator concept*() const { return ptr; }
	concept* operator->() const { return ptr; }
	bool operator==(ref const & other) const { return self.ptr == other.ptr; }
	bool operator!=(ref const & other) const { return self.ptr != other.ptr; }
	bool operator<(ref const & other) const { return self.ptr < other.ptr; }
	concept & deref() { return *ptr; }

	std::string dump(ref skipmarkertype, ref skipmarkertarget) const;

	concept * const ptr;
};

}
}
