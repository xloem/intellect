#pragma once

#include <memory>

class node;
using ref = std::shared_ptr<node>;

class node
{
public:
	virtual ref get();
	virtual ref get(ref way);

	virtual ref link(ref way, ref what);
	virtual void unlink(ref link);
};
