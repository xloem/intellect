// given there is a lot of walking, it could be good to clean up
// the connection lists somehow in advance.  otherwise makes strong
// goal pressure at start.

	// well, do lists go away when no longer referenced?
	// let's run a test and see.

// plan is to make a member function now
reference make_list_of_connections_of_kind(reference kind)
{
	return filtered_kind_equal(kind).make_list_of_connections();
}

reference make_list_of_connection_kinds()
{
	auto result = make_list();
	reference last_key;
	for (auto item : *this)
	{
		if (item->kind != last_key) {
			result.list_push_back(item->kind);
			last_key = item->kind;
		}
	}
	return result;
}

reference make_list_of_connections()
{
	reference result = make_list()
	for (auto connection : *this) {
		result.list_push_back(connection);
	}
	return result;
}

// TO AVOID VM: propose using maps to copy, and behavior being copying-maps.

	// what does copy function look like?
	// yeah.
	// obviously takes two maps and copies from one to the other.
	// we can then put a copy-operation in map-waynesses.
	// want a quick way of writing maps, to program them.
	
	// would adding incoming connections harm this?
	// 	you'd want to represent incoming/outgoing in kinds.
	// 	it goes well with every connection being paired with
	// 	an opposite.  opposite would be optional if not found.
	// 		
	
	// so, the brief-maps request expanded to opposite-boilerplate,
	// which makes memory-management harder sometimes.  you can reference
	// an opposite connection
	// 	this sounds fixable via generalization.  maybe maps should
	// 	be first-order concept.
	
	// considering doing first-class maps and second-class sets
	// kind of expect opposites to become inherent somehow
	
	// it merged with use-static-references-in-constructor
	// concept becomes very simplified
		// space for sets?
			// they can be a use of a part that has no links
		// set of links?
			// is opening virtual methods.  current structure
			// optimized for maps, not sets.
	// okay we added wayness.
	// no virtual functions, no setness, no mapness.
	// maps are made easy by using generalized wayness
	// everything is data and possibly a way to do stuff on data
	// all compile-time methods are run-time properties
	// inhibited now, though.

constant_reference(way_kind);
constant_reference(way_source);
constant_reference(way_destination);
constant_reference(way_connections);
constant_reference(way_connection_kinds);
constant_reference(way_connections_kind); // look for something near to be kind
reference get_part_in_way(reference way)
{
	if (way == way_kind()) {
		return kind;
	} else if (way == way_source()) {
		return source;
	} else if (way == way_destination()) {
		return destination;
	} else {
		return {};
	}
}
reference set_destination(reference destination)
{
	this->destination = destination;
}
reference get_connections()
{
	return make_list_of_connections();
}
// roughly, improve V ^

// so basically we make a VM now.
// its 'registers' are the active memory.
	// copy ... needs maps ....  maps seem way to go
		// map is list of ways to get to reference
		// atm just says kind-toward-destination.  should also say source, destination, connection-itself.
	// a connection-itself map would say ... kind, kind_destination
	// instead of just kind.
	// runs into issue: cannot handle multiple kinds.  treated in parallel.
	// can do kind, kind_only, kind_first, kind_next
	// wayness on kinds in maps would do it.
constant_reference(kind_source);  
constant_reference(kind_destination);

reference find_near_in_way(reference way)
{
	if (way == way_kind()) {
		return kind;
	} else if (way == way_source()) {
		return source;
	} else if (way == way_destination()) {
		return destination;
	} else if (way == way_connections()) {
		return make_list_of_connections();
	} else if (way == way_connection_kinds()) {
		return make_list_of_connection_kinds();
	} else if (way.is(way_connections_kind())) {
		return make_list_of_connections_of_kind(way.kind);
	} else {
		return {};
	}
}
reference change_near_in_way(reference way, reference value)
{
	if (way == way_destination()) {
		destination = value;
	} else if (way == way_connections()) {
		// change all connections to list?
	} else if (way == way_connections_kind()) {
		(*this)[value->kind] = value;
		// PROBLEM: kind of destination must be set to kind of its
		// link.  makes break in connecting to others' links.
	}
}
reference add_near_in_way(reference way, reference value)
{
	// refuse to change source or kind, can upset graph
	if (
}

reference dispatch(reference what)
{
}

// we'll then need a simple meta structure that is made of dispatch calls
