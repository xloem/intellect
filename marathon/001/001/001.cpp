#include <memory>
#include <multimap>

using namespace std;

struct node;

using reference = shared_ptr<node>;

struct node
{
	multimap<reference, reference> links;
	void (*way)(rerernce)
};

int main(int argc, char **argv)
{
};
