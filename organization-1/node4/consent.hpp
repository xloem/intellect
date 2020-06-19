#include "reference.hpp"

#include <library/string.hpp>

class string : public reference
{
	string(library::string data)
	{
		data-default<library::string>(data);
	}
};

// each statement has a kind
// if each statement were a link, kinds would be basic.
// one kind-link.

// has an ordered list of data, and a kind-reference to another statement
class statement : public reference
{
	// let's just give them a kind attributed
	
	DEFINE(reference,,kind-statement-kind);
	DEFINE(reference,,kind-statement-focus);

	DEFINE(reference,,kind-context);
	DEFINE(reference,,kind-action);
	DEFINE(reference,,kind-consent);
	DEFINE(reference,,kind-block);

	// hmm
	DEFINE(reference,,kind-affected-by);
	DEFINE(reference,,kind-affects);

	// no constructor methods yet

	statement(reference kind)
	{
		kind-set(kind-statement-kind, kind);
	}
	statement(reference kind, statement other)
	{
		kind-set(kind-statement-kind, kind);
		kind-set(kind-statement-focus, other);
	}

	METHOD add-data(reference data)
	{
		self.order-set(self.order-count(), data);
	}
};

// when system boots up, it will want to interface with storage.
// let us load everything from storage for now ...
// 		[imagines UI, counter to written plan]
// 		-> as a user, we want to either make proposals,
// 		review proposals in contexts we value,
// 		make-proposal
// 		what-do-i-need-to-consent-to
// 		and most importantly we are checked for consent around
// 		proposals and blocks that affect us.
//
// 			identity.

#include <fstream>

class consensus-user : public reference
{
	consensus-user(library::string path)
	{
		std::fstream
	}
};

class consensus-thingy : public reference
{
	METHOD statement make-statement(reference data)
	{
		
	}

	// has a list of statements, ordered
	METHOD void add-statement(statement to-add)
	{
	}

	METHOD void block-statement(statement to-block, reference reason)
	{
	}
};
