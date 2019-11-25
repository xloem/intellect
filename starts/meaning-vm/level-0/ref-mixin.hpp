#pragma once

#include <vector>

namespace intellect {
namespace level0 {

template <typename ref, template<typename> typename vref>
struct refmixin {
	using array = std::vector<ref>;

	void link(ref const & type, ref const & target) { ptr()->link(type, target); }
	void unlink(ref const & type, ref const & target) { ptr()->unlink(type, target); }
	void unlink(ref const & type) { ptr()->unlink(type); }

	bool linked(ref const & type) const { return ptr()->linked(type); }
	bool linked(ref const & type, ref const & target) const { return ptr()->linked(type, target); }

	array getAll(ref const & type) const { return conv<array>(ptr()->getAll(type)); }

	ref get(ref const & type) const { return conv<ref>(ptr()->get(type)); }
	void set(ref const & type, ref const & target) { ptr()->set(type, target); }

	template <typename T>
	vref<T> vget(ref const & type) const { return conv<vref<T>>(get(type)); }

private:
	inline concept * ptr() const { return conv<ref*>(this)->ptr; }
	template <typename OUT, typename IN>
	static inline OUT conv(IN r) { return *(OUT*)&r; }
};
}
}
