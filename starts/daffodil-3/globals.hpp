#pragma once

#include "touch.hpp"

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
	node SET_VALUE;
	node TYPE;
	
	// it will likely make sense to make type information
	// have its own thing, like in life-style-project
	//virtual class touch_ & touch(reference node, reference way);
	node NODE;
	node WAY;

	/*
	node ALL;
	node ANY;
	node NONE;
	*/

	// we no longer can partially include things with this
	// approach.
	// might be able to work around with a class registration
	// system.
	//class_type<touch_> touch_type;
	touch_::type_t touch_type;

	reference const setup_done;

private:
	friend class node;

	static globals_t storage;
	globals_t();
};
