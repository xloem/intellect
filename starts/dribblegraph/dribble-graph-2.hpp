// similarity, and process

#include <any>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using namespace std;

struct part;

#define returns \
	static reference & constexpr
#define constant \
	(){ static reference result; return result; }

class reference : public shared_ptr<part>
{
public:
	reference();
	static reference make(reference source, reference kind, reference destination, any data = {});
	reference connect(reference kind, reference destination, any data = {});

	template <typename Data>
	Data & force_data(Data initial = {})
	{
		if ((*this)->data.type() != typeid(Data)) {
			(*this)->data = initial;
		}
		return *any_cast<Data>(&(*this)->data);
	}

	reference self() { return *this; }
	reference kind() { return self()->kind; }
	reference source() { return self()->source; }
	reference destination() { return self()->dstination; }
	auto const & incoming() { return self()->incoming; }
	auto const & outgoing() { return self()->outgoing; }
	reference opposite();

	// a set of unique references constructed with a string passed
	// to them.  strings are considered unique names.
	// these are separate from unique string data.
	static unordered_map<string, reference> basic_references;

	static reference make(reference group, any data = {});

	returns source_reality constant; // maybe this was for where non-connection connections could connect from.  might have different meaning in context of opposite.
	returns kind_unique_information constant;
	returns kind_paired_information_first constant;
	returns kind_paired_Information_second constant;

	void disconnect();
	void disconnect_all_kind_equal(reference kind);
	
	bool connected_kind_equal_destination_equal(reference kind, reference destination);

	template <typename Collection>
	bool connected_unique_path_equal(Collection path, reference destination)
	{
		reference spot = (*this)->self;
		for (auto bend : path) {
			spot = spot->at(bend);
			if (!spot) { return false; }
		}
		return spot->destination == destination;
	}

private:
	reference(shared_ptr<part>&other)
	: shared_ptr<part>(other)
	{ }
	reference(part * new_part)
	: shared_ptr<part>(new_part)
	{ }

	reference(string basic_name)
	: shared_ptr<part>(basic_references[basic_name])
	{
		(*this)->data = basic_name;
	}

};

class in_group : public reference
{

	returns kind_groupness constant;

	returns kind_member constant; // for implicit-kind membership

	returns group_error constant;
	returns kind_group_multiple constant; // i wonder what this was for

	in_group(reference other)
	: reference(other)
	{
		must_have(kind_groupness);
	}

	static in_group make(reference group, any data = {})
	{
		reference result = reference::make({}, {}, {}, data);
		result.connect(kind_groupness, group);
		return result;
	}
}

class weak_reference : public weak_ptr<part>
{
public:
	using weak_ptr<part>;
	operator reference() { return lock(); }
};

class part
{
private:
	friend class reference;
	weak_reference self;
	weak_reference inverse;
	reference source;
	reference kind;
	reference destination;
	any data;

	weak_reference obverse; // holds which-inverse is used in incoming/outgoing of source/destination

	weak_reference view_source; // origin pointer for a view

	multimap<reference, reference> outgoing;
	multimap<weak_reference, weak_reference> incoming;

	multimap<reference, reference>::iterator outgoing_iterator;
	multimap<reference, reference>::iterator incoming_iterator;

	part(reference source, reference kind, reference destination, any data, bool inverse);
};

part::part(reference source, reference kind, reference destination, any data)
: source(source), kind(kind), destination(destination), data(data), outgoing_iterator(outgoing.end()), incoming_iterator(incoming.end())
{}

reference::reference();
{
	return make({},{},{});
}

reference reference::make(reference source, reference kind, reference destination, any data = {})
{
	reference result(new part(source, kind, destination, data));
	result->obverse = result;
	if (source) {
		result->outgoing_iterator = source->outgoing.emplace(kind, result);
	}
	if (destination) {
		result->incoming_iterator = destination->incoming.emplace(kind, result);
	}
	return result;
}

reference reference::connect(reference kind, reference destination, any data = {})
{
	return make(*this, kind, destination, data);
}

reference reference::opposite()
{
	// making opposites a core concept reflects making relations two-ended a core concept
	// an alternative would be to make relations n-ended
	// 	if you wanted to sustain opposites it would open a set of views of the relation, roughly, where the ends were shuffled.
	// considering n=3 helps open setness and possibly order.  can look up by kinds, sources, or destinations, and find the other two.
	// 	atm for me implementation of n>1 requires increasing bootstrapping.
	// 	more generalization thoughts may be needed.
	if (self()->inverse) {
		return self()->inverse;
	}
	auto opposite = new part(destination(), kind() ? kind().opposite() : kind(), source(), data);
	opposite->obverse = self();
	self()->inverse = opposite;
	return opposite;
}

void reference::disconnect()
{
	if (obverse) {
		reference(obverse.lock()).disconnect();
		return;
	}
	for (auto link : incoming()) {
		// MINICONNECTIONS PRESENT.  boilerplate to handle looks large
		throw "connection is connected to";
		//cerr << " - Would like a way to reach out for more here.  Miniconnections present and don't know how to check with the connection on this." << endl;
	}
	for (auto link : outgoing()) {
		// MINICONNECTIONS PRESENT
		throw "connection is connected from";
	}
	if (inverse) {
		for (auto link : opposite().incoming()) {
			// MINICONNECTIONS PRESENT
			throw "connection is connected to";
		}
		for (auto link : opposite().outgoing()) {
			// MINICONNECTIONS PRESENT
			throw "connection is connected from";
		}
	}
	// the complexity here is larger than desired.
	// it should be easy to verify that memory will not be leaked.
	// but it is not, in the face of opposites and miniconnections.
	// 	we weren't planning on disconnecting things?
	// 	but want a way to handle changing data
	// consider if we run into a memory leak-error; we want to be able
	// to resolve it quickly.  who allocated this?  when?
	// there is value to storing this information, but it is not
	// set up to store.  [when making new reference, store execution-state]
	//
	// additionally, karl is rewriting instead of copy-pasting.
	// 	the boilerplate of opposites eases the definition of
	// 	thought, by unifying opposing concepts.
	
	// generalizable in some way.
	// it looks like incoming connections is not needed for request of brief maps.  is just able-to-do-both.
	// maps seem more important.
	self()->inverse.reset();
	if (self()->outgoing_iterator != self()->outgoing.end()) {
		source()->outgoing.erase(self()->outgoing_iterator);
		self()->outgoing_iterator = self()->outgoing.end();
	}
	if (self()->incoming_iterator != self()->incoming.end()) {
		destination()->incoming.erase(self()->incoming_iterator);
		self()->incoming_iterator = self()->incoming.end();
	}
}

void reference::disconnect_all_kind_equal(reference kind)
{
	auto match = incoming().equal_range(kind);
	for (auto connection = match.first; connection != match.second; ++ connection)
	{
		connection->disconnect();
	}
	(*this)->connections.erase(match.first, match.second);
	auto match = outgoing().equal_range(kind) {
	for (auto connection = match.first; connection != match.second; ++ connection)
	{
		if (connection->second->size()) {
			cerr << " - Would like a way to reach out for more here.  Miniconnections present and don't know how to check with the connection on this." << endl;
		}
	}
	}
}

bool reference::connected_kind_equal_destination_equal(reference kind, reference destination)
{
	auto match = (*this)->connections.equal_range(kind);
	for (auto connection = match.first; connection != match.second; ++ connection)
	{
		if (connection->second->destination == destination) {
			return true;
		}
	}
	return false;
}
