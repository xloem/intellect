#include "basic_includes.hpp"
#include "tools.hpp"

using value = double;

namespace next_word
{
	class thought;
	using reference = shared_ptr<thought>;
	using ref = reference;

	class thought {
	public:
		string word;
		//map<value, ref> next;
		ref next;
	
		shared_ptr<any> more;
	}; 

	class book {
	public:
		map<string, ref> index; // hmm.  a book is near context.
	};

	
	class thinker {
	public:
		reference first_thought;
		reference last_thought;
		reference current_thought;

		book current_book;


		thinker() {
			first_thought.reset(new thought({"special-start-of-conversation"}));
			last_thought = first_thought;
			current_thought = first_thought;
		}
	
		void hear(string word) {
			auto & word_ref = current_book.index[[word]];
			if (!word_ref) {
				word_ref.reset(new thought({word}));
			}
			if (current_thought->next) {
				if (current_thought->next->word != word) {
					cerr << " -- OMIGOD!  '" << word << "'! " << endl;
					// be nice to link to more here
					// be nice to not repeat too much
				} else {
					cerr << " -- Yes, '" << word << "' comes after '" << current_thought->word << "'... " << endl;
				}
			} else {
				cerr << " -- How new: '" << word << "' =)" << endl;
			}
			last_thought = current_thought;
			current_thought = word_ref;
			last_thought->next = current_thought;
		}
		string say() {
			if (current_thought->next) {
				hear(current_thought->next->word);
			} else {
				hear("");
			}
			return current_thought->word;
		}
	};
}

int main()
{
	next_word::thinker thinker;
	while (true) {
		string word;
		while ((word = tools::line_word()).size()) {
			cerr << "heard " << word << endl;
			thinker.hear(word);
		}
		thinker.hear("");
		while ((word = thinker.say()).size()) {
			cout << " " << word;
		}
		cout << endl;
	}
}
