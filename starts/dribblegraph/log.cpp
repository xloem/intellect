// similarity

#include <any>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <vector>

using namespace std;

struct connectedness;
using reference = shared_ptr<connectedness>;

// can a map be a connectedness
	// yeah it would interconnect in ways that mean i-am-a-map

struct connectedness
{
	static reference make(reference source, reference kind, reference destination, any data = {})
	{
		auto result = make_shared<connectedness (source, kind, destination, data);
		result->self = result;
		if (source) {
			result->iterator = source->connections.emplace(kind, result);
		}
		return result;
	}
	reference connect(reference kind, reference destination, any data = {})
	{
		return make(self, kind, destination, data);
	}

	template <typename Data>
	Data & force_data(Data initial = {})
	{
		if (data.type() != typeid(Data)) {
			data = initial;
		}
		return any_cast<Data>(data);
	}

	reference kind;
	reference destination;
	any data;

	static reference source_reality;
	static reference kind_unique_information;
	static reference kind_paired_information_first;
	static reference kind_paired_information_second;

	static reference kind_groupness;
	static reference group_error;
	static reference kind_group_multiple;

	static reference kind_member;

	static reference make(reference group, any data = {})
	{
		result = make({}, {}, {}, data);
		result.connect(kind_groupness, group_set);
		return result;
	}

	bool connected_kind_equal_destination_equal(reference kind, reference destination)
	{
		auto match = connections.equal_range(kind);
		for (auto connection = match.begin(); connection != match.end(); ++ connection)
		{
			if (connection->destination == destination) {
				return true;
			}
		}
		return false;
	}

	template <typename Collection>
	bool connected_unique_path_equal(Collection path, reference destination)
	{
		reference spot = self;
		for (auto bend : path) {
			spot = spot.at(bend);
			if (!spot) { return false; }
		}
		return spot->destination == destination;
	}

	/*
	template <typename Collection>
	bool connected_any_path_equal(Collection path, reference destination)
	{
		reference spot = self;
		vector<tuple<typename Collection::iterator, reference, decltype(connections)::iterator>, decltype(connections)::iterator> queue;
		auto iterator = path.begin();
		auto match = connections.equal_range(*iterator);
		queue.emplace_back(path.begin(), match.first, match.second);
		while (queue.size()) {
			auto & entry = queue.back();
			if (entry.first == path.end()
		}
		for (auto bend : path) {
			spot = spot.at(bend);
			if (!spot) { return false; }
		}
		return spot->destination == destination;
	}
	*/

	void must_connected_equal(reference kind, reference destination)
	{
		if (!connected_kind_equal_destination_equal(kind, destination)) {
			throw make(group_error, string("required connection missing"))
				->connect(kind_paired_information_first, kind)->source
				->connect(kind_paired_information_second, destination)->source;
		}
	}

	void must_be(reference group)
	{
		must_connected_equal(kind_groupness, group_set);
	}

