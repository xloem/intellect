// C++ structures
#include <any>
#include <map>
#include <memory>

using namespace std;

struct concept;

using reference = shared_ptr<concept>;

struct concept
{
	multimap<reference, reference> references;
	any data;

	concept(any data)
	: data(data) {}

	concept(initializer_list<pair<reference,reference>> references)
	: references(references) {}

	void behavior(reference context)
	{
		any_cast<function<void(reference)>(data)(context);
	}
};


