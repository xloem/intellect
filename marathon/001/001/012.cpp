#include <any>
#include <functional>
#include <map>
#include <memory>

struct concept;

using std::initializer_list;

using ref = std::shared_ptr<concept>;

using behavior = std::function<void(ref)>;
using text = std::string;

ref create(initializer_list<std::pair<ref,ref>> refs = {}, std::any data = {});

ref context = create();
ref next = create();
ref input = create();
ref output = create();
ref nothing = create();

struct concept
{
	std::multimap<ref,ref> refs;
	std::any data;

	void operator()(ref context)
	{
		std::any_cast<behavior>(data)(context);
	}

	ref operator[](ref what)
	{
		auto result = refs.find(what);
		if (result == refs.end()) {
			return nothing;
		} else {
			return result->second;
		}
	}
};

ref create(initializer_list<std::pair<ref,ref>> refs, std::any data)
{
	ref result(new concept());
	result->refs.insert(refs.begin(), refs.end());
	result->data = data;
	return result;
}

ref sequence(initializer_list<behavior> behaviors)
{
	ref first, next;
	for (auto & behavior : behaviors)
	{
		ref last = next;
		next = create({}, behavior);
		if (!last) {
			first = next;
		} else {
			last->refs.insert(std::pair(next, create()));
		}
	}
	return first;
}

ref run = create({}, (behavior)[](ref context){
	ref sequence = (*context)[input];
	ref shared_context = (*context)[::context];
	ref next = sequence;
	do {
		(*next)(shared_context);
		next = (*next)[::next];
	} while (next != nothing);
});

#include <iostream>
int main()
{
	ref steps = sequence({
		[](ref) {
			std::cout << "hello world" << std::endl;
		}
	});
	ref context = create({
		{context, create()},
		{input, steps}
	});
	(*run)(context);
}

