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

	concept(any data)
	: data(data) {}

	concept(initializer_list<pair<reference,reference>> references)
	: references(references.begin(), references.end()) {}

	void act(reference context)
	{
		any_cast<behavior>(data)(context);
	}
};

#include <iostream>
int main()
{
	reference words(new concept(text("hello")));
	reference output(new concept((behavior)[](reference what){
		cout << any_cast<text>(what->data) << endl;
	}));
	output->act(words);
}
