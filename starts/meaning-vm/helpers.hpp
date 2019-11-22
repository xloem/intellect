#pragma once

#include "concept.hpp"

#include <sstream>

template <typename... T>
void __helper_init_ref_names(std::string names, T &... refrefs)
{
	std::stringstream ss(names);
	ref* refptrs[] = {&refrefs...};
	for (std::size_t i = 0; i < sizeof...(refrefs); ++ i) {
		std::string name;
		ss >> name;
		if (name[name.size() - 1] == ',') {
			name = name.substr(0, name.size() - 1);
		}
		refptrs[i]->ptr = ref(name).ptr;
	}
}

#define decl(...) \
	ref __VA_ARGS__; \
	__helper_init_ref_names(#__VA_ARGS__, __VA_ARGS__)
