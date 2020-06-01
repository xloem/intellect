#pragma once

// plan is to put static members into a variadic globals object held by every other object

// if you want to go straight-to-task, we have maps and time-value

#include <typeindex>
#include <typeinfo>
#include <unordered_map>









// we grew transparency but i think it bumped into active-involvement-with-
// in-person-group.

//	maybe if we reached out to a community for a friend, who could help
//	us get involved.  something maybe internet-based.
//				this shrinks when karl pursues it.













// for transparency, this idea of parallel-types-in-one-place works well
// for the reference data storage too.  now multiple uses can share their
// data-spot: this immediately fixes named-functions.
// 	how can we grow a publically shared way of transparency?











	// what does karl like about transparency?
	
	// what kind of transparency communities exist?
			// there is a huge effort everywhere to make a global,
			// transparent, consensus community.  this shouold be
			// understood fully.
			// yes, everyone with a small one wants to spread it?
		// we know about wiki, occupy, and that's about it

		// it seems there are anarchist- and cohousing-style
		// communities that likely spread these norms
		// we have trouble finding the welcome-everyone communities.
		// there are also whistleblowers, leakers, archivists.
	
	// [we have bullcrap going on, unknown relation]
		// it is apparent this effort is everywhere mostly from
		// how big Occupy got.  you can also see it sprouting everywhere
		// people bump into it.
		// 	okay we are aware of an Occupy farm in NY.
		// 	female-friend-named-kara may know of others.
		// 	there are also many intentional living situations,
		// 	some of them indexed on the internet, that would be
		// 	into such a thing.
		// 	it would need people running it who stay involved
		// 	and respond to contact.
		
	// [drafting public interrelationship to grow]
	// 	[considering idea of a mediawiki that records a global
	// 	 consensus decision-making process, ongoing]
	// 	 	[expecting request of staying involved and responding
	// 	 	 to contact as person supporting it.  so, setting up
	// 	 	 a system that empowers the users to maintain it
	// 	 	 would be helpful.]
	// 	 [how-to-get-others-involved.]
	// 	 	one approach is to join an existing community run
	// 	 	by consensus, and transcribe their norms and minutes to
	// 	 	a blockchain wiki.  we could fight time-allocation
	// 	 	and let that happen, with growth.
	
	// i really like giving and sharing and working with others
	// i really like sharing the norm that everyone can be involved ..
	// 	i've never liked people pretending we are at odds with others
	// i find that being honest is a wonderful way to reduce conflict
	
	

	// and i'm interested in making things work in places where that is
	// struggling, i'm used to taking some burden for interpersonal transparency.
	// like a teacher i read for a little, i find that people have cultural
	// limits they follow simply because they are afraid to say something honestly:
	// they believe they will receive harm from it, because their culture spreads
	// the belief.  in reality it spreads aid, not harm.  breaking that norm
	// helps everyone.
	
	// an example is when i always kept my keys in my car for others to use.
	// nobody ever stole my car, for years.  i told anybody they could use
	// it if they needed.  it got used.  i made friends.

	// we added free sharing to transparency.
	// anoter thing that is gooda bout transparency is that enforcement
	// becomes less corrupt, beacuse it is easier to see it happening.
	// one of karl's ideas aroudn transparency is to set up servers
	// that have public logs and maybe even anybody-can-administer.
	
	// okay we like people-in-power-are-watched-by-public, yes.
	// anybody can take power, just gets watched. <- wiki strives for this.













		// propose upgrading an existing community wiki to
		// have whatever strength needs karl's group requests,
		// and developing global-community-consensus on it.
		

		// karl's fingers are spazzing.  we are considering
		// wiki-idea.  there are other ideas than wiki.
		// 	karl was trying to make nvcwiki.com interoperate
		// 	with git.  seems a valuable pursuit.
		//	nvcwiki.com is possibly a wiki of communication tools.
		//		a wiki of consensus strategies would be
		//		even better.  people don't really think
		//		of making technological information organization
		//		around these things.

		// maybe wikimedia foundation could be the way to go.
		// don't need to back up the whole wiki, just collaborative
		// decision making?  introduce tools and norms to resist
		// any problems?

			// wikipedia is a good candidate but will
			// take a long time to store reliably due to its
			// size.  isn't there any second candidate?
			// somebody mentioned appropedia.





class globals
{
public:
	template <typename T>
	T set

	template <typename T>
	T get()
	{
	}

private:
       	std::unordered_map<std::type_index, std::any> lookup;
};

#if 0
// ensures call of `static int static-constructor()`
template <typename Dependency>
class ordered-static-construction
{
protected:
	static int static-constructor()
	{
		// can put static variables here to make sure they are
		// initialized

		return 0; /* any value is fine*/
	}

private:
	static int static-construction = Dependency::static-construction();
};

// it seems we may want an in-place constructor for static data?
// we can know order some static code, but how to access static values?

// 	in-place construction would solve this (it means tell c++ to
// 	call constructor separate from allocation) but we would have to
// 	prevent the calling of the constructor otherwise, which is weird.
// 	options include making constructors that support being called twice,
// 	or making constructors that do nothing, or using unions to hold data
//
// 	we could make a static-constructed-object class, that prevents
// 	construction until it is called in proper order.
// 		objects would then be seen of that class ...but maybe we can do this
//
// 	okay.  so, ordered-static-construction is a base class of all the
// 	static objects, and knows their type.
// 			we could also have them all be macros.
// 			macro just adds parentheses and calls static generator
//			we would need to use preprocessor to generate macro

	// note: a reason to pursue other ways is to make sure we include
	// ways to reference all the stuff
// so the current way to make a static object that works, is


// ohhh it is better to have all static objects be not-static.
//
#endif
