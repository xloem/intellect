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
	baseref(std::string const & name) : baseref(getnamed(name)) { }
	baseref(const char *name) : baseref(std::string(name)) { }
	baseref(bool b) : baseref(b ? "true" : "false") { }
	baseref() : baseref("nothing") { }

	bool isa(ref group) const { return level1::isa(self, group); }
	bool isan(ref group) const { return isa(group); }

	std::string const & name() const { return getname(self); }
	operator std::string const &() const { return getname(self); }
	operator char const *() const { return getname(self)->data.c_str(); }

	ref operator-(ref other) { return hyphenate(self.ptr(), other.ptr()); }

	template <typename T>
	void vset(ref const & type, T const & v) { self.set(type, level1::alloc(v)); }

	template <typename... Ref>
	std::function<ref(Ref...)> & fun() { return self.template val<std::function<ref(Ref...)>>(); }
	template <typename... Ref>
	void fun(std::function<ref(Ref...)> const & f) { val(f); }
	template <typename... Ref>
	void fun(std::function<void(Ref...)> const & f) { val(voidtoret(f)); }
	template <typename... Ref>
	void fget(ref const & type) { return self.template vget<std::function<ref(Ref...)>>(type); }
	template <typename... Ref>
	void fset(ref const & type, std::function<ref(Ref...)> f) { self.vset(type, f); }
	template <typename... Ref>
	void fset(ref const & type, std::function<void(Ref...)> f) { fset(type, voidtoret(f)); }

	template <typename... Ref>
	ref operator()(Ref... args) { return self.template fun<Ref...>()(args...); }

	std::string dump(ref skipmarkertype, ref skipmarkertarget);

private:
	template <typename... Refs>
	std::function<ref(Refs...)> voidtoret(std::function<void(Refs...)> f)
	{
		return [f](Refs... args) -> ref
		{
			std::initializer_list<ref const *>({&args...});
			f(args...);
			return concepts::nothing;
		};
	}
};

}
}
