// WHOOPS: switching to python3 so our bytecode is the same across systems, since we are learning it.

#include <vector>
#include <string>
#include <map>

template <typename T>
using list = std::vector<T>; // TODO use far-call class
using reference = std::string; // TODO use far-call class
template <typename T>
using lookup = std::map<reference, list<T>>;

struct item;

struct kind
{
	reference identifier;
	void (*use)(kind & _kind, item & selection, item & context);
	reference way_to_pick; // could this be a kind or an item?
};

struct relevent_kind
{
	reference relevence;
	reference identifier;
};

struct item
{
	reference kind;
	reference identifier;
	list<relevent_kind> needs; // can be subparts or goals but mixing both might be confusing
		// note: each need is the reason for running a behavior
	void * data;
};

// the nascent idea is to have ways-to-pick and ways-to-check-if-is-good-idea
// and then to select from kinds to build running code: e.g. function calls, variable references

void use_function(kind & _kind, item & selection, item & context)
{
}

kind function = {
	"c-function",
	use_function,
	"random"
};

// the best reason for python here is that the binary representation is universal across platforms

int main(int argc, char ** argv)
{
	// f3 0f 1e fa // endbr64 // this is just a marker that indicates the code can be the target of an indirect jmp or call
	// 31 c0       // xor eax, eax // mov 0, eax
	// c3          // retq
}

item main_function = {
	"c-function", // kind
	"main", // name
	{
		{ "0-int-argc", "input-value" },
		{ "1-char**-argv", "input-value" },
		{ "int", "output-variable" }
	}, // needs
	(void*)main // data
};


lookup<item> kinds = {
	{ function.identifier, {
		main_function
	}}
};

// goes inside function:
// kinds[function.identifier].push_back(main_function);

// it looks like items might be ways?  dunno.

// it sounds like there is a simple vm-concept that
// has only variables and functions, nothing else, and
// that it wouldn't be hard to write a compiler to any
// representation from this.
// let's make a quick library[s] for that?
// 	or we cna just use the item-thing we're already making?
// 	make-function needs list-of-function-calls
// 	function-call needs function, input values, output variables
//
