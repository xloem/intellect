#include <memory>
#include <multimap>

using namespace std;

struct node
{
	multimap<shared_ptr<reference>,shared_ptr<reference>> links;
	void (*way)(reference);
	void * pointer;
};
