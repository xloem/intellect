#include <any>
#include <functional>
#include <map>
#include <memory>

using namespace std;

struct concept;

using ref = shared_ptr<concept>

using behavior = function<void(reference)>;
using text = string;

struct concept
{
	multimap<ref,ref> refs;
	any data;

	concept(initializer_list<pair<ref,ref>> refs = {})
	: refs(refs.begin(), refs.end())
	{ }

	void operator()(ref context)
	{
		any_cast<behavior>(data)(context);
	}
};

namespace concepts {
	ref next(new concept());
}

namespace sugar {
	ref sequence(initializer_list<behavior> behaviors)
	{
		ref first, next;
		for (auto & behavior : behaviors)
		{
			if (!next) {
				first = ref(new concept());
				next = first;
			} else {
				ref last = next;
				next = ref(new concept());
				last->refs.insert(next, new concept());
			}
			next->data = behavior;
		}
		return first;
	}
}

int main()
{
	ref run(new concept());
	run.data = (behavior)[](ref sequence){
		ref first = sequence;
		do {
			
		}
	};
}