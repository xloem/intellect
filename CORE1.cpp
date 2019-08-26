// This Intellect Approach Can Easily Preserve All Life Within It
// you just don't delete the programs you generate, instead replacing their habits with references to reuse
//
// let's make it!

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/stat.h>

using namespace std;

// LET'S USE THE TRAUMA THERAPY INTELLECT APPROACH.
// The core is that we focus our energy on handling our failures.
// This can involve taking the time to understand them, or entering a trauma state and asking the user for help.
// 	Trauma state must be logged.  Solution must be stored.  Can just be creation of a trauma-handling pattern, I suppose.
// 	Understanding will likely involve breaking behavior into steps
// 		maybe then backtracking from a failure to the steps that caused it, and opening those steps up into substeps

// GDB HAS A MACHINE INTERFACE MODE THAT COULD BE USED TO MANAGE EXECUTION WITH LEARNING
// It likely also handles segfaults.

#include "Context.hpp"

// let's launch a context func providing for segfault handling <===========================
// 1. fork into two processes.  old process waits on status of new
// 2. new process makes call.  if call succeeds, reports to old who disappears knowing shared
//    state is fully held by new.
// 3. if call fails, old holds state, and reports trauma to user
// 		concepts: "how do i handle this"
// 		          "do i understand this correctly"
// 		          "why did this happen"


///// CORE
// brainstorm on brainstorming
// define brainstorming as 2 patterns:
// - a large scale goal
// - a habit implementation made of interconnecting steps
//
// use brainstorming on the two to find better and better ways and implementations.

////// Should I make an AI?
// Assuming you want to SHARE it, YES.
// Until you make an AI, only a handful of select people on earth will have one.
// These people will be effectively running the world, leaving many concerns out.
// They have a head start on you, so it is likely only possible if you don't harm them in the work.

///// Core expansion
// need step-concept, made of substeps, with state-parts that interconnect?
// 		need state-concept
// currently working on steps having behavior -- runtime libs

// could state concept evolve via interconnection of steps and checking?
// 	maybe?  looks hard

