#include <any>
#include <map>
#include <memory>

using namespace std;

struct node;
using reference = shared_ptr<node>:

struct node
{
	multimap<reference,reference> nodes;
	any data;

	virtual void go(reference) {}
};


