#pragma once

class node;
using ref = node *;

class node
{
public:
	virtual ref get();
	virtual ref get(ref way);

	virtual ref link(ref way, ref what);
	virtual void unlink(ref link);
};
