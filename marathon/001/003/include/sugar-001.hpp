#pragma once

#include "habits-001.hpp"
/*

static thread_local std::unordered_set<concept*> to_text_already;

ref basic_to_text = habit({symbols::text},{symbols::what, symbols::detail}, [](ref context){
	ref what = context->get(symbols::what);
	bool contents = context->get(symbols::detail).isthing();
	if (to_text_already.count(&*what) || !what->refs.size()) { contents = false; }
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
	if (contents) {
		to_text_already.insert(&*what);
		for (auto refs : what->refs) {
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
	}
	if (contents) { result += "]"; }
	if (contents) {
		to_text_already.erase(&*what);
	}
	return result;
});

ref seq_to_text = habit({symbols::text},{symbols::what, symbols::detail}, [](ref context){
	ref what = context->get(symbols::what);
	bool contents = context->get(symbols::detail).isthing();
	if (to_text_already.count(&*what) || !what->refs.size()) { contents = false; }
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
	if (contents) {
		to_text_already.insert(&*what);
		for (ref item = what; item.isthing(); item = item->get(symbols::next)) {
			for (
		}
		for (auto refs : what->refs) {
			result += to_text(refs.first, false) + "=" + to_text(refs.second);
		}
	}
	if (contents) { result += "]"; }
	if (contents) {
		to_text_already.erase(&*what);
	}
	return result;
});

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

*/
