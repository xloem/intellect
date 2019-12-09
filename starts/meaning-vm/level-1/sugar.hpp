#pragma once

#include "common.hpp"
#include "ref.hpp"

#include <string>
#include <sstream>

namespace intellect {
namespace level1 {

ref a(ref group);
ref an(ref group);
ref a(ref group, ref name);
ref an(ref group, ref name);

bool isanonymous(ref topic);
ref movetoname(ref anonymous, ref name);

namespace internal {
	template <typename... T>
	void init_ref_names(std::string names, T &... refrefs)
	{
		std::stringstream ss(names);
		ref* refptrs[] = {&static_cast<ref&>(refrefs)...};
		for (std::size_t i = 0; i < sizeof...(refrefs); ++ i) {
			std::string name;
			ss >> name;
			if (name[name.size() - 1] == ',') {
				name = name.substr(0, name.size() - 1);
			}
			refptrs[i]->ptr() = ref(name).ptr();
		}
	}
}

#define decl(r) \
	ref r(#r);

#define decls(...) \
	ref __VA_ARGS__; \
	intellect::level1::internal::init_ref_names(#__VA_ARGS__, __VA_ARGS__)

}
}
