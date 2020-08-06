#include <any>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>

struct concept;

template<typename type>
using il = std::initializer_list<type>;

using ref = std::shared_ptr<concept>;
using behavior = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

ref nothing = create({},text("nothing"));
ref context = create({},text("context"));
ref next = create({},text("next"));
ref input = create({},text("input"));
ref output = create({},text("output"));

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

	text txt(bool contents = true)
	{
		static thread_local std::unordered_set<concept*> found;
		text result;
		if (found.count(this)) { contents = false; }
		if (data.has_value()) {
			if (data.type() == typeid(behavior)) {
				result = "<behavior";
			} else if (data.type() == typeid(text)) {
				result = "<\"" + std::any_cast<text>(data) + "\"";
			} else {
				result = "<data";
			}
			if (refs.size()) { result += ":"; }
		}
		found.insert(this);
		for (auto refs : this->refs) {
			result += "(" + refs.first->txt(false) + "=" + refs.second->txt() + ")";
		}
		found.erase(this);
		result += ">";
		return result;
	}
};

ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->refs.insert(refs.begin(), refs.end());
	result->data = data;
	return result;
}

template <typename type>
ref seq(il<type> items)
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
	ref steps = seq<behavior>({
		[](ref) {
			std::cout << "hello ";
		}, [](ref) {
			std::cout << "world" << std::endl;
		}
	});
	ref context = create({
		{context, create()},
		{input, steps}
	});
	std::cerr << context->txt() << std::endl;
	run->go(context);
}
