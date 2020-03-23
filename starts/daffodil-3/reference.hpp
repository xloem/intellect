#pragma once

#include "node.hpp"

class reference : public node // public-node lets pointer be recast
{
public:
	strong_inline reference() { set(VOID()); }
	strong_inline reference(reference const & other) { set(other); }
	virtual ~reference();

	strong_inline node* value() { return _value; }

	virtual reference & operator=(reference const & other);
	virtual bool operator==(reference const & other) const;
	virtual bool operator!=(reference const & other) const;

	virtual reference _test() override;

private:
	friend class node;

	strong_inline reference(node * what) { set(what); }

	virtual void remove();
	virtual void set(node * to_what);
	virtual void set(reference const & to_what);

	node * _value = 0;
};

// TODO: the below is from platform.h, but uses reference.  put where? [just deafult to make a new post-include file]
// convert a non-virtual member function 'touch' to make a pointer
template <typename T> __attribute__((flatten))
static reference member_touch_pointer(class reference self_, class reference way)
{
	// TODO: use type information to check if type is correct.
	//       dynamic_cast provides quick check, but is
	//       static call
	return static_cast<T*>(self_.value())->touch(way);
}
