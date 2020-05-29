#include "basic_includes.hpp"
#include "tools.hpp"

using amount = double;

namespace next_word
{
	class thought;
	using reference = shared_ptr<thought>;
	using ref = reference;

	class thought
	{
	public:
		virtual string & word() = 0;

		virtual bool next-is-known() = 0;
		virtual ref next() = 0;

		virtual bool note-next(reference next, bool capacity) = 0;

		virtual bool merge(ref) {} // TODO because spreads pointer change

		shared_ptr<any> more; // could turn into a virtual function
		//map<amount, ref> next;
	};

	class in-a-row : public thought
	{
	public:
		in-a-row(string word)
		: member-word(word)
		{ }
		string & word() override { return member-word; }

		bool next-is-known() override { return next().get() != nullptr; }
		ref next() override { return member-next; }

		bool note-next(reference next, bool capacity) override
		{
			if (next-is-known() && this->next() != next) {
				if (capacity) { return false; }
				cerr << " -- I want a way to reach out to more here!" << endl;
				// might randomly just return true here
			}
			member-next = next;
			return true;
		}


	private:
		string member-word;
		ref member-next;
	};

	class book {
	public:
		map<string, ref> index; // hmm.  a book is near context.
	};

	
	class thinker {
	public:
		reference first-thought;
		reference last-thought;
		reference current-thought;

		book current_book;

		static constexpr char const * silence = "special-silence";
		static constexpr char const * start-of-conversation = "special-start-of-conversation";
		static constexpr char const * end-of-conversation = "special-end-of-conversation";

		thinker() {
			first-thought.reset(new in-a-row(start-of-conversation));
			last-thought = first-thought;
			current-thought = first-thought;
		}
	
		void hear(string word, bool internal = false) {
			if (!internal) {
				cerr << " -- Heard: " << word << endl;
				// if internal we could recurse making a sublist of words for thinking, referencing memory -karl-idea
			}

			auto & word-ref = current_book.index[[word]];
			if (!word-ref) {
				word-ref.reset(new in-a-row({word}));
			}	
			if (current-thought->next-is-known()) {
				if (current-thought->next()->word() != word) {
					cerr << " -- OMIGOD!  '" << word << "'! " << endl;
				} else if (!internal) {
					cerr << " -- Yes, '" << word << "' comes after '" << current-thought->word() << "'... " << endl;
				}
			} else {
				cerr << " -- How new to " << (internal ? "think" : "hear") << ": '" << word << "' =)" << endl;
			}
			last-thought = current-thought;
			current-thought = word-ref;
			last-thought->note-next(current-thought, false);
		}
		string say() {
			if (current-thought->next-is-known()) {
				hear(current-thought->next()->word(), true);
			} else {
				cerr << " -- I need a way to reach out to more here!" << endl;
				hear(silence, true);
			}
			return current-thought->word();
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
		while (true) {
			word = thinker.say();
			if (word.compare(0, 8, "special-") == 0) {
				cout << " -- [" << word << "]";
			} else {
				cout << " " << word;
			}
			if (word == next_word::thinker::silence || word == next_word::thinker::end-of-conversation) { break; }
		}
		cout << endl << endl;
	}
}