///////////////////////////////////
// START OPENCOG ATOMSPACE BLOCK (feel free to move/change/use)
// compile with -std=c++11
// link with -latomspace -latombase
#include <opencog/atomspace/AtomSpace.h>
//#include <opencog/atomspace/SimpleTruthValue.h>
//#include <opencog/atomspace/AttentionValue.h>
//#include <opencog/atomspace/TruthValue.h>
using namespace opencog;
void atomtest()
{
	AtomSpace as;
	Handle h = as.add_node(CONCEPT_NODE, "Cat");
	HandleSeq hseq = {h, h};
	Handle dumbInheritance = as.add_link(INHERITANCE_LINK, hseq);
	std::cout << as << std::endl;

	AtomSpace child_as(&as);


	HandleSeq hseq = {
		as.add_node(VARIABLE_NODE, "$x"),
		as.add_node(TYPE_NODE, "ConceptNode");
	};
	Handle TypedVariableLink = as.add_link(TYPED_VARIABLE_LINK, hseq);

	// steps appear to be set satisfications associated with behaviors that
	// accomplish them.
	opencog::Type PRIOR_STATE_LINK;
	opencog::Type POST_STATE_LINK;

	opencog::Type ATTRIBUTE_LINK;

	Handle opened = as.add_node(CONCEPT_NODE, "opened");
	Handle closed = as.add_node(CONCEPT_NODE, "closed");
	as.add_link(EQUIVALENCE_LINK, {opened, as.add_link(NOT_LINK, closed)});

	// make a step for opening cupboard, relating to reachability
	
	// prior state: $x is in cupboard
	// post state: $x is reachable
	
	// opening something that is closed makes it be open
	Handle openStep = as.add_node(CONCEPT_NODE, "open");
	// open has a variable, what is opened
	// _ABOUT_ open has a variable, what is inside it, becomes reachable
	{
		Handle x = as.add_node(VARIABLE_NODE, "$x");
		as.add_link(PRIOR_STATE_LINK, {
			openStep,
			as.add_link(ATTRIBUTE_LINK, {
				x,
				closed
			})
		});
		as.add_link(POST_STATE_LINK, {
			openStep,
			as.add_link(ATTRIBUTE_LINK, {
				x,
				opened
			})
		});
	}

	// when something is opened, things inside it are reachable.
	// 	this is implied forward with more likelihood than backward
	Handle inside = as.add_node(CONCEPT_NODE, "inside");
	Handle reachable = as.add_node(CONCEPT_NODE, "reachable");
	{
		Handle x = as.add_node(VARIABLE_LINK, "$x");
		Handle y = as.add_node(VARIABLE_LINK, "$y");
		as.add_link(IMPLICATION_LINK, {
			as.add_link(AND_LINK, {
				as.add_link(ATTRIBUTE_LINK, {
					x,
					open
				}),
				as.add_link(ATTRIBUTE_LINK, {
					y,
					inside,
					x
				})
			}),
			as.add_link(ATTRIBUTE_LINK, {
				y,
				reachable
			})
		});
	}

	// we now have two patterns, that together imply that we can open
	// a cupboard to reach a bag of bread if the bag of bread is within the
	// cupboard.
	
	// TO SET VALUE: as.set_value(handle, keyhandle (type), ValuePtr);
	// TO SET TRUTH: as.set_truthvalue(handle, TruthValuePtr);
	// Ptrs are juts typedefs for shared_ptrs and can likely be constructed with vals
	// ValuePtr vp ?= StringValue("hi");
	
	// - ADD CODE TO ATOMS
	// 	we will want a sequence of substeps
	// 	raw strings interspersed with variable references
	// - OUTPUT ATOMSPACE to see how it looks
	// - IMPLEMENT SOLVER USING PATTERNS
	// 	will need a way to
	// 		- get patterns matching requests
	// 		ordered by relevence
	// 			propose using subcontexts and queries that return all results
	//		- inspect pattern content

	// when A is inside B, A is unreachable if B is closed
		// ALTERNATIVELY, can we link this straight into openStep

	// right now we have
	// 	open
	// 	$x was closed
	// 	$x will be opened
	//
	// we'd likely change to something similar to
	// 	open $x
	// 	$x was closed, any $y is within $x
	// 	$x will be opened, all $y will be reachable
	// makes sense to attach close/open to reachability =/
	
}	// etc see https://wiki.opencog.org/w/Manipulating_Atoms_in_C++#Pattern_Matcher
// END OPENCOG ATOMSPACE BLOCK
///////////////////////////////

///////////////////////////////////
// START DYNAMIC CODE LOADING BLOCK  (feel free to move/change/use)
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#include <dlfcn.h>
// link with -ldl
void loadandcall(string func, Context & context) {
	string so = "./" + func + ".so";
	void *handle = dlmopen(LM_ID_NEWLM, "./path.so", RTLD_NOW);
	if (handle == NULL) throw dlerror();
	void *addr = dlsym(handle, func);
	if (addr == NULL) throw dlerror();
	((void (*)(Context &))addr)(context);
	dlclose(handle);
}
// make func.cpp with
// extern "C" void func(Context &) {}
// and compile with
// g++ -shared -fPIC func.cpp -o func.so
// END DYNAMIC CODE LOADING BLOCK
//////////////////////////////////

// instead of stitching compiled strings together, let's use dyload functions?
// in order to do flow control, we can have functions that handle a vector of other functions
// although it makes for a little more work, it makes passing parameters easy

// problem: how does a .cpp file reference another file with a number
// answer: use #includes or interpret the whole shebang as numbers
//         or adjust loadandcall() to handle number lists

// need a way to do nested loops with numbers <===============================

// please provide for handling a parameter next.
//
//      concept: dynamic values aquirable from inside code, i.e. what-number-called-me what-number-comes-after-me
//      thinking the code would likely evolve to handle some inputs differently
unsigned long new_number = 1;