	reference filtered(function<bool(reference)> filter)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = self;
		for (auto connection : connections) {
			if (filter(connection.second)) {
				result.connections.emplace(connection.second);
			}
		}
		return result;
	}

	reference mutated(function<reference(reference)> mutation)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = self;
		for (auto connection : connections) {
			auto mutated_connection = mutation(connection);
			if (mutated_connection) {
				result.connections.emplace(mutated_connection);
			}
		}
		return result;
	}

	reference filtered_kind_equal(reference kind)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = self;
		auto match = connections.equal_range(kind);
		for (auto iterator = match.first; iterator != match.second; ++ iterator)
		{
			result.connections.emplace(iterator->first, iterator->second);
		}
		return result;
	}

	reference destinations()
	{
		return mutated([](reference connection){return connection.destination;});
	}

	reference destinations_kind_equal(reference kind)
	{
		return filtered_kind_equal(kind).destinations();
	}

	// ITERATORS
	static reference group_iterator;
	static reference kind_source;

	struct value_iterator : public decltype(connections)::iterator
	{
	public:
		using decltype(connections)::iterator;
		using decltype(connections)::operator++;
		reference operator*()
		{
			return decltype(Connections)::operator*().second;
		}
		reference * operator->()
		{
			return &decltype(Connections)::operator->()->second;
		}
	};

	/*
	reference make_iterator(decltype(connections)::iterator iterator) {
		auto result = make(group_iterator, iterator);
		result.connect(kind_source, self);
		result.connect(kind_unique_information, iterator->second);
		return result;
	}
	connectedness& operator++() // preincrement
	{
		if(connected_kind_equal_destination_equal(kind_groupness, group_iterator)) {
			auto & iterator = any_cast<decltype(connections)::iterator>(data);
			++ iterator;
			at(kind_unique_information)->destination = iterator->second;
			return *this;
		}
		reference next = at(kind_next);
		if (next) {
			return *next->destination;
		}
		throw make(group_error, string("don't know how to increment"))
			->connect(kind_unique_information, self)->source;
	}
	connectedness& operator*()
	{
		return *this->operator->();
	}
	reference operator->()
	{
		reference result = at(kind_unique_information);
		if (result) { return result; }
		return destination;
	}
	*/

	// iteration is done over connections
	connectedness& begin()
	{
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			return list_first();
		} else {
			return make_iterator(connections.begin());
		}
	}
	connectedness& end()
	{
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			return *this;
		} else {
			return make_iterator(connections.begin());
		}
	}
	size_t size()
	{
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			throw make(group_error, string("size not implemented for list"));
		}
		return connections.size();
	}

	static reference kind_member;
	/* // filters seem to make sets already
	// SET
	static reference group_set;
	void set_add(reference item)
	{
		must_be(group_set);
		if (!connected_kind_equal_destination_equal(kind_member, item)) {
			connect(kind_member, item);
		}
	}
	void members()
	{
		return filtered([](reference connection){return connection->kind == kind_member;});
	}
	void set_remove(reference item)
	{
		must_be(group_set);
		must_connected_equal(kind_member, item);
	}
	*/
	

	// GROUP HIERARCHY
	bool is(reference kind)
	{
		if (self == kind) { return true; }
		for (auto group : filtered_kind_equal(kind_groupness)) {
			if (group.is(kind)) { return true; }
		}
		return false;
	}
	reference filtered_kind_is(reference kind)
	{
		return filtered([kind](reference connection){return connection.kind.is(kind);});
	}
	reference filtered_destination_is(reference group)
	{
		return filtered([group](reference connection){return connection.destination.is(kind);});
	}

	// ATTRIBUTES
	reference at(reference kind)
	{
		reference result = filtered_kind_equal(kind);
		if (kind.is(kind_group_multiple)) {
			return result;
		}
		if (result.size() > 1) {
			throw make(group_error, string("single kind used for multiple connections"))
				->connect(kind_paired_information_first, kind)->source
				->connect(kind_paired_information_second, self)->source;
		}
		if (result.size() == 0) {
			return {};
		}
		return *result.begin();
	}
	connectedness& operator[](reference kind)
	{
		reference result = at(kind);
		if (!result) {
			result = make(self, kind, {});
		}
		return *result;
	}
	reference operator=(reference destination)
	{
		this->destination->source.reset();
		return this->destination = destination;
	}
	operator reference()
	{
		return self;
	}

	// ORDEREDNESS
	static reference kind_next;
	static reference kind_previous;
	static reference kind_first;
	static reference kind_last;
	static reference group_list;
	static reference group_list_entry;
	static reference make_list()
	{
		return make(group_list);
	}
	reference list_first()
	{
		must_be(group_list);
		return this->at(kind_first);
	}
	reference list_last()
	{
		must_be(group_list);
		return this->at(kind_last);
	}
	void list_push(reference item)
	{
		must_be(group_list);
		reference entry = make(group_list_entry);
		entry.connect(kind_unique_information, item);

		reference last = this->at(kind_last);
		if (!last) {
			(*this)[kind_first] = entry;
			(*this)[kind_last] = entry;
		} else {
			(*last)[kind_next] = entry;
			(*entry)[kind_previous] = last;
			(*this)[kind_last] = entry;
		}
		++ force_data<size_t>(0);
	}
	size_t list_size()
	{
		must_be(group_list);
		return force_data<size_t>(0);
	}
	void list_append(reference item)
	{
		must_be(group_list);
		reference entry = make(group_list_entry);
		entry.connect(kind_unique_information, item);

		reference last = this->at(kind_last);
		if (!last) {
			(*this)[kind_first] = entry;
			(*this)[kind_last] = entry;
		} else {
			(*last)[kind_next] = entry;
			(*entry)[kind_previous] = last;
			(*this)[kind_last] = entry;
		}
		++ force_data<size_t>(0);
	}
	class list_iterable
	{
	public:
		list_iterable(reference list)
		: list(list) { }
		
		reference const list;

		list_iterator begin()
		{
			return {list->at(kind_first)};
		}
		list_iterator end()
		{
			return {{}};
		}

		class list_iterator
		{
		public:
			list_iterator(reference item)
			: item(item)
			{ }
			connectable& operator*()
			{
				return *item->at(kind_unique_information)->destination;
			}
			reference operator->()
			{
				return item->at(kind_unique_information)->destination;
			}
			list_iterator &operator++()
			{
				item = item->at(kind_next);
				return *this;
			}
			list_iterator &operator--()
			{
				item = item->at(kind_previous);
				return *this;
			}
			bool operator==(list_iterator const& other)
			{
				return item == other.item;
			}
		private:
			reference item;
		};
	};
	list_iterable list_items()
	{
		must_be(group_list);
		return {self};
	}

	// EXPLORER
	static reference group_explorer;
	static reference kind_explored;
	static reference group_journey;
	void explorer_add(reference item)
	{
		must_be(group_explorer);
		if (connected_kind_equal_destination_equal(kind_member, item)) {
			return;
		}
		if (connected_kind_equal_destination_equal(kind_explored, item)) {
			return;
		}
		connect(kind_member, item);
	}
	bool explorer_explored(reference item)
	{
		must_be(group_explorer);
		return connected_kind_equal_destination_equal(kind_explored, item);
	}
	size_t & explorer_depth()
	{
		must_be(group_explorer);
		return force_data<size_t>(0);
	}
	reference explorer_explore(function<bool(reference)> filter)
	{
		must_be(group_explorer);
		for (auto member : destinations_kind_equal(kind_member)) {
		 	for (auto connection : member) {
				if (explorer_explored(connection)) {
					continue;
				}
				connect(kind_member, connection);
				connect(kind_explored, connection);
			}
			connect(kind_explored, member);
		}
		++ explorer_depth();
		// okay, we theoretically have lists now,
		// so we should keep our kind_members as
		// lists of connections to reach things
		// ONGOING
		return destinations_kind_equal(kind_member);
	}

	// ONGOING: MAKING DIRECTION-LIST EXPLORER TO CONSTRUCT
	// GRAPH-PART SIMILARITY

private:
	multimap<reference, reference> connections;
	multimap<reference, reference>::iterator iterator;

	weak_ptr<connectedness> self;
	reference source;

	connectedness(reference source, reference kind, reference destination, any data)
	: kind(kind), destination(destination), data, source(source)
	{ }
};

class explorer
{
public:

};

// okay made a cool all purpose link class.
// it can easily hold a set of neighbor links.
// i would like to expand it to keep track of its depth,
// and find further-distant neighbor links.
// likely also use a lamba to consider whether to deepen
// a link first.

// likely a separate construct due to specializedness of function, dunno
// but it seems like the basic behavior is:
// 	sets:
// 		1 fully visited
// 		2 visiting
// 		3 destinations
// 	so, first we have one item in 3
//	we move 3 to 2
//	we find all connections in 2
//		everything in 2 is moved into 1
//		the destinations go in 3 iff they are not in 1
//
// the current quick-pattern is to put reference methods
// inside the one big class.  hard to think on it, so let's
// consider keep doing that.
// we can use a reference link to handle what is fully visited.
