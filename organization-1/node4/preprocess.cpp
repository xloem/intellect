#include "reference.hpp"

//#include <pcrecpp.h>
#include <library/string.hpp>
#include <stdlib.h>

//using namespace pcrecpp;
using namespace library;

/*
string process(string content)
{
	string output;
	StringPiece input(content);

	StringPiece token;
	RE re_tokens = RE("^\s*\(DECLARE|DEFINE|METHOD\)");
	while (re_tokens.FindAndConsume(&input, &token)) {
		// input is advanced to point past the matched text
		cout << token << endl;
	}
	return output;
	//RE method_declaration = RE("^\(\s*\)METHOD\s\s*\(\S*\)\s\s*\([^(]*\)\s*(\([^)]*\))\s*;\s*$");
}
*/

using namespace library;
int main(int argc, char const * const* argv)
{
	//string filecontent;
	//StringPiece input(filecontent);
	if (argc != 3) {
		stderr::line("Usage: " + string(argv[0]) + " inputfile.*pp outpfile.*xx");
		return -1;
	}

	string infilename = argv[1];
	string outfilename = argv[2];

	string command = "/usr/bin/env bash preprocess.bash '" + infilename + "' '" + outfilename + "'";
	return system(command.c_str());
}