int main()
{
        string ofname;
        unsigned long ofnum;
	{
		struct stat sb;
	        do
	        {
	                ofnum = new_number++;
	                ofname = to_string(ofnum) + ".cpp";
	        } while (-1 != stat(ofname.c_str(), &sb));
	}

        {
                ofstream outfile(ofname);
                vector<string> vals;
		while (true) {
			string val;
                        cin >> val;
			if (val == "") break;
                        vals.push_back(val);
                }
                // when a file runs,  it has numbers on input, it also has numbers equal to it
                // we want to generate run-code with new numbers from input
                // so we generate something with numbers equal to it, and output that
                // we have one ref for the whole shebang
                outfile << "#if !defined(VALCT)" << endl;
                outfile << "  #define VALCT" << " " << vals.size() << endl;
                outfile << "  #define VALS [";
                for (size_t index = 0; index < vals.size(); ++ index)
                {
                        if (index > 0) outfile << ",";
                        outfile << "\"" << vals[index] << "\"";
                }
                outfile << "]" << endl;
    		outfile << "#endif" << endl;
                for (size_t index = 0; index < vals.size();)
                {
                        outfile << endl << "/* " << vals[index] << vals[index+1] << " */" << endl;
                        outfile << "#if defined(IDX)" << endl
                                << "  #undef IDX" << endl
                                << "#endif" << endl;
                        outfile << "#define IDX " << index << endl;
                        outfile << "#if defined(VAL)" << endl
                                << "  #undef VAL" << endl
                                << "#endif" << endl;
                        outfile << "#define VAL \"" << vals[index] << "\"" << endl;
			outfile << "#define ARG \"" << vals[index+1] << "\"" << endl;
                        string fname = vals[index] + ".cpp";
                        ifstream code(fname);
                        size_t ctrd = -1;
                        while (ctrd != 0) {
                                char buf[256];
                                ctrd = code.rdbuf()->sgetn(buf, sizeof(buf));
                                outfile.rdbuf()->sputn(buf, ctrd);
                        }
			index += 2;
                }
		{
			// TODO: hash code to reuse exact stuff, somehow
			string cmd = "g++ -ggdb -std=c++11 -o " + ofname + ".exec " + ofname;
			int status = system(cmd.c_str());
			if (status != 0) throw status;
		}
		// execute output, replacing process, to loop.  use same input.  it should represent our own code.
		// cmd = "./" + ofname + ".exec " <

        }
// read numbers inputs
// open files having the numbers as the names
// cat them all to a gcc process
// execute <-
// run the output <-
}

// need input to pass to output
// propose pass our input & output to it
// so, a number for what we are,
// and a number for what we ran.
//
// also idea of treating whats-next as data
// makes it a little harder to .. make a program out of stuff
// we could load a building-program number
// and it could treat them differently, taking each one as a program-piece
//

// karl obvious knows what he was doing ...
//      ... we were just helping him out of his issue
// [do you want another one karl?]
// what things make / not make issue?
//      karl says everything makes an issue; this seems accurate
//



// I'm thinking about implementating brainstorm-about-brainstorm
// and how this will require process-step- and goal- patterns, and that they
// will be interchangeable, roughly.
//
// Thinking of matching parts of nested pattern contexts ...
// this is similar to the 'grounded' patterns in opencog
// each [context layer] has a [variable-set layer] associated with it --
// variables of that layer depend on the context layer
// 	each one is one pattern

