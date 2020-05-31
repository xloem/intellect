#include <vector>

// the universe is made of parts, all of which are linked to each other in some way.
// we summarize this in our different contexts, considering only briefly some of the links, and extensively others.

template <typename Way>
struct Node {
	std::map<relation, size_t> peer_lookup;
	std::vector<std::pair<Way, std::shared_ptr<node>>> peers;
	std::vector<Way> interlinks; // relations between peer connections
};

template <typename Amount>
class view { 
public:
	using node = view<Amount>;
	using node_ptr = std::shared_ptr<node>;

	struct relation {
		Amount way;
		node_ptr way;
		node_ptr what;
	}; // each relation has a set of links to other relations.

	// it sounds like we have pairs or lists of metaness
		// these things all might to be connected so learning can spread
		// along them

	// this tiny view is like a decision: it has a kind of nearness
	// and amounts everything is near
	std::map<relation, size_t> peer_lookup;
	std::vector<std::pair<Amount, std::shared_ptr<node>>> peers;
	std::vector<Amount> interlinks; // relations between peer connections

	// other things are near what is near
	// 		[ways of looking for kinds]
	// 		[kinds of kinds]
	// 		[the idea of looking for other stuff]
	
	// we also want kinds near that kind [contexts near that context]

	// and an extra: e.g. the way of looking for near kinds, or the depths
	// of kinds of near contexts.
};

using node = view<double>;
using node_ptr = std::shared_ptr<node>;

node_ptr universe = new node();

/*
node_ptr metaness = new node({universe, metaness, 1000});

node_ptr relationness = new node({universe, metaness, 10});

node_ptr partness = new node({universe, relationness, 1});
*/
node_ptr word = new node({universe, "part", 1},{universe, "kind", 1});
node_ptr apple = new node("apple", {word, "instance", 1});
	// might make syntax to be like word.make("apple");
	// can we make nodes be links
	// views are like links.  views of reality where what's interesting
	// is an apple-use.

	// a link is a graph is a node
	// everything is interlinked, but only some things are worth exploring
	// or storing

node_ptr summary = new node();
node_ptr details = new node();


// request to make below a smooth proposal to ease storing references +1

// 1 consider morphing between views

	// when morphing between views, we consider all the pairings between
	// them, and walk the best paths.

// 2 consider kinds of relations

// below is an example near merging contextual interlinks smoothly for learning.
// a reading-realization is below too.

// maybe a better example is words. [used to have letters here]
// 'that book is cool' [can i buy it?]
// vs
// 'that book is cool, brr' [did you leave it in the fridge?]
// result in very different contexts =)
//
//	okay i see. and we want to smoothly enter the one that makes sense.

// we want to consider morphing the two graphs/vectors [contexts]
// we can totally take each node, and move them into each other
