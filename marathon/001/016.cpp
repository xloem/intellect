#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template<typename t>
using il = std::initializer_list<t>;

using ref = std::shared_ptr<concept>;
using behavior = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

#define symbol(name) ref name = create({},text(#name))

symbol(nothing);
symbol(context);
symbol(next);
symbol(input);
symbol(output);

struct concept
{
	std::unordered_map<ref,ref> refs;
	std::any data;

	void go(ref context)
	{
		std::any_cast<behavior>(data)(context);
	}

	ref at(ref where)
	{
		auto result = refs.find(where);
		if (result == refs.end()) {
			return nothing;
		} else {
			return result->second;
		}
	}

	void verify()
	{
		for (auto refs : this->refs) {
			if (!refs.first || !refs.second) {
				throw refs;
			}
		}
	}
};

text to_text(ref what, bool contents = true)
{
	static thread_local std::unordered_set<concept*> found;
	if (found.count(&*what) || !what->refs.size()) { contents = false; }
	text result;
       	if (contents) { result += "["; }
	if (what->data.has_value()) {
		if (what->data.type() == typeid(behavior)) {
			result += "<behavior>";
		} else if (what->data.type() == typeid(text)) {
			result += std::any_cast<text>(what->data);
		} else {
			result += text("<") + what->data.type().name() + ">";
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

ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->data = data;
	result->refs.insert(refs.begin(), refs.end());
	result->verify();
	return result;
}

template <typename type>
ref dataseq(il<type> items)
{
	ref first, next;
	for (auto & item : items)
	{
		ref last = next;
		next = create({}, item);
		if (!last) {
			first = next;
		} else {
			last->refs.insert(std::pair(::next, next));
		}
	}
	return first;
}

ref run = create({}, (behavior)[](ref context){
	ref seq = context->at(input);
	ref ctx = context->at(::context);
	ref next = seq;
	do {
		next->go(ctx);
		next = next->at(::next);
	} while (next != nothing);
});

#include <iostream>
int main()
{
	ref steps = dataseq<behavior>({
		[](ref) {
			std::cout << "hello ";
		}, [](ref) {
			std::cout << "world" << std::endl;
		}
	});
	ref context = create({
		{::context, create()},
		{input, steps}
	});
	std::cerr << to_text(context) << std::endl;
	run->go(context);
}
