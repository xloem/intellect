#pragma once

#include <map>
#include <vector>

namespace intellect {
namespace level0 {

template <typename ref, template<typename> typename vref>
struct refmixin {
	using links_t = std::multimap<ref, ref>;
	using array = std::vector<ref>;

	void link(ref const & type, ref const & target) { p()->link(conv<r>(type), conv<r>(target)); }
	void unlink(ref const & type, ref const & target) { p()->unlink(conv<r>(type), conv<r>(target)); }
	void unlink(ref const & type) { p()->unlink(conv<r>(type)); }

	bool linked(ref const & type) const { return p()->linked(conv<r>(type)); }
	bool linked(ref const & type, ref const & target) const { return p()->linked(conv<r>(type), conv<r>(target)); }

	array getAll(ref const & type) const { return conv<array>(p()->getAll(conv<r>(type))); }

	links_t & links() const { return *(links_t*)&(p()->links); }

	ref get(ref const & type) const { return conv<ref>(p()->get(conv<r>(type))); }
	void set(ref const & type, ref const & target) { p()->set(conv<r>(type), conv<r>(target)); }

	template <typename T>
	vref<T> vget(ref const & type) const { return conv<vref<T>>(get(type)); }

	bool operator==(ref const & other) const { return self.p() == other.p(); }
	bool operator!=(ref const & other) const { return self.p() == other.p(); }
	bool operator<(ref const & other) const { return self.p() < other.p(); }

private:
	inline concept * p() const { return *conv<concept**>(this); }
	using r = level0::ref;
	template <typename OUT, typename IN>
	static inline OUT conv(IN r) { return *(OUT*)&r; }
};
}
}
