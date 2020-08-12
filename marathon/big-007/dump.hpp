#pragma once

#include "ref.hpp"

class seq;

char const * dump(ref what) __attribute__((warn_unused_result));

char const * dump_ex(ref what, bool contents, char const * indentation = "", bool reset = false) __attribute__((warn_unused_result));
// see below, no specialization yet
//char const * dump(seq what, bool contents) __attribute__((warn_unused_result));

using cxxfunction = cxxhabit::cxxfunction;

char const * dump_ex(ref what, bool contents, char const * indentation, bool reset)
{
	static thread_local std::string storage;
	static thread_local unsigned long long id = 0;
	static thread_local std::unordered_map<basic_ref, std::string> found;
	if (reset) { found.clear();}
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
			if (data.type() == typeid(cxxfunction)) {
				result = std::to_string(id) + "<habit>";
			} else if (data.type() == typeid(std::string)) {
				result = what.data<std::string>();
				static std::string ctrlchars[0x20] = { "\\0","\\x01","\\x02","\\x03","\\x04","\\x05","\\x06","\\a","\\b","\\t","\\n","\\v","\\f","\\r","\\x0e","\\x0f","\\x10","\\x11","\\x12","\\x13","\\x14","\\x15","\\x16","\\x17","\\x18","\\x19","\\x1a","\\x1b","\\x1c","\\x1d","\\x1e","\\x1f"};
				if (result.size() > 0 && result[result.size()-1] < 0x20) {
					std::string instead = ctrlchars[(unsigned)result[result.size()-1]];
					result.resize(result.size() - 1);
					result = "\"" + result + instead + "\"";
				}
			} else {
				result = std::to_string(id) + std::string("<") + data.type().name() + ">";
			}
		} else {
			result = std::to_string(id);
		}
		if (what.get(sym::variable)) {
			result += "<var>";
		}
		found[what] = result;
	}
	if (contents) {
		result = "[" + result + ":\n";
		bool first = true;
		for (auto refs : (*what).refs) {
			if (!first) {
				//result += ",";
			} else {
				first = false;
			}
			result += indentation;
			result += dump_ex(*(ref*)&refs.first, false, "");
			result += "=";
			std::string subindentation = indentation + std::string("  ");
			result += dump_ex(*(ref*)&refs.second, true, subindentation.c_str());
			result += "\n";
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

char const * dump(ref what)
{
	return dump_ex(what, true, "");
}
