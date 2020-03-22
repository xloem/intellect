#pragma once
#include "node.hpp"

// CRUFT WIP

class type : public node
{
public:

protected:
	virtual void add_property(reference property) final;
	virtual void add_way(reference way) final;

protected:
	std::unordered_set<reference> properties;
	std::unordered_set<reference> ways;
};

class class_type<typename T> : /*public type,*/ public T::globals_t
{
public:
protected:
	/*
	virtual void construct()
	{
		for (reference property : T::properties)
		{
			add_property(property);
		}
		// way pairs a reference with a member function
		for (reference way : T::ways)
		{
			add_way(way);
		}
	}
	*/

private:
	//using behavior = reference T::*(reference);
};

class typed_node<type& _type> : public node
{
public:
	// provide getters and setters for the properties
}
