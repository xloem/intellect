#include <vector>

// goop is atm a vector of stuff that can be moved along
template <typename number_matter>
class goop
{
public:
	using position = int; // TODO: interpolate

	std::vector<number_matter> backing;

	goop() {
		backings.resize(1024);
	}

	position start() {
		return backings.size()/2;
	}

	number_matter value(position where)
	{
		return backing[where]
	}

	position update(position where, number_matter what)
	{
		// TODO: use a contextual goop to decide on the learning shape (wtf? that's impossible. [you'll need more understanding to actually do that])

		// first move the position.  look both ways to see if there is a better position.
		number_matter difference = abs(backing[where] - what);
		if (where > 0) {
		       	number_matter lower_difference = abs(backing[where - 1] - what);
			if (where < backing.size() - 1) {
				if (lower_difference > abs(backing[where + 1] - what)) {
					++ position;
				} else {
					-- position;
				}
			} else {
				-- position;
			}
		} else {
			++ position;
		}

		// the goal spot becomes the goal, and the surrounding areas are pulled towards it
		number_matter other;
		backing[where] = what;
		other = what;
		for (auto whence = where; whence-- >= 0;) {
			other = backing[whence] = (backing[whence] + other) / 2;
		}
		other = what;
		for (auto whither = where; whither < backing.size(); ++ whither) {
			other = backing[whither] = (backing[whither] + other) / 2;
		}

		return position;
	}

private:
	std::vector<number_matter> backing;
};

template <typename number_matter, size_t _count>
class vector_matter
{
public:
	using matter = vector_matter<number_matter, size_t count>;
	constexpr static size_t count = _count;

	std::array<number_matter, count> vector;

	number_matter & operator[](size_t index) { return vector[index]; }
	matter operator+(matter & other)
	{
		matter result;
		for (auto i = 0; i < count; ++ i) {
			result[i] = vector[i] + other[i];
		}
		return result;
	}
	matter operator-(matter & other)
	{
		matter result;
		for (auto i = 0; i < count; ++ i) {
			result[i] = vector[i] - other[i];
		}
		return result;
	}
	matter operator/(number_matter & other)
	{
		matter result;
		for (auto i = 0; i < count; ++ i) {
			result[i] = vector[i] / other;
		}
		return result;
	}
	bool operator<(matter & other)
	{
		number_matter result = 0;
		for (auto i = 0; i < count; ++ i) {
			result += vector[i] - other[i];
		}
		return result < 0;
	}
	matter abs()
	{
		matter result;
		for (auto i = 0; i < count; ++ i) {
			result[i] = vector[i] < 0 ? -vector[i] : vector[i];
		}
		return result;
	}
};

template <typename number_matter, size_t _count>
vector_matter<number_matter, _count> abs(vector_matter<number_matter, _count> & number)
{
	return number.abs();
}

template <size_t count>
using vector_goop = goop<vector_matter<count, double>>;

template <size_t axes>
struct word_knowledge;
{
	std::string word;
	vector_goop<axes> near_words;
};

class esoteric_grimoire
{
public:
	enum axes_of_meaning {
		speaking_or_listening;
		what_to_say;
		how_to_say_it;
		axis_count;
	};

	std::vector<word_knowledge<axis_count>> words_by_number;
	std::map<std::string, size_t> words_by_word;

	vector_goop<1> word_nearness;
	
	struct event {
		size_t word;
		word_knowledge<axis_count>::position meaning_axes;
		decltype(word_nearness)::position nearness_axes;
	};
	std::list<event> memory; // TODO: turn into a goop, and always keep 1 event in. [and store.]

	void happen(event what, bool correction, ssize_t index = -1)
	{
		auto this_word_index = words_by_word[_word];
		auto & this_word = words_by_number[this_word_index];
		event * last_event = nullptr;
		event * this_event;
		if (index == -1) {
			memory.push_back();
			if (memory.size() > 1) {
				last_event = &memory[memory.size() - 2];
			}
			this_event = &memory.back();
		} else if (index > 0) {
			last_event = &memory[index - 1];
		}

		if (correction) {
			// we'll want to update last_event if it exists
			if (last_event != nullptr) {
				last_event.update(
			}
			// and if there's an index to update our overlapping event
		} else {
			*this_event = what;
		}
	}
	
};

#include <sstream>
#include <iostream>

int main()
{
	esoteric_grimoire book_of_echoes;

	// let's do learning, then storage, then testing.

	while (true) {
		while (book_of_echos.words_to_say()) {
			std::cout << " " << book_of_echoes.speak();
		}
		std::cout << std::endl;
	
		std::string line;
		while (std::getline(std::cin, line)) {
			std::istringstream words(line);
			std::string word;

			words >> word;
			if (word == "correction" || word == "correction:") {
				// we'll need a record of the path we took, to correct each part of it.
			} else {
				book_of_echoes.read(word);
			}
	
			while (words >> word) {
				book_of_echoes.read(word);
			}
		}
	}
}

// 1. LEARNING
// 2. STORING
// 3. TESTING
