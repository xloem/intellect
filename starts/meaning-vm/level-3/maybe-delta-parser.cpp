//#include "delta-parser.hpp"

#if 0

#include "../level-2/habits.hpp"
#include "../level-2/ref.hpp"

#include <string>
#include <fstream>

using namespace std;
using namespace intellect::level2;
#define ref intellect::level2::ref

namespace delta {

// SYNTAX PROPOSAL DEVELOPMENT
// list of words, equivalent to file
// words are processes, take ordered parameters
// ideas:
// parameters may be expressions
// 'ref' expression takes 1 symbol, looks up symbols in context  (words are symbols)
// 'context' expression sets context
// 'set context symbol value' sets symbol value in context
// [ ] ?
//
// link a b c 
// is same as
// set a b c
//
// concepts are contexts. <= simplifies a lot.  WE DO NOT NEED TO SOLVE EVERYTHING.
//
// concepts are contexts; if a word is in the active context, it is looked up in it automatically?
//
// simlifying the code to a list of words is helpful.  given you have a parser, it may make sense to store code as
// lists of words.  this maks it easier to generate.  atm code is stored by different kinds of graph structures.
// 	<karl is trying to relate around value of graph strucure vs just words>
// 	<graph structure provides for code-that-is-code-at-all>
// 	<making a random list of words means we need to bootstrap enough relevence to actually do anything>
// 	<we could do that, for example linking each word to possible relevent words>
// 		graph structure sounds more efficient.  the structure is a litle overcomplex, but mostly karl is
// 		inhibited from thinking around it. delta may have a quality idea that helps connect with their resources,
// 		but the idea doesn't seem complete enough to be efficient to do.  -advisor
//
//
// we have to decide when to treat a wordsymbol as a process to evaluate
// karl proposes check context
// so we need a process to make a process
//
// process [ ] maybe
// propose [ ] is special and makes a list of words as an expression

ref parsefile(string filename)
{
	ifstream ss(filename);
	// need language to do things in.  evaluate habits.
	// is file.  lines of text, start to end.
	// how about 'use <process>' lines make the whole chunk be treated by process.
	// source wants array indices.  could require major restructure.
	// we could simulate by walking links.  or using numbers as relative references.
	// we like the learning-numbers-like-humans approach.  let's do that one.
	ref fileref = makeconcept();
	fileref.link("filename", filename);
	ref lastline, nextline, lastword, nextword;
	while (true) {
		string line;
		getline(ss, line);
		if (!ss) break;

		ref lineref = makeconcept();
		if (lastline != "nothing") {
			lineref.link("previous", lastline);
			lastline.link("next", lineref);
		}
		lastline = lineref;

		stringstream words(line);
		while (true) {
			string word;
			words >> word;
			ref wordref = makeconcept();
			lineref.link("word", wordref);
			wordref.link("line", lineref);
			if (lastword != "nothing") {
				wordref.link("previous", lastword);
				lastword.link("next", wordref);
			}
			lastword = wordref;
		}
		fileref.link("line", line);
		ref(line).link("file", fileref);
	}
	return fileref;

}
	void parse(ref firstword)
	{
		if (firstword == "run") {
			// find process and evaluate
		}
	}

	void init(string filename)
	{
		ref file = parsefile(filename);
		ref firstword = *file.getAll("line").begin()->getAll("word").begin();
		
		// the world is made of concepts, which are made of typed links to other
		// concepts.  like apple color red
		map<string,ref> processes = {
			{"using",
				// process says how to turn words into references when used
			},
			{"run".
				// find next word and evaluate as process
			},{"store",
				// evaluate as store "name" until "terminator"
				// maybe ??
			},{"get",
				// evaluate as get "name" "type", puts on stack
				// maybe ??
				// it sounds like you don't want any variables.
				// you act only on the contents of the file.
				// you want to produce new files, maybe?
				// 	want to produce processes that produce processes
				// 	act on streams of words to make more.
				// okay, let's say output is appended to the stream.
				// maybe we could have a focus pointer in the working stream
				// build links between words based on focuses
				// we might need a way to hold a reference to other words
				// maybye overlapping streams, and the process decides how
				// to move them ...
				// how about each word can be a stream.  we start in file
				// stream.
				// it sounds like this approach is highly immature.  shall
				// we go back to the existing approach, premade, yes.
			},{"link",
				// considering link "name" "type" "target"
			}
		};
		// wordref word1 word2 word3
		// lineref line1 line2 line3
		// goto line1
		// examine line2 put in line4 <- this doesn't make for turing machine
		//  [it sounds like you want me to fill in a generic programming language.  i don't have a lot of capacity for that with the uh control going on.  takes a long time.  wrote one already, but doesn't match your preferences]
		//
	}

// delta processes for bootstrapping
// [the lines _are_ the code.  no need for special line thing.  just self-reference it seems.]
// okay, so we have active-process, and move to next-process.  each line could be a process,
// or each word.  maybe each word is possibly a process, can check next-word, previous-word.
// and then new words are made, nd resulting file could be output for next iteration.

/*
 * process run
 *
 * if line is extra, ignore.
 * if line is words, run words.
 * special words run c/c++ code
 * otherwise, they run processes.
 * 	okay.  we can make processes for the existing functionality.
 * 	how do we output stuff?  return values?
 * ... thinking... could modify a local context
 * 		takes context in file, i suppose.
 * 		word, line, file
 * 		then can add things _not_ in file, to that context.
 */

// we have 'habits' that take named inputs and produce named outputs, generally 'result'

// make-more-deltay, or-implement-a-process-to-do-things-with-it.

}

#endif
