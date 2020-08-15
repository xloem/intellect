
#define ___STATIC_
#define ___EXTERN_ extern
# 1 "preprocess.cpp"
#include "reference.stable.hxx"

//#include <pcrecpp.h>
#include <library/string.hpp>
#include <stdlib.h>

#include <fstream>

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

	/*
	{
		std::ifstream infile(infilename.std());
		std::ofstream outfile(outfilename.std());
	
		
		bool linenumbers = true;
		auto line_number = [&](int number){
			if (!linenumbers) { outfile << "//"; }
			outfile << ("# " + string(number) + "\"" + infilename + "\"\n").std();
		};
#undef ___CLASSNAME___
#undef ___CLASSNAMESTR___
# 53 "preprocess.cpp"

		outfile <<
			"#define ___STATIC_\n"
			"#define ___EXTERN_ extern\n";
		line_number(1);
	
		// getline returns infile, and infile evaluates to false on no_result_because_no_more_data
		library::string line;
		while (std::getline(infile, line.std())) {

		}
		outfile.close();

	}
	*/
	// change preprocess.bash to append to existing outfile,
	// and move bits to above
	{
		string command = "/usr/bin/env bash node4-preprocess.bash '" + infilename + "' '" + outfilename + "'";
		return system(command.c_str());
	}
}
