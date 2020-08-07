#pragma once

#include "ref.hpp"

class seq;

template <typename t>
char const * dump(t what) __attribute__((warn_unused_result));

char const * dump(ref what, bool contents) __attribute__((warn_unused_result));
char const * dump(seq what, bool contents) __attribute__((warn_unused_result));

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

// guide says off-trail, but our people will learn to clean up after you
// 	woah. uhhh somehow our coding order has resulted in no need
// 	for cpp files, all code in header files.  this depends on seq.hpp
// 	which depends on exception.hpp which depends on this, so seq isn't
// 	defined yet when it's reached.  could move it.
// 		// the right-thing here is to check sym::is instead
// 		// of using an overload, so the code can function in
// 		// multiple projects
/*
char const * dump(seq what, bool contents)
{
	static thread_local std::string storage;
	storage = dump(what.as<ref>(), false);

	if (!contents) { return storage.c_str(); }

	std::string result = "[" + storage + ":";

	bool first = true;
	// might need to change this to reference parts
	// if implementation doesn't have iterator.
	// previous solution was just to make subclass of ref have operator++
	// and operator*, return from begin() and end()
	for (auto item : what)
	{
		if (first) {
			first = false;
		} else {
			result += ",";
		}
		result += dump(item);
	}

	storage = result + "]";
	return storage.c_str();
}
*/

template <typename t>
char const * dump(t what)
{
	return dump(what, true);
}
