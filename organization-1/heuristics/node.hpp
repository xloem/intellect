#pragma once

class node;
class node_list;

// when doing this we have default contexts
// 	and nodes mirror structures


// a common issue is bootstrap coding contexts
// python might be better

class node_context : public node
{
	virtual node & name() { return get_unique("name"); } // name here refers to the name attribute of this particular class
};

class node_factory
{
public:
	virtual node & new_node() = 0;
	virtual void destroy_node(node &) = 0;

	virtual node & named_node(node & context, library::string name) = 0;

	virtual node_list all_nodes() = 0;
};

class node
{
public:
	virtual void link(node & way, node & what) = 0;
	virtual node & get_unique(node & way) = 0;

	virtual node_list get_all(node & way) = 0;
};

// weird to ask implementation to hold allocation of node_list
class node_list
{
public:
	virtual unsigned long count() = 0;
	virtual node & get(unsigned long index) = 0;
	virtual node & set(unsigned long index, node & value) = 0;

	virtual ~node_list();
};
