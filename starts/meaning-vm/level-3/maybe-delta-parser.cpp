#include "delta-parser.hpp"

#include "../level-2/ref.hpp"

#include <string>
#include <fstream>

using namespace std;

namespace delta {

void parsefile(string filename)
{
	ifstream ss(filename);
	// need language to do things in.  evaluate habits.
	// is file.  lines of text, start to end.
	// how about 'use <process>' lines make the whole chunk be treated by process.
	ref fileref = makeconcept();
	while (true) {
		string line;
		getline(ss, line);
		if (!ss) break;

		ref lineref = makeconcept();

		stringstream words(line);
		while (true) {
			string word;
			words >> word;
			lineref.link("word", word);
		}
		fileref.link("line", line);
	}
}

// delta processes for bootstrapping
// [the lines _are_ the code.  no need or special line thing.  just self-reference it seems.]
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
