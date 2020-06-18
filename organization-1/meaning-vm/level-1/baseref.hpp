#pragma once

#include "common.hpp"
#include "funcs.hpp"

#include "../level-0/ref.hpp"

#include <functional>

namespace intellect {
namespace level1 {

template <typename ref>
struct baseref : public level0::baseref<ref>
{
	baseref(concept * p) : level0::baseref<ref>(p) { }
	baseref(level0::ref const & other) : baseref(other.ptr()) { }
	baseref(std::string const & name, concept* allocator = nullptr) : baseref(getnamed(name, allocator)) { }
	baseref(const char *name, concept* allocator = nullptr) : baseref(std::string(name), allocator) { }
	baseref(bool b) : baseref(b ? "true" : "false") { }
	baseref() : baseref("nothing") { }
	
	char const * dbglinks(int depth = 0) const { return level1::dbglinks(this->ptr(), depth); }

	bool isa(ref group) const { return level1::isa(self, group); }
	bool isan(ref group) const { return isa(group); }

	std::string name() const { return getname(self); }
	explicit operator char const *() const { return getname(self)->data.c_str(); }

	ref operator-(ref other) const { return hyphenate(self, other); }
	ref operator[](ref subref) const { return self.get(subref); }

	template <typename T>
	void vset(ref const & type, T const & v) { self.set(type, level1::alloc(level0::concepts::allocations(), v)); }

	template <typename... Ref>
	std::function<ref(Ref...)> & fun() { return self.template val<std::function<ref(Ref...)>>(); }
	template <typename... Ref>
	void fun(std::function<ref(Ref...)> const & f) { self.val(f); }
	template <typename... Ref>
	void fun(std::function<void(Ref...)> const & f) { self.val(voidtoret(f)); }
	template <typename... Ref>
	void fget(ref const & type) { return self.template vget<std::function<ref(Ref...)>>(type); }
	template <typename... Ref>
	void fset(ref const & type, std::function<ref(Ref...)> f) { self.vset(type, f); }
	template <typename... Ref>
	void fset(ref const & type, std::function<void(Ref...)> f) { fset(type, voidtoret(f)); }

	template <typename... Ref>
	ref operator()(Ref... args) { return self.template fun<Ref...>()(args...); }

	std::string dump(ref set) { return level1::dump(self, set); };

private:
	template <typename... Refs>
	std::function<ref(Refs...)> voidtoret(std::function<void(Refs...)> f)
	{
		return [f](Refs... args) -> ref
		{
			std::initializer_list<ref const *>({&args...});
			f(args...);
			return "nothing";
		};
	}
};

}
}
