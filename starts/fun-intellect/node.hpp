#pragma once

#include <memory>

class node;
class ref;

class node
{
public:
	virtual class ref ref() final;
	virtual class ref get();
	virtual class ref get(class ref way);

	virtual class ref link(class ref way, class ref what);
	virtual void unlink(class ref link);

protected:
	node() = default;
};

class ref : public node
{
public:
	virtual ref get() final;
	virtual ref get(ref way) final;
	virtual ref link(ref way, ref what) final;
	virtual void unlink(ref link) final;

private:
	friend class node;
	ref(node * n);

	std::shared_ptr<node> p;
};
