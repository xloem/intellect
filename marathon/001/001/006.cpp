#include <any>
#include <map>
#include <memory>

using namespace std;

struct node;
using reference = shared_ptr<node>;

struct node
{
	multimap<reference,reference> nodes;
	any data;

	node(any value)
	: data(value) {}

	node(std::initializer_list<std::pair<reference,reference>> nodes)
	: nodes(nodes)
	{ }
};


