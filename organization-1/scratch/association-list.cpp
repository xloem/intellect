// goal: associate words in ways
//
// user inputs a word, associated words are output.

#include <node4/easy.hpp>

int main(int argc, char ** argv)
{
	easy welcome = "Association palace.  Enter a starting term."_e;
	welcome.line();
	
	input user("input"_e);
	easy word = user.line();

	easy("You entered: " + word.data()).line();
}
