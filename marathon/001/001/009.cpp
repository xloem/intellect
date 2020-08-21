#include <any>
#include <functional>
#include <map>
#include <memory>

using namespace std;

struct concept;

using reference = shared_ptr<concept>;

using behavior = function<void(reference)>;
using text = string;

struct concept
{
	multimap<reference, reference> references;
	any data;

	concept() {}

	concept(text data)
	: data(data) {}

	concept(behavior data)
	: data(data) {}

	concept(initializer_list<pair<reference,reference>> references)
	: references(references.begin(), references.end()) {}

	void act(reference context)
	{
		any_cast<behavior>(data)(context);
	}
};

reference sequence(initializer_list<behavior> behaviors)
{

}

int main()
{
	reference next(new concept());

	reference sequence(new concept());

	sequence.references.emplace(next, new concept(), 
}
