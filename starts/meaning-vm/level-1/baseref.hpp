#pragma once

#include "common.hpp"
#include "funcs.hpp"

namespace intellect {
namespace level1 {

template <typename ref>
struct baseref : public level0::baseref<ref>
{
	baseref(concept * p) : level0::baseref<ref>(p) { }
	baseref(level0::ref const & other) : baseref(other.ptr()) { }
	baseref(std::string const & name) : baseref(getnamed(name)) { }
	baseref(const char *name) : baseref(std::string(name)) { }
	baseref(bool b) : baseref(b ? "true" : "false") { }
	baseref() : baseref("nothing") { }

	bool isa(ref group) const { return level1::isa(self, group); }
	bool isan(ref group) const { return isa(group); }

	std::string const & name() const { return getname(self)->data; }
	operator std::string const &() const { return getname(self)->data; }
	operator char const *() const { return getname(self)->data.c_str(); }

	ref operator-(ref other) { return hyphenate(self.ptr(), other.ptr()); }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);
};

}
}
