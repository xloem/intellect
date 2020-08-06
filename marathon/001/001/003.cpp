#include <any>
#include <memory>
#include <multimap>

using namespace std;

struct node
{
	using reference = shared_ptr<node>;
	multimap<reference, reference> links;
	any data;
};

void start(node::self, node::reference)
{
	
}

int main()
{
}
