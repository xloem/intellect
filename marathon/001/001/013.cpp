#include <any>
#include <functional>
#include <memory>
#include <unordered_map>

struct concept;

template<typename type>
using il = std::initializer_list<type>;

using ref = std::shared_ptr<concept>;
using behavior = std::function<void(ref)>;
using text = std::string;

ref create(il<std::pair<ref,ref>> refs = {}, std::any data = {});

ref nothing = create();
ref context = create();
ref next = create();
ref input = create();
ref output = create();

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
};

ref create(il<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->refs.insert(refs.begin(), refs.end());
	result->data = data;
	return result;
}

void dump(ref what)
{

}

template <typename type>
ref seq(il<type> behaviors)
{
	ref first, next;
	for (auto & behavior : behaviors)
	{
		ref last = next;
		next = create({}, behavior);
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
	run->go(context);
}
