#pragma once

#include "node.hpp"

class reference : private node // defined in reference.cpp
{
public:
	virtual reference touch(reference way) final override;

	inline reference() { set(VOID()); }
	inline reference(reference const & other) { set(other); }
	virtual ~reference();

	virtual reference & operator=(reference const & other);
	virtual bool operator==(reference const & other) const;
	virtual bool operator!=(reference const & other) const;

	virtual reference _test() override;

private:
	friend class node;

	inline reference(node * what) { set(what); }

	virtual void remove();
	virtual void set(node * to_what);
	virtual void set(reference const & to_what);

	node * value = 0;
};
