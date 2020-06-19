#include <pcrecpp.h>
#include <library/string.h>

using namespace pcrecpp;
using namespace library;

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

int main()
{
	string filecontent;
	StringPiece input(filecontent);
}
