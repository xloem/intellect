#include <any>
#include <memory>
#include <map>

using namespace std;
struct node;

using reference = shared_ptr<node>;

struct node
{
	multimap<reference,reference> links;
	any data;

	void operator()(reference what) { act(what); }

	virtual void act(reference what) {}
};

#include <iostream>
struct output : public node { virtual void act(reference what) {
	std::cout << std::any_cast<std::string>(what->data) << std::endl;
} } output;

struct text : public node {
	text(std::string what)
	{ data = what;}
};

int main()
{
	reference words(new text("hello"));
	output.act(words);
}