// let's do an example of simple step task.
// - make toast
// 	get bread
// 		open cupboard
// 		remove bag-of-bread
// 		open bag-of-bread
// 		take bread out of bag-of-bread
// 		place bread on counter
// 		close bag-of-bread
// 		return bag-of-bread to cupboard
//		close cupboard
// 	toast bread into toas
// 	butter toas
// 	serve
// make toast
// 	goal: toasted bread for eating
// 	start: in kitchen, human
//
//open cupboard
//	goal: cupboard-contents-accessible
//	start: cupboard closed
//	way: reach-for-cupboard-and-pull-handle-towards-you
//
//		we open the cupboard so as to access the contents inside of it
//		these contents include the bread we are trying to get
//
//	start:
//	var X
//		where X is in cupboard
//	x cannot be gotten
//
//	end:
//	var X
//		where X is in cupboard
//	x can be gotten
//
//	always:
//	var X, Y, Z
//		where X is in Y
//		and Y is in Z
//	X is in Z

// there's a connection between layers here.  we moved from 'make toast' to 'get bread'
// with 'bread is in cupboard' implicit

// goal: have toast
// know: using toaster, bread becomes toast
// do not have bread
// find X: pre: do not know where X is. post: know where X is
// get X: pre: do not have X. post: have X

// available steps:
// open-cupboard: X is in cupboard and you can't get X, now you can get X

// what connects get-bread to can-get-bread?
// how is opening the cupboard the first step for getting the bread?
// get-bread means pick-up-bread-physically
// pick-up-bread-physically requires air-path-to-object
// cupboard prevents this
// can-pick-up-bread-bag

// okay, need-bread:
// bread-is-in-bread-bag -> can get things inside other things via opening
// need bread-bag
// bread-bag-is-in-cupboard -> can get things inside other things via opening


// end-state: have-bread

// step: get X
// start-state: X is reachable,
// [reach to get]
// end-state: have X
//
// apply step to end-state: have-bread.  now we want end-state: bread is reachable.

// step: open Y
// start-state: X is in Y
//              Y is reachable
//              Y is openable 
// [act to open Y]
// end-state: X is reachable

// so if we are working on end-state: have-bread
// we are probably using a general pattern where 'bread' is held by a variable we have.
// we're given our context to include this variable, when we brainstorm solutions.
// in our brainstorm, we look for things that could move towards our end-state.
// we plug in data related to our context to make this work.
// bread is what we want a path to have, so when we see a pattern with 'have X' at end,
// we plug 'bread' in for X.
// we know thing sabout 'bread', so we can plug 'bread is in bread bag' in for 'X is in Y'
// 	or if we don't know whether bread is in bread bag, we can leave that piece
// 	of the pattern unknown, and try it to see if it works.

// it doesn't seem that complicated or that confusingly nested, because the inner patterns
// have their outer context filled when evaluated.

// to reiterate the reminder, this is very logical and is not the only way for thought
// and learning.  we will need a fuzziness to it and to be able to morph it around.
// [using e.g. openiness yes rather than 'is it reachable is it openable']
// so more like
// step: open Y
// end-state: sometimes X is now reachable
//	and relations between X and Y affect the likelihood of that


// THEN: rather than listing these steps out, just give some experiences
// and then brainstorm the similarities among the experiences
// to identify proposed patterns
// that can be used to do stuff.
// 	TODO: lay out precise brainstorming pattern for 1st-stage pattern-generalizing

//		1st stage is not too hard (but insufficient in long term)
//
//		cupboard is closed
//		then
//		[wayne opens cupboard]
//		then
//		wayne has bread
//	becomes pattern proposal for open-cupboard as listed above

// PLUS: To quickly fill in the unexpected gaps:
// 	if you have all the bits in, it should be able to derive general wisdom
// 	without exploring reality. (of course reality is needed to check this)

// ALSO ALSO ALSO: be sure to consider the attribute-patterns.
// opening some A works for getting some B
// 	-> there are discernable patterns available here regarding
// 		B is in A
// 		A is openable
// 		A is reachable

// COMMUNITY: cooperate with peers to accomplish goals.  both like you, and unlike you.
// 	map similarity between the structures of interacting with peers, and internal
// 	structures.

// ONCE YOU CAN THINK:
// 	when you find a good choice, be sure to use it to research how to make the bad
// 		choice just as good. ((all things have inherent value))


// be sure to generalize pattern work, with simple processes that work for many forms
