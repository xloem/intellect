
template <typename T>
using list = std::vector<T>; // TODO use far-call class
using reference = std::string; // TODO use far-call class

struct kind
{
	reference identifier;
	void (*use)(kind & _kind, item selection, item context);
	reference way_to_pick; // could this be a kind or an item?
};

struct item
{
	reference kind;
	reference identifier;
	list<kind> needs; // can be subparts or goals but mixing both might be confusing
	void * data;
};

// the nascent idea is to have ways-to-pick and ways-to-check-if-is-good-idea
// and then to select from kinds to build running code: e.g. function calls, variable references
