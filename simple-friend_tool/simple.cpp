#include "basic_includes.hpp"
#include "tools.hpp"

using amount = double;
using quantity = size_t;

namespace words
{
		static constexpr char const * silence = "special-silence";
		static constexpr char const * start-of-conversation = "special-start-of-conversation";
		static constexpr char const * end-of-conversation = "special-end-of-conversation";
}

namespace next-memory
{
	template <typename Data>
	class memory;

	template <typename Data = any>
	class memory : public tools::registered<memory<Data>>
	{
	public:
		using reference = shared_ptr<memory<Data>>;

		static reference make(reference previous, Data data = {})
		{
			if (previous) {
				new memory<Data>(previous, data);
				return previous->member-next;
			}
			reference result;
			result.reset(new memory<Data>({}, data));
			return result;
		}

		reference previous() { return member-previous; }
		reference next() { return member-next; }
		Data & data() { return member-data; }
		/*
		template <typename Type>
		Type & data() { return any_cast<Type>(member-data); }
		*/

		// memories could reference other memories for similarity,
		// this would provide for prediction and learning.

		shared_ptr<any> more;
	
	private:
		memory(reference previous, Data data)
		: member-previous(previous),
		  member-data(data)
		{
			if (member-previous) {
				member-previous->member-next.reset(this);
			}
		}

		reference member-previous;
		reference member-next;
		Data member-data;
	};

	/*
	class word-thinker
	{
	public:
		reference first-memory;

		word-thinker()
		{
			first-memory.reset(new memory({}), word-event{words::start-of-conversation, word-event::boundary-prior});
		}

		void hear(string word, bool internal = false)
		{
		}

		string say()
		{
			return {};
		}
	};
	*/
}

namespace next_word
{
	class thought;
	using reference = shared_ptr<thought>;
	using ref = reference;

	class thought : public tools::registered<thought>
	{
	public:
		thought(string word)
		: member-word(word)
		{}
		virtual ~thought() {}

		void replace(reference other)
		{
			for (auto thought : registry) {
				thought->replace(this, other);
			}
		}

		virtual void replace(thought *old, reference newer) = 0;

		virtual string & word() { return member-word; }

		virtual amount next-is-known() = 0;
		virtual ref next() = 0;

		// returns false if caller-has-capacity and event is too confusing
		// to predict from.
		virtual bool note-next(reference next, bool caller-has-capacity) = 0;

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

		void replace(thought * old, reference newer) override
		{
			if (member-next.get() == old) {
				member-next = newer;
			}
		}

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
	public:
		likelihood(string word)
		: thought(word), member-total-experiences(0)
		{ }
		
		void replace(thought * old, reference newer) override
		{
			auto found = member-experiences.end();
			for (auto iterator = member-experiences.begin(); iterator != found; ++ iterator) {
				if (iterator->first.get() == old) {
					found = iterator;
					break;
				}
			}
			if (found != member-experiences.end()) {
				member-experiences[[newer]] = found->second;
				member-experiences.erase(found);
			}
		}
		
		amount next-is-known() override
		{
			/*if (member-total-experiences > 2 && member-experiences.size() == 1) {
				return true;
			} else*/ if (member-total-experiences > 0) {
				amount confidence = get-best()->second / amount(member-total-experiences + 1) * 100;
				return confidence;
			}
			return 0;//false;
		}

		ref next() override
		{
			auto best = get-best();
			if (best->second != member-total-experiences) {
				cerr << " -- I'm a little worried.  I'm stating " << best->first->word() << " as following " << word() << " but it could be any of " << members-to-string() << endl;
			}
			return get-best()->first;
		}

		// for decisions we could use a note-not-next: a few different ways to handle that exist

		bool note-next(reference next, bool capacity) override
		{
			// all these words are an eumulation of smooth feedback.
			// ideally this incrementation is a smooth understanding.
			// we need time to act on something being new.
			if (member-experiences.size() <= 1) {
				if (member-total-experiences > 1 && next != member-experiences.begin()->first) {
					if (capacity) { return false; }
					cerr << " -- I want a way to reach out to more here.  I used to know how to predict this and no longer do." << endl;
				} else if (member-total-experiences >= 3) {
					cerr << " -- How boring to see again: " << tools::quote_special(next->word()) << endl;
				} else {
					cerr << " -- How helpful to consider: " << tools::quote_special(next->word()) << " =)" << endl;
				}
			} else {
				cerr << " -- I got " << next->word() << " after " << word() << " and already expected: " << members-to-string() << endl;
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
		
		string members-to-string()
		{
			string result;
			bool first = true;
			for (auto & item : member-experiences) {
				if (first) {
					first = false;
				} else {
					result += " ";
				}
				result += item.first->word() + " x" + to_string(item.second);
			}
			return result;
		}
	};

	class book {
	public:
		map<string, ref> index; // hmm.  a book is near context.
	};

	class word-event
	{
	public:
		string word;
		enum {
			ongoing,
			boundary-prior,
			boundary-after,
			boundary
		} change;

		word-event(string word, word-event & previous)
		: word(word)
		{
			if (tools::starts_with(word, "special-")) {
				change = boundary;
				if (previous.change == ongoing) {
					previous.change = boundary-after;
				}
			} else if (previous.change == boundary) {
				change = boundary-prior;
			}
		}

		word-event(string word)
		: word(word)
		{
			if (tools::starts_with(word, "special-")) {
				change = boundary;
			} else {
				change = boundary-prior;
			}
		}
	};

	
	class thinker {
	public:
		using memory = next-memory::memory<word-event>;
		using reference = shared_ptr<memory>;

		reference first-thought;
		reference last-thought;
		reference current-thought;

		// book current_book; // is there capacity to look up from the nearby graph?

		static constexpr char const * silence = words::silence;
		static constexpr char const * start-of-conversation = words::start-of-conversation;
		static constexpr char const * end-of-conversation = words::end-of-conversation;

		reference look_up_word(std::string word)
		{
			// it is helpful when looking up a word from likelihood,
			// to know how we reached each state

			// the intent of first-thought is to provide that,
			// but it may not be true

			// we've been avoiding for a long time having a listed
			// record of states.  maybe we can first implement
			// new-states.
			return {};
		}

		thinker() {
			first-thought = memory::make({}, word-event(start-of-conversation));
			last-thought = first-thought;
			current-thought = first-thought;
		}
	
		void hear(string word, bool internal = false) {
			if (!internal) {
				cerr << " -- Heard: " << tools::quote_special(word) << endl;
				// if internal we could recurse making a sublist of words for thinking, referencing memory -karl-idea
			}

			reference next-thought = memory::make(current-thought, word-event(word, current-thought->data()));

			/*
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
			*/

			cerr << " -- I need a way to reach out to more here.  I don't know how to see if this has happened before." << endl;


			last-thought = current-thought;
			current-thought = next-thought;
			/*
			bool expected = last-thought->note-next(current-thought, true);
			if (!expected) {
				cerr << " --- Woah...! " << tools::quote_special(last-thought->word()) << " " << tools::quote_special(word) << endl;
				// we'll need a way of branching from the old situation to the new one.  the logic here atm is flawed.
				current-thought.reset(new likelihood(word));
			}
			*/
		}
		string say() {
			/*if (current-thought->next-is-known()) {
				hear(current-thought->next()->word(), true);
			} else {*/
				cerr << " -- I need a way to reach out to more here!  I don't know what to do!" << endl;
				hear(silence, true);
			//}
			return current-thought->data().word;
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
