#pragma once

#include "ref.hpp"

char const * dump(ref what) __attribute__((warn_unused_result));
char const * dump(ref what, bool contents) __attribute__((warn_unused_result));

char const * dump(ref what, bool contents)
{
	static thread_local std::string storage;
	static thread_local unsigned long long id = 0;
	static thread_local std::unordered_map<basic_ref, std::string> found;
	std::string result;
	if (found.count(what)) {
		result = found[what];
		contents = false;
	} else if (!(*what).refs.size()) {
		contents = false;
	}
	if (!result.size()) {
		std::any & data = (*what).data;
		++ id;
		if (data.has_value()) {
			if (data.type() == typeid(std::function<void(ref)>)) {
				result = std::to_string(id) + "<habit>";
			} else if (data.type() == typeid(std::string)) {
				result = what.data<std::string>();
			} else {
				result = std::to_string(id) + std::string("<") + data.type().name() + ">";
			}
		} else {
			result = std::to_string(id);
		}
		found[what] = result;
	}
	if (contents) {
		result = "[" + result + ":";
		bool first = true;
		for (auto refs : (*what).refs) {
			if (!first) {
				result += ",";
			} else {
				first = false;
			}
			result += dump(*(ref*)&refs.first, false);
			result += "=";
			result += dump(*(ref*)&refs.second, true);
		}
		result += "]";
	}
	storage = result;
	return storage.c_str();
}

char const * dump(ref what)
{
	return dump(what, true);
}
