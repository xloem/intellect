
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
			result += "<habit>";
		} else if (what->_data.type() == typeid(text)) {
			result += what->data<text>();
		} else {
			result += text("<") + what->_data.type().name() + ">";
		}
		if (contents) { result += ":"; }
	}
	if (contents) {
		found.insert(&*what);
		for (auto refs : what->refs) {
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
		found.erase(&*what);
	}
	if (contents) { result += "]"; }
	return result;
}
