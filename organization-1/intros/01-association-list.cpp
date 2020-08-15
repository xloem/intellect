// goal: associate words in ways
//
// user inputs a word, associated words are output.

#include <node4/easy.hpp>




int main(int argc, char ** argv)
{
	easy welcome = "Thought association palace."_e;
	welcome.line();
	
	input user("input"_e);

	//easy word = user.line();

	easy thought-input-ways("thought-input-ways");
	// 'easy' means each word means precise thought
	// 'reference' may be different things
	
	reference current-thought; // associated with words
		//"MANY THOUGHTS HAVE WORDS FOR THEM.
		 //ALL THOUGHTS HAVE NEARBY WORDS, THOUGH."
		 //"Can identify thought based on nearest words."

	thought-input-ways["thought"] = (std::any)(std::function<void()>)[&]() {
		// ask for a word; once thought is unique or correct pick it.
		easy thought = user.word();
			// easy is to be easy to use:
			// to that end we would like to be able to make
			// unqiue thoughts either without a name, or differing
			// but with the same name, using it
	};

	thought-input-ways["add-thought"] = (std::any)(std::function<void(reference)>)[&](reference to-what) {
	};
	thought-input-ways["thought-category"] = (std::any)(std::function<void(reference)>)[&](reference to-what) {
	};

	easy word = user.line();
	"You entered: "_e.write();
	word.line();
	//easy("You entered: " + word.data()).line();
	
	// default-methods appear to be stored on the link, and work.  they can actually be called on any class.
}
