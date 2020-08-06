#pragma once

/*
void verify(ref what)
{
	for (auto refs : what->refs) {
		if (!refs.first || !refs.second) {
			throw refs;
		}
	}
}
*/

//text to_text(ref what, bool contents = true);

text to_text(ref what, bool contents)
{
	static thread_local std::unordered_map<concept*, text> found;
	static unsigned long long id = 0;
	text result;
	if (found.count(&*what)) {
		result = found[&*what];
		contents = false;
	} else if (!what->refs.size()) {
		contents = false;
	}
	if (!result.size()) {
		if (what->_data.has_value()) {
			if (what->_data.type() == typeid(cxxcode)) {
				result = "<cxxcode>";
			} else if (what->_data.type() == typeid(text)) {
				result = what->data<text>();
			} else {
				result = text("<") + what->_data.type().name() + ">";
			}
		} else {
			result = std::to_string(++id);
		}
		found[&*what] = result;
	}
	if (contents) {
		result = "[" + result + ":";
		for (auto refs : what->refs) {
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
		result += "]";
		//found.erase(&*what);
	}
	return result;
}
