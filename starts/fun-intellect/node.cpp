#include "node.hpp"

ref node::ref()
{
	return (class ref)(this);
}

ref::ref(node * n)
: p(n)
{ }

ref ref::get()
{
	return p->get();
}

ref ref::get(ref way)
{
	return p->get(way);
}

ref ref::link(ref way, ref what)
{
	return p->link(way, what);
}

void ref::unlink(ref link)
{
	p->unlink(link);
}
