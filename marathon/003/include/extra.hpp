
void verify(ref what)
{
	for (auto refs : what->refs) {
		if (!refs.first || !refs.second) {
			throw refs;
		}
	}
}

text to_text(ref what, bool contents = true)
{
	static thread_local std::unordered_set<concept*> found;
	if (found.count(&*what) || !what->refs.size()) { contents = false; }
	text result;
       	if (contents) { result += "["; }
	if (what->_data.has_value()) {
		if (what->_data.type() == typeid(cxxcode)) {
			result += "<cxxcode>";
		} else if (what->_data.type() == typeid(text)) {
			result += what->data<text>();
		} else {
			result += text("<") + what->_data.type().name() + ">";
		}
		if (contents) { result += ":"; }
	}
	ref next = symbols::nothing;
	if (contents) {
		found.insert(&*what);
		for (auto refs : what->refs) {
			if (refs.first == symbols::next) {
				next = refs.second;
				continue;
			}
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
	}
	if (contents) { result += "]"; }
	if (next->isthing()) {
		result += ",";
		result += to_text(next, contents);
	}
	if (contents) {
		found.erase(&*what);
	}
	return result;
}
