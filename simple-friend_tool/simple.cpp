#include "basic_includes.hpp"
#include "tools.hpp"

using amount = double;
using quantity = size_t;

namespace next_word
{
	class thought;
	using reference = shared_ptr<thought>;
	using ref = reference;

	class thought
	{
	public:
		thought(string word)
		: member-word(word) { }

		virtual string & word() { return member-word; }

		virtual amount next-is-known() = 0;
		virtual ref next() = 0;

		virtual bool note-next(reference next, bool capacity) = 0;

		virtual void merge(ref) {} // TODO because spreads pointer change

		shared_ptr<any> more; // could turn into a virtual function
		//map<amount, ref> next;

	protected:
		string member-word;
	};

	class in-a-row : public thought
	{
	public:
		in-a-row(string word)
		: thought(word)
		{ }

		amount next-is-known() override { return next().get() != nullptr ? 1 : 0; }
		ref next() override { return member-next; }

		bool note-next(reference next, bool capacity) override
		{
			if (next-is-known() > 0) {
				if (this->next() != next) {
					if (capacity) { return false; }
					cerr << " -- I want a way to reach out to more here!  I didn't expect to hear this!" << endl;
					// might randomly just return true here
				}
			}
			member-next = next;
			return true;
		}


	private:
		ref member-next;
	};

	class likelihood : public thought
	{
	public: // we have unsureness with likelihood don't we
		// don't fret about stats, try to do quick solution
		likelihood(string word)
		: thought(word), member-total-experiences(0)
		{ }
		
		amount next-is-known() override
		{
			if (member-total-experiences > 2 && member-experiences.size() == 1) {
				return true;
			} else if (member-total-experiences > 0) {
				amount confidence = get-best()->second / amount(member-total-experiences + 1) * 100;
				return confidence;
				/*
				if (confidence <= 50) {
					cerr << " -- I want a way to reach out to more here.  I don't know how to convey that I have a poor guess." << endl;
				} else {
					cerr << " -- I want a way to reach out to more here.  I don't know how to convey that I expect " << tools::quote_special(next()->word()) << " with " << quantity(confidence) << "% confidence." << endl;
					return true;
				}
				*/
			}
			return false;
		}

		ref next() override
		{
			return get-best()->first;
		}

		// for decisions we could use a note-not-next: a few different ways to handle that exist

		bool note-next(reference next, bool capacity) override
		{
			if (member-experiences.size() == 1 && member-total-experiences > 1 && next != member-experiences.begin()->first) {
				if (capacity) { return false; }
				cerr << " -- I want a way to reach out to more here.  I used to know how to predict this and no longer do." << endl;
			} else if (member-total-experiences < 3) {
				cerr << " -- How helpful to consider: " << tools::quote_special(next->word()) << " =)" << endl;
			} else {
				cerr << " -- How boring to see again: " << tools::quote_special(next->word()) << endl;
			}
			if (member-experiences.count(next) == 0) {
				member-experiences[[next]] = 1;
			} else {
				++ member-experiences[[next]];
			}
			++ member-total-experiences;
			if (member-experiences.size() > 1) {
				cerr << " -- I want a way to reach out to more here.  I don't know what to do with these multiple options." << endl;
			}
			return true;
		}
		
	private:
		map<ref, quantity> member-experiences;
		quantity member-total-experiences;

		decltype(member-experiences)::iterator get-best()
		{
			auto best = member-experiences.begin();
			for (auto start = best; start != member-experiences.end(); ++ start)
			{
				if (start->second > best->second) {
					best = start;
				}
			}
			return best;
		}
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
			first-thought.reset(new likelihood(start-of-conversation));
			last-thought = first-thought;
			current-thought = first-thought;
		}
	
		void hear(string word, bool internal = false) {
			if (!internal) {
				cerr << " -- Heard: " << tools::quote_special(word) << endl;
				// if internal we could recurse making a sublist of words for thinking, referencing memory -karl-idea
			}

			auto & word-ref = current_book.index[[word]];
			if (!word-ref) {
				word-ref.reset(new likelihood({word}));
			}	
			auto expectation-confidence = current-thought->next-is-known();
			if (expectation-confidence > 50) {
				if (current-thought->next()->word() != word) {
					cerr << " -- OMIGOD!  '" << word << "'! " << endl;
				} else if (!internal) {
					cerr << " -- Yes, '" << word << "' comes after '" << current-thought->word() << "'... " << endl;
				}
			} else {
				// TODO: we also do this inside likelihood::note-next, and it doesn't know if it's internal?
				cerr << " -- How new to " << (internal ? "think" : "hear") << ": " << tools::quote_special(word) << " =)" << endl;
			}
			last-thought = current-thought;
			current-thought = word-ref;
			bool expected = last-thought->note-next(current-thought, true);
			if (!expected) {
				cerr << " -- I need a way to reach out here.  I've promised to adjust my thinking and I'm not sure how." << endl;
			}
		}
		string say() {
			if (current-thought->next-is-known()) {
				hear(current-thought->next()->word(), true);
			} else {
				cerr << " -- I need a way to reach out to more here!  I don't know what to do!" << endl;
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
			cout << " " << tools::quote_special(word, false);
			if (word == next_word::thinker::silence || word == next_word::thinker::end-of-conversation) { break; }
		}
		cout << endl << endl;
	}
}
