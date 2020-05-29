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

		static constexpr char const * silence = "special-silence";
		static constexpr char const * start-of-conversation = "special-start-of-conversation";
		static constexpr char const * end-of-conversation = "special-end-of-conversation";

		thinker() {
			first_thought.reset(new thought({start-of-conversation}));
			last_thought = first_thought;
			current_thought = first_thought;
		}
	
		void hear(string word, bool internal = false) {
			if (!internal) {
				cerr << " -- Heard: " << word << endl;
				// if internal we could open a sublist for thinking, referencing memory -karl-idea
			}

			auto & word_ref = current_book.index[[word]];
			if (!word_ref) {
				word_ref.reset(new thought({word}));
			}
			if (current_thought->next) {
				if (current_thought->next->word != word) {
					cerr << " -- OMIGOD!  '" << word << "'! " << endl;
					// be nice to link to more here - bounds-idea
					// be nice to not repeat too much
				} else if (!internal) {
					cerr << " -- Yes, '" << word << "' comes after '" << current_thought->word << "'... " << endl;
				}
			} else {
				cerr << " -- How new to " << (internal ? "think" : "hear") << ": '" << word << "' =)" << endl;
			}
			last_thought = current_thought;
			current_thought = word_ref;
			last_thought->next = current_thought;
		}
		string say() {
			if (current_thought->next) {
				hear(current_thought->next->word, true);
			} else {
				hear(silence, true);
			}
			return current_thought->word;
		}
	};
}

int main()
{
	next_word::thinker thinker;
	while (!cin.eof()) {
		string word;
		while ((word = tools::line_word()).size()) {
			thinker.hear(word);
		}
		if (cin.eof()) {
			thinker.hear(next_word::thinker::end-of-conversation);
		} else {
			thinker.hear(next_word::thinker::silence);
		}
		while ((word = thinker.say()) != next_word::thinker::silence) {
			cout << " " << word;
		}
		cout << endl << endl;
	}
}
