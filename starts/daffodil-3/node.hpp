#pragma once

#include <cstddef>

// two problems:
// 1. we want link access
// 2. we want allocation to be storable.
//	We want to be able to make bubbles that _don't_ reference each other, and can be loaded / unloaded, copied, etc.
//		For imagination, we propose copy.

class reference;
struct globals_t;

// In case self-binary-modification is desired, all functions
// are made virtual, for ease of relocation.

class node
{
public:
	inline reference operator()(reference way); // defined in node_inlines.hpp
	virtual reference touch(reference way);
	virtual operator reference() final;

	virtual globals_t & _globals() final;
	virtual reference _test();

	virtual ~node() noexcept(false);

	globals_t & globals;

	// make this protected to do abstract-virtual approach
	inline node() : globals(_globals()) { construct_node(false); }
protected:

private:
	virtual void construct_node(bool heap) final;

	friend class globals_t;
	void * operator new(std::size_t size);
	
	friend class reference;
	std::size_t reference_count;
	bool reference_delete;
};

class reference : private node // defined in reference.cpp
{
public:
	virtual reference touch(reference way) final override;

	inline reference(); // defined in node_inlines.hpp
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

struct globals_t // defined in globals.cpp
{
	// import and export with a map of reference-to-reference
	// with one entry for each node
	
	node VOID; // no-meaning-specified

	node SUCCESS;
	node FAILURE;
	
	node YES;
	node NO;

	node GET;
	node SET;

	
	// it will likely make sense to make type information
	// have its own thing, like in life-style-project
	virtual class touch_ & touch(reference node, reference way);
	node NODE;
	node WAY;

	/*
	node ALL;
	node ANY;
	node NONE;
	*/

	reference const setup_done;

private:
	friend class node;

	static globals_t storage;
	globals_t();
};

#include "node_inlines.hpp"
