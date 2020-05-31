#include <iostream>
#include <string>

#include "reference.hpp"

using namespace std;

// okay time-value
//
// basics:
// 	wayness: how-to-update, how-to-pick
// 	activate state
// 	connectedness: time-values on choices

// choices will have values on each option
// so implementing connectedness a little could help.
	// connectedness could use some basics of typedness.

// we note that gettersetterness has great utility for generalization
// we also note that connectedness aids time-value
// 	the generalized solution is to use wayness for gettersetterness.  then wayness can do connectedness and typedness and others.
// 		the original wayness needs some kind of typedness.  this will be handled by simply copying it around.

// we're proposing there is a special member for getting attributes.  this member can implement typedness, connectedness, etc.

// we just need words for theese things.

class connection_reference : public reference
{
public:
	//connection_reference
};

class indirect_reference : public reference
{
	// say we want to make [] = work.
	// it would help to use connection objects.
};

/*
class getter : public reference
{
public:
	static reference getter;

	reference operator[](reference which);
};
*/

// this could be merged into the root concept: it would check for a get member.

// possibility
class type_reference : public reference
{
public:
	// we want to be able to set members that are inherited by types
	// other possibilities include:
	// 	- creating something of a type
	// 	- verifying something is of a type
	// 	- verifying something of a type is used as that type should be used
};

int main(int argc, char ** argv)
{
	reference concept, is, object, chair, wood, material;
	reference::basic_set(chair, is, object);
	reference::basic_set(concept, is, chair);
	reference::basic_set(concept, material, wood);

	((string&)wood) = "tree-body";

	cout << (string)reference::basic_get(concept, material) << endl;
}
