// similarity

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

struct connectable;
using reference = shared_ptr<connectable>;
using weak_reference = weak_ptr<connectable>;

// can a map be a connectable
	// yeah it would interconnect in ways that mean i-am-a-map

struct connectable
{
	// automatically adds to source if provided
	// TODO: maintain a list of incoming connections (weak_ptrs?): progresses to keeping connection kinds in invertable pairs
	// HM: this might be simplifiable to just use sets for everything.  each set is a reference, in many other sets.
	// HM HM: it seems like each link should have a way of asking for anything at all, and if can't be handled recipient should try to reach out for more
	static reference make(reference source, reference kind, reference destination, any data = {})
	{
		reference result(new connectable(source, kind, destination, data));
		result->self = result;
		if (source) {
			result->iterator = source->connections.emplace(kind, result);
		}
		return result;
	}
	reference connect(reference kind, reference destination, any data = {})
	{
		return make(self.lock(), kind, destination, data);
	}

	template <typename Data>
	Data & force_data(Data initial = {})
	{
		if (data.type() != typeid(Data)) {
			data = initial;
		}
		return *any_cast<Data>(&data);
	}

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
		auto result = make({}, {}, {}, data);
		result->connect(kind_groupness, group);
		return result;
	}

	void disconnect_all_kind_equal(reference kind)
	{
		auto match = connections.equal_range(kind);
		for (auto connection = match.first; connection != match.second; ++ connection)
		{
			if (connection->second->size()) {
				cerr << " - Would like a way to reach out for more here.  Miniconnections present and don't know how to check with the connection on this." << endl;
			}
		}
		connections.erase(match.first, match.second);
	}

	bool connected_kind_equal_destination_equal(reference kind, reference destination)
	{
		auto match = connections.equal_range(kind);
		for (auto connection = match.first; connection != match.second; ++ connection)
		{
			if (connection->second->destination == destination) {
				return true;
			}
		}
		return false;
	}

	template <typename Collection>
	bool connected_unique_path_equal(Collection path, reference destination)
	{
		reference spot = self.lock();
		for (auto bend : path) {
			spot = spot->at(bend);
			if (!spot) { return false; }
		}
		return spot->destination == destination;
	}

	/*
	template <typename Collection>
	bool connected_any_path_equal(Collection path, reference destination)
	{
		reference spot = self.lock();
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


	// this could be simplified to sets:
	// every link has a reverse link
	// which simplifies to tracking incoming connections
	void must_be(reference group)
	{
		must_connected_equal(kind_groupness, group);
	}

	// a view is a reference that is imaging another reference and reports the other as itself
	bool is_view()
	{
		return view.use_count();
	}
	void must_be_view()
	{
		if (!is_view()) {
			throw make(group_error, string("already embodied: not a view"))
				->connect(kind_unique_information, self.lock())->destination;
		}
	}
	reference reference_to_this_view()
	{
		must_be_view();
		return view.lock();
	}
	/* // this should work but seems to overcomplicate the approach
	void view_embody_miniconnections_viewed()
	{
		self = view;
		view.reset();
		auto old_connections = connections;
		connections.clear();
		for (auto connection : old_connections) {
			reference connections_as_view = make(self.lock(), connection->kind, connection->destination, connection->data);
			connections_as_view->connections = connection->connections;
			connections_as_view->self = {}; // unknown whether it will be used for connections or reference, so unset
			connections_as_view->view = connections_as_view;
		}
	}
	void view_embody_miniconnections_dropped()
	{
		self = view;
		view.reset();
		auto old_connections = connections;
		connections.clear();
		for (auto connection : old_connections) {
			reference connections_as_view = make(self.lock(), connection->kind, connection->destination, connection->data);
		}
	}
	*/

	reference viewed()
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = {};
		result->view = result;
		return result;
	}

	/*
	reference cloned_miniconnections_viewed()
	{
		reference result = viewed();
		result.embody_miniconnections_viewed();
		return reference;
	}

	reference cloned_miniconnections_dropped()
	{
		reference result = viewed();
		result.embody_miniconnections_dropped();
		return reference;
	}
	*/

	reference filtered(function<bool(reference)> connection_filter)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = {};
		result->view = result;
		for (auto connection : connections) {
			if (connection_filter(connection.second)) {
				result->connections.emplace(connection);
			}
		}
		return result;
	}

	reference mutated(function<reference(reference)> connection_mutation, bool take_empty_connections = false)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = {};
		result->view = result;
		for (auto connection : connections) {
			auto mutated_connection = connection_mutation(connection.second);
			if (mutated_connection) {
				if (!mutated_connection->source && take_empty_connections) {
					mutated_connection->source = result;
				}
				result->connections.emplace(mutated_connection->kind, mutated_connection);
			}
		}
		return result;
	}

	reference filtered_kind_equal(reference kind)
	{
		auto result = make({}, kind, destination, data);
		result->source = source;
		result->self = {};
		result->view = result;
		auto match = connections.equal_range(kind);
		for (auto iterator = match.first; iterator != match.second; ++ iterator)
		{
			result->connections.emplace(iterator->first, iterator->second);
		}
		return result;
	}

	reference destinations()
	{
		return mutated([](reference connection){return connection->destination;});
	}

	reference destinations_kind_equal(reference kind)
	{
		return filtered_kind_equal(kind)->destinations();
	}

	// ITERATORS
	//static reference group_iterator;
	//static reference kind_source;

	using Connections = multimap<reference,reference>;
	struct value_iterator : public Connections::iterator
	{
	public:
		value_iterator(Connections::iterator const & iterator)
		: Connections::iterator(iterator)
		{ }
		using Connections::iterator::operator++;
		auto operator*()
		{
			return *Connections::iterator::operator*().second;
		}
		auto operator->()
		{
			return Connections::iterator::operator->()->second;
		}
	};

	/*
	// future: to help clarity, let us put group-specific functions in their own methods, and then call them from operators.
	reference make_iterator(decltype(connections)::iterator iterator) {
		auto result = make(group_iterator, iterator);
		result.connect(kind_source, self.lock());
		result.connect(kind_unique_information, iterator->second);
		return result;
	}
	connectable& operator++() // preincrement
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
			->connect(kind_unique_information, self.lock())->source;
	}
	connectable& operator*()
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
	auto begin()
	{
		/*
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			return list_first();
		} else {
			return make_iterator(connections.begin());
		}
		*/
		return value_iterator(connections.begin());
	}
	auto end()
	{
		/*
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			return *this;
		} else {
			return make_iterator(connections.begin());
			// do we have a way to remove a connection yet?
		}
		*/
		return value_iterator(connections.end());
	}
	size_t size()
	{
		if (connected_kind_equal_destination_equal(kind_groupness, group_list)) {
			throw make(group_error, string("size not implemented for list"));
		}
		return connections.size();
	}

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
		if (self.lock() == kind) { return true; }
		for (auto group : *filtered_kind_equal(kind_groupness)) {
			if (group.is(kind)) { return true; }
		}
		return false;
	}
	reference filtered_kind_is(reference kind)
	{
		return filtered([kind](reference connection){return connection->kind->is(kind);});
	}
	reference filtered_destination_is(reference group)
	{
		return filtered([group](reference connection){return connection->destination->is(group);});
	}

	// ATTRIBUTES
	reference at(reference kind)
	{
		reference result = filtered_kind_equal(kind);
		if (kind->is(kind_group_multiple)) {
			return result;
		}
		if (result->size() > 1) {
			throw make(group_error, string("single kind used for multiple connections"))
				->connect(kind_paired_information_first, kind)->source
				->connect(kind_paired_information_second, self.lock())->source;
		}
		if (result->size() == 0) {
			return {};
		}
		return *result->begin();
	}
	connectable& operator[](reference kind)
	{
		reference result = at(kind);
		if (!result) {
			result = make(self.lock(), kind, {});
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
		return self.lock();
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
	reference list_first_entry()
	{
		must_be(group_list);
		return this->at(kind_first);
	}
	reference list_last_entry()
	{
		must_be(group_list);
		return this->at(kind_last);
	}
	reference list_entry_item()
	{
		must_be(group_list_entry);
		return this->at(kind_unique_information);
	}
	reference list_first()
	{
		return list_first_entry()->list_entry_item();
	}
	reference list_last()
	{
		return list_last_entry()->list_entry_item();
	}
	void list_push_back(reference item)
	{
		must_be(group_list);
		reference entry = make(group_list_entry);
		entry->connect(kind_unique_information, item);

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
	reference list_copy()
	{
		must_be(group_list);
		reference new_list = make_list();
		for (auto item : list_items()) {
			new_list->list_push_back(item);
		}
		return new_list;
	}
	size_t list_size()
	{
		must_be(group_list);
		return force_data<size_t>(0);
	}
	class list_iterable
	{
	public:
		list_iterable(reference list)
		: list(list) { }
		
		reference const list;

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
			bool operator!=(list_iterator const& other)
			{
				return item != other.item;
			}
		private:
			reference item;
		};

		list_iterator begin()
		{
			return {list->at(kind_first)};
		}
		list_iterator end()
		{
			return {{}};
		}
	};
	list_iterable list_items()
	{
		must_be(group_list);
		return {self.lock()};
	}

	// UNIQUE DATA
	static reference group_unique_data;
	template <typename Type>
	static inline reference group_type()
	{
		static reference group_type_reference = make(group_unique_data, typeid(Type));
		return group_type_reference;
	}
	template <typename Type>
	static reference from(Type data)
	{
		// note it should be fine to use a weak_reference here, would save memory
		static std::unordered_map<Type, reference> by_data;
		reference & result = by_data[data];
		if (!result) { result = make(group_type<Type>(), data); }
		return result;
	}
	template <typename Type>
	reference unique_data()
	{
		must_be(group_type<Type>());
		return any_cast<Type>(data);
	}

	// HASHING
	using hash_t = size_t;
	static hash_t hash(reference to_hash)
	{
		return std::hash<reference>()(to_hash);
	}
	static hash_t hash_combine(size_t left, size_t right)
	{
		return (left << 1) ^ right;
	}
	hash_t hash_connections()
	{
		size_t hash_data = 0;
		// this changes depending on order ... but same items will order the same
		for (auto connection : *this) {
			hash_data = hash_combine(hash_data, hash(connection));
		}
		return hash_data;
	}
	// TOFO hash_deep, hash_mini?

	// below here is weirder ... i think can be simplified a ton when thinking clearer.  most complicated issue presently aware of is having new differences found when new material is connected in [current explorer would preserve old maps, so that new links can be plugged into them when made]
	
	// PART MAPS
	// TODO: maybe make maps just lists of kinds for simplicity, instead of lists of sources, kinds, and destinations.  takes changing.
	static reference group_map;
	reference at_map(reference map)
	{
		map->must_be(group_list);
		map->must_be(group_map);
		reference spot = (viewed() ? view : self).lock();
		for (auto direction : group_map->list_items()) {
			spot = spot->at(direction.kind);
			if (!spot) { return spot; }
		}
		return spot;
	}

	// EXPLORER
	static reference group_explorer;
	static reference kind_explored_fully;
	static reference kind_explored_partially;
	reference make_explorer(reference item)
	{
		reference result = make(group_explorer);
		result->explorer_add(item);
		return result;
	}
	void explorer_add(reference item)
	{
		must_be(group_explorer);
		if (explorer_explored(item)) {
			return;
		}
		reference map = make_list();
		map->connect(kind_groupness, group_map);
		map->list_push_back(make({},{},item));
		connect(kind_member, map);
	}
	bool explorer_explored(reference item)
	{
		must_be(group_explorer);
		return connected_kind_equal_destination_equal(kind_explored_partially, item) || connected_kind_equal_destination_equal(kind_explored_fully, item);
	}
	size_t & explorer_depth()
	{
		must_be(group_explorer);
		return force_data<size_t>(0);
	}
	reference explorer_explore(function<bool(reference)> connection_filter)
	{
		must_be(group_explorer);
		vector<reference> new_members;
		for (auto map : *destinations_kind_equal(kind_member)) {
			auto last = map.list_last();
			if (last->size()) {
				cerr << " - I could use a way to reach out to more here.  I'm exploring and ran into connections that have their own connections, but the maps we're using don't represent that." << endl;
			}
			auto member = last->destination;
			if (!connected_kind_equal_destination_equal(kind_explored_fully, member)) {
				bool explored_destination_fully = true;
				for (auto connection : *member) {
					if (explorer_explored(connection)) {
						continue;
					}
					if (connection_filter(connection)) {
						auto new_map = map.list_copy();
						new_map->list_push_back(connection);
						new_members.push_back(kind_member);
						connect(kind_explored_fully, connection);
					} else {
						explored_destination_fully = false;
					}
					connect(kind_member, connection);
				}
				if (explored_destination_fully) {
					connect(kind_explored_fully, member);
				} else if (!connected_kind_equal_destination_equal(kind_explored_partially, member)) {
					connect(kind_explored_partially, member);
				}
			}
		}
		++ explorer_depth();
		disconnect_all_kind_equal(kind_member);
		for (auto member : new_members)
		{
			connect(kind_member, member);
		}
		cerr << " - could use a way to reach out: i explored some places but didn't track their connectedness to where i started.  is that what connectedness is for?" << endl;
		return destinations_kind_equal(kind_member);
	}

	// finding similarity means comparing maps, to see if any maps that are the same end with the same thing.
	// we can keep looking for similarity exhaustively, using an explorer.
	
	// the algorithm would instantiate an explorer for every item in the group[set],
	// and look for matching map+final-destinations.  the ones that matched for every item are the set-similarities.
	// the plan is to start with the fewest needed.
	
	// question: how could we upgrade explorer to track the parallel maps internally?
	// 	it seems like it would be doing the same thing as instantiating an explorer for each item
	// 	but if it did them in parallel, it might be able to look for an equivalent one when it made a new one,
	// 	or something.
	// do naively for now.
	

	static reference group_similarity_finder;
	static reference kind_maps;
	static reference kind_explorer;
	reference make_similarity_finder(reference item_set)
	{
		// realistically, we will expand depth until we find them all.  they'll be in the form of maps that an explorer could explore.
		// so, if we seed the explorer with maps, then it could function same as similarity finder
		reference result = make(group_similarity_finder);
		for (auto item : *item_set)
		{
			reference explorer = make_explorer(item);
			result->connect(kind_explorer, explorer);
		}
		return result;
	}


	reference similarity_finder_explore(reference other, function<bool(reference)> connection_filter)
	{
		must_be(group_similarity_finder);
		disconnect_all_kind_equal(kind_maps);
		for (auto explorer : *destinations_kind_equal(kind_explorer)) {
			connect(kind_maps, explorer.explorer_explore(connection_filter));

		}
		// intersetion of all the sets
		// with a specific set of stuff used to judge identiy
		// 	-> list entries, kinds of all, destination of last
		cerr << " -  I kind of want to reach out to more to generalize this.  Using a rather specific way of noting which parts of something matter to me here." << endl;
			// hasher is a mutator
			// acting on maps that are more
			// general than our current maps.
		auto hasher = [](reference list) {
			hash_t result = 0;
			for (auto item : list->list_items()) {
				result = hash_combine(result, hash(item.kind));
			}
			result = hash_combine(result, hash(list->list_last()->destination));
			return result;
		};

		// goal: similarity of maps
		// use hasher
		// when find map that is missing, put in difference
		/*
		reference accumulated = make(group_set);
		reference similarity = make(group_set);
		reference difference = make(group_set);
		*/

		auto maps_set = *destinations_kind_equal(kind_maps);

		// we'll want to convert the maps to hashes

		// break out process.  do in small parts.
		unordered_map<size_t, reference> similarities_by_hash;
		unordered_map<size_t, reference> differences_by_hash;

		
		bool first = true;
		for (auto maps : maps_set) {
			unordered_map<size_t, reference> paths_by_hash;
			for (auto map : maps) {
				paths_by_hash[hasher(map)] = map;
			}
			if (first) {
				first = false;
				similarities_by_hash = paths_by_hash;
				continue;
			}
			// if it is not in similarities_by_hash, it is a difference.
			// if something in similarities_by_hash is not covered, that is a difference
			// otherwise it is indeed a similarity
			auto old_similarities = similarities_by_hash;
			similarities_by_hash.clear();
			for (auto map : maps) {
				auto hash = hasher(map);
				auto node = old_similarities.extract(hash);
				if (node) {
					similarities_by_hash.insert(move(node));
				} else {
					differences_by_hash.insert(move(node));
				}
			}
			differences_by_hash.merge(old_similarities);

		}
		for (auto similarity : similarities_by_hash) {
			connect(kind_member, similarity.second);
		}
		return *destinations_kind_equal(kind_member);
	}

	static reference group_differences;
	reference similarities_differentiate(reference other_similarities)
	{
		must_be_view();
		other_similarities->must_be_view();
		must_be(group_similarity_finder);
		other_similarities->must_be(group_similarity_finder);

		reference left = view.lock();
		auto right = other_similarities;

		reference results = make(group_differences);
		
		// this and other are already list of similarities shared by group
		auto hasher = [](reference list) {
			hash_t result = 0;
			for (auto item : list->list_items()) {
				result = hash_combine(result, hash(item.kind));
			}
			return result;
		};
		// we find similarities that are in both lists, but have different ends
		for (auto left_map : *left) {
			hash_t left_directions_hash = 0;
			for (auto item : left_map.list_items())  {
				left_directions_hash = hash_combine(left_directions_hash, hash(item.kind));
			}
			// see if map is in other
			for (auto right_map : *right) {
				hash_t right_directions_hash = 0;
				for (auto item : right_map.list_items()) {
					right_directions_hash = hash_combine(right_directions_hash, hash(item.kind));
				}
				if (right_directions_hash == left_directions_hash && left->list_last()->destination != right->list_last()->destination) {
					results->connect(kind_member, left_map.mutated([](reference connection){return connection->kind ? make({},connection->kind,{}) : reference{}; }));

				}
			}
		}
		return results;
	}


	// PLEASE MAKE THIS SO SIMILARITY/DIFFERENCE OPERATIONS
	// CAN BE PULLED OUT AND USED FOR OTHER THINGS, GOING
	// FORWARD
	// ^-- whatever!
	
		// two things here: what of our links differentiate us from other, and what of a set differentiates each from other
		// <BLARGH!>
		// us the same as other, and what of a set
		// makes 
	
	// the next behavior is to find the attributes that
	// always differ between two sets of things.
	// one algorithm is to find the similarities shared
	// for each set, and then find the results that are
	// not shared between the two.
	// 	(there is a bug in that if one set has unknown
	// 	items that item may be treated as different
	// 	when it is not
	// 	the quickest fix is to only include differences
	// 	where content is known for both sets.)
	// so we have two things [say]
	// we form two sets of similarities
	// we take the difference between the sets, with
	// a filter on it

	// ONGOING: MAKING DIRECTION-LIST EXPLORER TO CONSTRUCT
	// GRAPH-PART SIMILARITY

	// differences are quick and easy? how's that work?
	// 	two groups, already known to have a similarity between them,
	// 	want to find the difference
	// 	find similarities between separate groups
	// 	and form set of things that are not in both.
	
private:
	weak_reference view;
	multimap<reference, reference> connections;
	multimap<reference, reference>::iterator iterator;

	weak_reference self;
	reference source;
	reference kind;

	connectable(reference source, reference kind, reference destination, any data)
	: destination(destination), data(data), source(source), kind(kind)
	{ }
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


// TODO: split 'modules' out into classes that construct with a reference, instead of chunks in one class.
